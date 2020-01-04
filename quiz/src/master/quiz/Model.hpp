#ifndef HEADER_quiz_Model_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Model_hpp_ALREADY_INCLUDED

#include <quiz/Question.hpp>
#include <quiz/Score.hpp>
#include <gubg/mss.hpp>
#include <vector>

namespace quiz { 

#define unroll(x) x(Idle) x(NewQuestion) x(Thinking) x(Answer) x(Error)
    enum class State
    {
#define x(name) name,
        unroll(x)
#undef x
    };
    inline std::ostream &operator<<(std::ostream &os, State state)
    {
        switch (state)
        {
#define x(name) case State::name: os << #name; break;
            unroll(x)
#undef x
        }
        return os;
    }
#undef unroll

    class Model
    {
    public:
        class Events
        {
        public:
            virtual void me_show_question(const Question *question_ptr) = 0;
            virtual void me_show_answer_team_order(const std::string &team, const std::string &order) = 0;
            virtual void me_play_music(bool b, bool restart) = 0;
            virtual void me_answer_was_correct(bool b) = 0;
            virtual void me_show_error(const std::string &msg) = 0;
            virtual void me_show_score(const Score *score) = 0;
        };

        Model()
        {
            if (!construct_(ctor_error_) && ctor_error_.empty())
                ctor_error_ = "Unknown construction error for Model";
        }

        void inject_events_receiver(Events *events) {events_ = events;}

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(ctor_error_.empty(), error = ctor_error_);
            MSS(!!events_, error = "Model.events_ not set");

            if (answer_team_.empty() && !answer_order_.empty())
            {
                answer_team_ = answer_order_.substr(0, 1);
                answer_order_ = answer_order_.substr(1);
                MSS(change_state_(State::Answer, sm_error_));
            }

            events_->me_show_answer_team_order(answer_team_, answer_order_);
            events_->me_show_score(state_ == State::Idle ? &score_ : nullptr);

            MSS_END();
        }

        void process_char(char ch)
        {
            if (!process_char_(ch))
            {
                if (events_)
                    events_->me_show_error(std::string("Error: ")+sm_error_);
            }
        }

    private:
        const Question *get_question_(unsigned int ix) const
        {
            if (ix >= questions_.size())
                return nullptr;
            return &questions_[ix];
        }

        bool process_char_(char ch)
        {
            MSS_BEGIN(bool);

            MSS(!!events_);

            sm_error_.clear();

            if ('0' <= ch && ch <= '9')
                number_ = number_.value_or(0)*10 + (ch-'0');
            else if (ch == 'q')
                MSS(change_state_(State::NewQuestion, sm_error_));
            else if ('a' <= ch && ch <= 'e')
            {
                switch (state_)
                {
                    case State::Idle:
                    case State::Error:
                        return true;
                        break;
                    default:
                        if (answer_order_.find(ch) != std::string::npos)
                            return true;
                        if (!answer_team_.empty() && answer_team_[0] == ch)
                            return true;
                        answer_order_.push_back(ch);
                        break;
                }
            }
            else if (ch == 'x')
            {
                if (!answer_team_.empty())
                {
                    --score_.team__score[answer_team_];
                    events_->me_answer_was_correct(false);
                    MSS(change_state_(State::Thinking, sm_error_));
                }
            }
            else if (ch == 'v')
            {
                if (!answer_team_.empty())
                {
                    ++score_.team__score[answer_team_];
                    events_->me_answer_was_correct(true);
                    MSS(change_state_(State::Idle, sm_error_));
                }
            }
            else if (ch == 27)
                MSS(change_state_(State::Idle, sm_error_));
            else if (ch == ' ')
            {
                if (current_question_)
                {
                    auto &question = current_question_.value();
                    auto it = question.descriptions.begin();
                    if (it != question.descriptions.end())
                    {
                        question.descriptions.erase(it);
                    }
                    events_->me_show_question(&question);
                }
            }
            else if (ch == 'r')
                events_->me_play_music(true, true);

            MSS_END();
        }

        bool construct_(std::string &error)
        {
            MSS_BEGIN(bool);

            if (false)
            {
                score_.team__score["a"] = 0;
                score_.team__score["b"] = 0;
                score_.team__score["c"] = 0;
                score_.team__score["d"] = 0;
            }

            const std::string folder = "quiz/media/question/";
            auto add_text = [&](const std::string &title, const std::string &descr = "") -> Question&
            {
                Question q;
                q.title = title;
                if (!descr.empty())
                    q.descriptions.push_back(descr);
                questions_.push_back(q);
                return questions_.back();
            };
            auto add_image = [&](const std::string &title, const std::string &image_fn) -> Question&
            {
                Question q;
                q.title = title;
                q.image_fn = folder+image_fn;
                questions_.push_back(q);
                return questions_.back();
            };
            auto add_music = [&](const std::string &title, const std::string &music_fn, float pitch = 1.0f) -> Question&
            {
                Question q;
                q.title = title;
                q.music_fn = folder+music_fn;
                q.pitch = pitch;
                questions_.push_back(q);
                return questions_.back();
            };
            add_text("Leefwereld van je kinderen", "How many bitches\ncan you fit in a Tesla?");
            add_image("Vertaal vanuit het Azarts", "azart_spaghetti.png");
            add_image("Leg dit begrip uit", "azart_hypotenusa.png");
            add_text("Doe de vergelijking kloppen", "1000 = \n8 8 8 8 8 8 8 8\nGebruik enkel:\n+ - x : ( ) of niets");
            add_text("Roepen maar", "Vl dz vrg n mt\nd jst mdklnkrs:\ndrk f n rp ht wrd\n\"spghtt\"");
            add_music("Welke sport horen we hier?", "morse_high_speed_telegraphy.ogg");
            add_text("", "Hoeveel symbolen\nworden erin\nMorse code gebruikt?");
            add_music("Welk woord horen we hier?", "morse_winawinterweekend.ogg");
            add_image("Teken hetvolgende symbool", "complete_12345.png");
            add_text("Geef hetvolgende cijfer", "3.141592");

            //Wie ben ik
            {
                auto &q = add_text("Wie ben ik?");
                q.descriptions.emplace_back("Ik ben geboren in\nde middeleeuwen");
                q.descriptions.emplace_back("Ik ben katholiek");
                q.descriptions.emplace_back("Heel veel mensen\nkomen dagelijks\nnaar mij kijken");
                q.descriptions.emplace_back("Victor Hugo heeft\neen verhaaltje over\nmij geschreven");
                q.descriptions.emplace_back("Ik woon op\neen eiland");
                q.descriptions.emplace_back("Vorige lente had\nik last van een brandje");
                q.descriptions.emplace_back("Ik woon in\nParijs");
            }

            //Fast round
            add_text("Ork, ork, ork", "Soep eet je\nmet een ...?");

            //Spoiler alert
            add_text("Spoiler alert", "Wie is de opa\nvan Rey uit\nStar Wars?");
            add_text("Spoiler alert", "Wat gebeurt er\nop het einde van\nelke 5TV film?");

            //Doe-ronde
            add_text("Just do-it", "Maak het vormpje\nin LEGO na");

            add_image("Geef 5 albums", "11_albums.jpg");

            //Face swap
            add_image("Face-swap", "faceswap_00.png");
            add_image("Face-swap", "faceswap_01.png");
            add_image("Face-swap", "faceswap_02.png");
            add_image("Face-swap", "faceswap_03.png");
            add_image("Face-swap", "faceswap_04.png");
            add_image("Face-swap", "faceswap_05.png");
            add_image("Face-swap", "faceswap_06.png");
            add_image("Face-swap", "faceswap_07.png");
            add_image("Face-swap", "faceswap_08.png");

            //Poes film
            add_image("Geef de naam van de film", "poes_00.png");
            add_image("Geef de naam van de film", "poes_01.png");
            add_image("Geef de naam van de film", "poes_02.png");
            add_image("Geef de naam van de film", "poes_03.png");
            add_image("Geef de naam van de film", "poes_04.png");
            add_image("Geef de naam van de film", "poes_05.png");
            add_image("Geef de naam van de film", "poes_06.png");

            add_image("Op welke plaats staat de auto?", "parkeerplaats.jpg");

            add_image("Droedel", "droedel_ding.png");
            add_image("Droedel", "droedel_kleineren.png");

            MSS_END();
        }

        State state_ = State::Idle;
        bool change_state_(State new_state, std::string &error)
        {
            MSS_BEGIN(bool);

            if (new_state == state_)
                return true;

            MSS(!!events_, error = "Model.events_ not set");

            //Exit operations
            switch (state_)
            {
                case State::Idle:
                    break;
                case State::NewQuestion:
                    break;
                case State::Thinking:
                    break;
                case State::Answer:
                    answer_team_.clear();
                    break;
            }

            state_ = new_state;

            //Enter operations
            switch (state_)
            {
                case State::Idle:
                    events_->me_show_question(nullptr);
                    answer_team_.clear();
                    answer_order_.clear();
                    break;
                case State::NewQuestion:
                    {
                        question_ix_ = number_.value_or(question_ix_+1);
                        number_.reset();
                        answer_team_.clear();
                        answer_order_.clear();

                        auto question = get_question_(question_ix_);
                        MSS(!!question, error = std::string("there is no question ")+std::to_string(question_ix_));
                        current_question_ = *question;
                        std::cout << std::endl << "Showing question " << question_ix_ << std::endl;
                        events_->me_show_question(&current_question_.value());

                        return change_state_(State::Thinking, error);
                    }
                    break;
                case State::Thinking:
                    events_->me_play_music(true, false);
                    break;
                case State::Answer:
                    events_->me_play_music(false, false);
                    break;
                case State::Error:
                    break;
            }

            MSS_END();
        }

        std::string ctor_error_;
        std::string sm_error_;
        std::vector<Question> questions_;
        std::optional<Question> current_question_;

        int question_ix_ = -1;

        std::optional<unsigned int> number_;
        std::string answer_team_;
        std::string answer_order_;
        Score score_;

        Events *events_ = nullptr;
    };

} 

#endif
