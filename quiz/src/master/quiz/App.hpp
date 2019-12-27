#ifndef HEADER_quiz_App_hpp_ALREADY_INCLUDED
#define HEADER_quiz_App_hpp_ALREADY_INCLUDED

#include <gubg/mss.hpp>
#include <quiz/Presenter.hpp>

namespace quiz { 

    class App
    {
    public:
        bool operator()(std::string &error)
        {
            if (presenter_.quit())
                return false;
            const auto ok = process_(error);
            if (!ok && error.empty())
                error = "unknown error occured";
            return ok;
        }

    private:
        bool process_(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS(presenter_(error));
            MSS_END();
        }
        Model model_;
        View view_{800, 600};
        Presenter presenter_{model_, view_};
    };

} 

#endif
