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
            virtual void me_play_music(bool b) = 0;
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
            events_->me_show_score(&score_);

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
                if (answer_order_.find(ch) != std::string::npos)
                    return true;
                if (!answer_team_.empty() && answer_team_[0] == ch)
                    return true;
                answer_order_.push_back(ch);
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
            else if (ch == ' ')
                MSS(change_state_(State::Idle, sm_error_));

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

            {
                Question q;
                q.description = "How many bitches can you fit in a Tesla?";
                q.image_fn = "quiz/media/question/mrbean.jpg";
                q.music_fn = "quiz/media/question/music.wav";
                q.pitch = 2.0f;
                questions_.push_back(q);
            }

            MSS_END();
        }

        State state_ = State::Idle;
        bool change_state_(State new_state, std::string &error)
        {
            MSS_BEGIN(bool);

            if (new_state == state_)
                return true;

            MSS(!!events_, error = "Model.events_ not set");

            std::cout << "Changing state from " << state_ << " to " << new_state << std::endl;

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
                        events_->me_show_question(question);

                        return change_state_(State::Thinking, error);
                    }
                    break;
                case State::Thinking:
                    events_->me_play_music(true);
                    break;
                case State::Answer:
                    events_->me_play_music(false);
                    break;
                case State::Error:
                    break;
            }

            MSS_END();
        }

        std::string ctor_error_;
        std::string sm_error_;
        std::vector<Question> questions_;

        int question_ix_ = -1;

        std::optional<unsigned int> number_;
        std::string answer_team_;
        std::string answer_order_;
        Score score_;

        Events *events_ = nullptr;
    };

} 

#endif
