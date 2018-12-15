#ifndef HEADER_laurot_App_hpp_ALREADY_INCLUDED
#define HEADER_laurot_App_hpp_ALREADY_INCLUDED

#include "app/Options.hpp"
#include "app/Queue.hpp"
#include "app/CallCenter.hpp"
#include "gubg/mss.hpp"
#include <vector>
#include <thread>

namespace app { 

    class App
    {
    public:
        bool process(int argc, const char **argv)
        {
            MSS_BEGIN(bool);
            MSS(options_.parse(argc, argv));
            std::cout << options_ << std::endl;
            MSS_END();
        }

        bool run()
        {
            MSS_BEGIN(bool, "");

            if (options_.print_help)
            {
                std::cout << options_.help();
            }
            else
            {
                MSS(setup_());

                Message msg;
                msg.poll.emplace(0);
                out_queue_.push(msg);

                MSS(call_center_.process());
                MSS(call_center_.process());
                for (auto i = 0u; i < 20; ++i)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    MSS(call_center_.process());
                }
            }

            MSS_END();
        }

    private:
        bool setup_()
        {
            MSS_BEGIN(bool);
            MSS(call_center_.setup(options_, in_queue_, out_queue_));
            MSS_END();
        }

        Options options_;

        Queue in_queue_;
        Queue out_queue_;
        CallCenter call_center_;
    };

} 

#endif
