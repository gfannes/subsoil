#ifndef HEADER_app_Queue_hpp_ALREADY_INCLUDED
#define HEADER_app_Queue_hpp_ALREADY_INCLUDED

#include <queue>
#include <optional>

namespace app { 

    struct Message
    {
        struct Poll
        {
            unsigned int to;
            Poll(unsigned int to): to(to) {}
        };
        std::optional<Poll> poll;
    };

    using Queue = std::queue<Message>;

} 

#endif
