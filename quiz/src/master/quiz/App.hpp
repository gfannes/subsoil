#ifndef HEADER_quiz_App_hpp_ALREADY_INCLUDED
#define HEADER_quiz_App_hpp_ALREADY_INCLUDED

#include <gubg/mss.hpp>
#include <quiz/Options.hpp>
#include <quiz/Presenter.hpp>
#include <thread>
#include <chrono>

namespace quiz { 

    class App
    {
    public:
        bool process(const Options &options, std::string &error)
        {
            MSS_BEGIN(bool);
            if (options.buttons_device)
            {
                gubg::serial::Settings serial_settings;
                serial_settings.baud_rate = options.baudrate.value_or(9600);
                MSS(view_.connect_to_buttons(*options.buttons_device, serial_settings), error = "could not connect to buttons");
            }
            MSS_END();
        }

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
            std::this_thread::sleep_for(std::chrono::milliseconds{10});
            MSS_END();
        }
        Model model_;
        View view_{800, 600};
        /* View view_{1800, 1200}; */
        Presenter presenter_{model_, view_};
    };

} 

#endif
