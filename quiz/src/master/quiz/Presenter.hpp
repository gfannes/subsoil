#ifndef HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED

#include <quiz/Model.hpp>
#include <quiz/View.hpp>
#include <gubg/Range.hpp>

namespace quiz { 

    class Presenter: public View::Events
    {
    public:
        Presenter(Model &model, View &view): model_(model), view_(view)
        {
            view_.inject_events_receiver(this);
        }

        bool quit() const {return quit_;}

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(!quit());

            MSS(model_(error));
            MSS(view_(error));

            if (start_question_)
            {
                start_question_ = false;

                qix_ = number_.value_or(qix_+1);
                number_.reset();
                answer_order_.clear();

                auto question = model_.get_question(qix_);
                std::ostringstream oss;
                if (question)
                    oss << question->description;
                else
                    oss << "Error: there is no question " << qix_ << ".";
                view_.set_description(oss.str());
            }
            view_.set_answer_order(answer_order_);

            if (false)
            {
                const unsigned int count = 100;
                if (tick_%count == 0)
                {
                    const auto tt = tick_/count;
                    view_.answer_was_correct(tt%2 == 0);
                }
            }
            if (false)
                view_.set_description("How many bitches can you fit in a Tesla?\n(model 3)");

            ++tick_;

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

            if ('0' <= ch && ch <= '9')
            {
                number_ = number_.value_or(0)*10 + (ch-'0');
            }
            else if ('a' <= ch && ch <= 'e')
            {
                if (answer_order_.find(ch) == std::string::npos)
                    answer_order_.push_back(ch);
            }
            else if (ch == 'q')
            {
                start_question_ = true;
            }
            else if (ch == 'x')
            {
            }
            else if (ch == 'v')
            {
            }
        }

    private:
        bool quit_ = false;
        Model &model_;
        View &view_;

        int qix_ = -1;

        std::optional<unsigned int> number_;
        bool start_question_ = false;
        unsigned int tick_ = 0;
        std::string answer_order_;
    };

} 

#endif
