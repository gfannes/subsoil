#ifndef HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED

#include <quiz/Model.hpp>
#include <quiz/View.hpp>

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

            if (false)
            {
                const unsigned int count = 100;
                if (tick_%count == 0)
                {
                    const auto tt = tick_/count;
                    view_.answer_was_correct(tt%2 == 0);
                }
            }
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

    private:
        bool quit_ = false;
        Model &model_;
        View &view_;

        unsigned int tick_ = 0;
    };

} 

#endif
