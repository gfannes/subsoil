#ifndef HEADER_laurot_Master_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Master_hpp_ALREADY_INCLUDED

#include "gubg/rs485/Endpoint.hpp"
#include "gubg/mss.hpp"
#include <ostream>

namespace laurot { 

    enum class PollState {Idle, WaitForAnswer, Timeout};
    inline std::ostream &operator<<(std::ostream &os, PollState state)
    {
        switch (state)
        {
#define l_case(name) case PollState::name: os << #name; break
            l_case(Idle);
            l_case(WaitForAnswer);
            l_case(Timeout);
            default: os << "Unknown state"; break;
        }
        return os;
    }

    class Master
    {
    public:
        Master(gubg::rs485::Endpoint::Ptr ep): ep_(ep)
        {
            for (auto i = 0u; i < slaves_.size(); ++i)
                slaves_[i].id = i;
        }

        bool process()
        {
            MSS_BEGIN(bool, "Master");

            MSS(process_tick_());

            for (std::byte byte; ep_->receive(byte);)
                MSS(process_(byte));

            for (const auto &slave: slaves_)
            {
                std::cout << C(slave.id)C(slave.is_online) << std::endl;
            }

            MSS_END();
        }

    private:
        struct SlaveInfo
        {
            Id id = -1;
            bool is_online = false;
            PollState poll_state = PollState::Idle;
            unsigned int ticks = 0;
        };

        bool process_tick_()
        {
            MSS_BEGIN(bool, "");
            auto &slave = slaves_[slave_ix_];
            ++slave.ticks;
            L(C(slave.id)C(slave.ticks));
            switch (slave.poll_state)
            {
                case PollState::Idle:
                    ep_->send(std::byte{'a'+slave_ix_});
                    change_state_(PollState::WaitForAnswer);
                    break;
                case PollState::WaitForAnswer:
                    if (slave.ticks > 3)
                        change_state_(PollState::Timeout);
                    break;
            }
            MSS_END();
        }
        bool process_(std::byte byte)
        {
            MSS_BEGIN(bool, "");
            auto &slave = slaves_[slave_ix_];
            ++slave.ticks;
            switch (slave.poll_state)
            {
                case PollState::WaitForAnswer:
                    slave.is_online = true;
                    change_state_(PollState::Idle);
                    break;
            }
            MSS_END();
        }

        void change_state_(PollState new_state)
        {
            auto &slave = slaves_[slave_ix_];
            if (slave.poll_state == new_state)
                return;

            //Exit
            switch (slave.poll_state)
            {
            }

            std::cout << "Changing state from " << slave.poll_state << " to " << new_state << std::endl;
            slave.poll_state = new_state;

            //Enter
            switch (slave.poll_state)
            {
                case PollState::Idle:
                    ++slave_ix_;
                    slave_ix_ %= slaves_.size();
                    break;
                case PollState::Timeout:
                    std::cout << "TIMEOUT" << std::endl;
                    slave.is_online = false;
                    change_state_(PollState::Idle);
                    break;
            }
        }

        gubg::rs485::Endpoint::Ptr ep_;

        std::array<SlaveInfo, 4> slaves_;
        size_t slave_ix_ = 0;
    };

} 

#endif
