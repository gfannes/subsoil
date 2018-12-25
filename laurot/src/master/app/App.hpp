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

                MSS(call_center_.process());
                MSS(call_center_.process());
                for (; true; )
                /* for (auto i = 0u; i < 3000; ++i) */
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

            poll_messages_.resize(options_.nr_slaves);
            for (auto ix = 0u; ix < poll_messages_.size(); ++ix)
            {
                auto &msg = poll_messages_[ix];
                msg.poll.emplace(ix);
                auto add_again = [=](auto stage)
                {
                    std::cout << "Message stage: " << (int)stage << std::endl;
                    out_queue_.push(poll_messages_[ix]);
                };
                msg.event.connect([](auto stage){});
                out_queue_.push(msg);
            }

            MSS_END();
        }

        Options options_;

        std::vector<Message> poll_messages_;

        Queue in_queue_;
        Queue out_queue_;
        CallCenter call_center_;
    };

} 

#endif
