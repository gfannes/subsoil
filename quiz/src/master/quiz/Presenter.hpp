#ifndef HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED

#include <quiz/Model.hpp>
#include <quiz/View.hpp>
#include <gubg/OnlyOnce.hpp>
#include <gubg/Range.hpp>
#include <chrono>
#include <optional>

namespace quiz { 

    class Presenter: public View::Events, public Model::Events
    {
    public:
        Presenter(Model &model, View &view): model_(model), view_(view)
        {
            view_.inject_events_receiver(this);
            model_.inject_events_receiver(this);
        }

        bool quit() const {return quit_;}

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(!quit());

            if (reset_background_timepoint_)
            {
                view_.set_score(score_);
                if (reset_background_timepoint_.value() <= Clock::now())
                {
                    view_.set_background_color(sf::Color::Black);
                    reset_background_timepoint_.reset();
                }
            }
            else
            {
                view_.set_score("");
                view_.set_board(board_names_, board_stati_);
            }

            MSS(model_(error));
            MSS(view_(error));

            MSS_END();
        }

        //View::Events implementation
        void ve_close() override
        {
            quit_ = true;
            view_.close();
        }
        void ve_key(char ch) override
        {
            std::cout << ch; std::flush(std::cout);
            model_.process_char(ch);
        }

        //Model::Events implementation
        void me_show_question(const Question *question_ptr) override
        {
            if (!question_ptr)
            {
                view_.set_title("");
                view_.set_description("");
                view_.set_image("");
                view_.load_music("");
                return;
            }

            const auto &question = *question_ptr;
            view_.set_title(question.title);
            if (question.descriptions.empty())
                view_.set_description("");
            else
                view_.set_description(question.descriptions[0]);
            view_.set_image(question.image_fn);
            view_.load_music(question.music_fn, question.offset, question.pitch);
        }
        void me_show_answer_team_order(const std::string &team, const std::string &order) override
        {
            view_.set_answer_team(team);
            view_.set_answer_order(order);
        }
        void me_play_music(bool b, bool restart) override
        {
            view_.play_music(b, restart);
        }
        void me_answer_was_correct(bool b) override
        {
            view_.set_background_color(b ? sf::Color::Green : sf::Color::Red);
            reset_background_timepoint_ = Clock::now()+std::chrono::milliseconds{2000};
            view_.play_sound_answer_was_correct(b);
        }
        void me_show_error(const std::string &msg) override
        {
            view_.set_title(msg);
            view_.set_description("");
            view_.set_score("");
            view_.set_image("");
            view_.load_music("");
        }
        void me_show_score(const Score *score) override
        {
            std::ostringstream oss;
            if (score)
            {
                oss << "Score:" << std::endl;
                for (const auto &p: score->team__score)
                    oss << " team " << p.first << ": " << p.second << std::endl;
            }
            score_ = oss.str();
        }
        void me_show_board(const Board *board) override
        {
            board_names_.clear();
            board_stati_.resize(0);

            if (!!board)
            {
                unsigned int cix = 0;
                for (const auto &p: board->group__categories)
                {
                    const auto &category = p.second;
                    board_names_ += std::to_string(cix)+"  "+category.name+"\n";
                    for (auto i = 0; i < category.status.size(); ++i)
                    {
                        if (i >= board_stati_.size())
                            board_stati_.resize(i+1);
                        board_stati_[i] += std::string(1, category.status[i])+"\n";
                    }

                    ++cix;
                }
            }
        }

    private:
        bool quit_ = false;
        Model &model_;
        View &view_;

        std::string score_;
        std::string board_names_;
        std::vector<std::string> board_stati_;

        using Clock = std::chrono::system_clock;
        std::optional<Clock::time_point> reset_background_timepoint_;
    };

} 

#endif
