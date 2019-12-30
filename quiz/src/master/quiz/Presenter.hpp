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

            if (reset_background_timepoint_ && reset_background_timepoint_.value() <= Clock::now())
            {
                view_.set_background_color(sf::Color::Black);
                reset_background_timepoint_.reset();
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
                view_.set_description("");
                view_.set_image("");
                view_.load_music("");
                return;
            }

            const auto &question = *question_ptr;
            view_.set_description(question.description);
            view_.set_image(question.image_fn);
            view_.load_music(question.music_fn);
        }
        void me_show_answer_team_order(const std::string &team, const std::string &order) override
        {
            view_.set_answer_team(team);
            view_.set_answer_order(order);
        }
        void me_play_music(bool b) override
        {
            view_.play_music(b);
        }
        void me_answer_was_correct(bool b) override
        {
            view_.set_background_color(b ? sf::Color::Green : sf::Color::Red);
            reset_background_timepoint_ = Clock::now()+std::chrono::milliseconds{2000};
            view_.play_sound_answer_was_correct(b);
        }
        void me_show_error(const std::string &msg) override
        {
            view_.set_description(msg);
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
            view_.set_score(oss.str());
        }

    private:
        bool quit_ = false;
        Model &model_;
        View &view_;

        using Clock = std::chrono::system_clock;
        std::optional<Clock::time_point> reset_background_timepoint_;
    };

} 

#endif
