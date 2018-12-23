#ifndef HEADER_app_Queue_hpp_ALREADY_INCLUDED
#define HEADER_app_Queue_hpp_ALREADY_INCLUDED

#include "gubg/Signal.hpp"
#include <queue>
#include <optional>
#include <string>

namespace app { 

    struct Message
    {
        enum Stage
        {
            Serialized,
        };

        struct Poll
        {
            unsigned int to;
            Poll(unsigned int to): to(to) {}
        };
        std::optional<Poll> poll;

        mutable gubg::Signal<Stage> event;
    };

    using Queue = std::queue<Message>;

} 

#endif
