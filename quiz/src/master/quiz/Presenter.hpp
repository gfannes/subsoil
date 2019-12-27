#ifndef HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Presenter_hpp_ALREADY_INCLUDED

#include <quiz/Model.hpp>
#include <quiz/View.hpp>

namespace quiz { 

    class Presenter: public View::Events
    {
    public:
        Presenter(Model &model, View &view): model_(model), view_(view) {}

        bool quit() const {return quit_;}
        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS(!quit());
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

    private:
        bool quit_ = false;
        Model &model_;
        View &view_;
    };

} 

#endif
