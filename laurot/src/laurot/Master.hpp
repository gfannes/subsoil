#ifndef HEADER_laurot_Master_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Master_hpp_ALREADY_INCLUDED

#include "gubg/rs485/Endpoint.hpp"
#include "gubg/mss.hpp"
#include <ostream>

namespace laurot { 

    namespace master { 
        enum class State {Idle, WaitForAnswer, Timeout};

        inline std::ostream &operator<<(std::ostream &os, State state)
        {
            switch (state)
            {
#define l_case(name) case State::name: os << #name; break
                l_case(Idle);
                l_case(WaitForAnswer);
                l_case(Timeout);
                default: os << "Unknown state"; break;
            }
            return os;
        }

        struct SlaveInfo
        {
            Id id = -1;
            bool is_online = false;
            State state = State::Idle;
            unsigned int ticks = 0;
        };
    } 

    class Master
    {
    public:
        using State = master::State;

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

            MSS_END();
        }

    private:
        void send_poll_message_(Id id)
        {
            ep_->send(std::byte{'a'+id});
        }
        //State machine
        bool process_tick_()
        {
            MSS_BEGIN(bool, "");
            auto &slave = slaves_[slave_ix_];
            ++slave.ticks;
            L(C(slave.id)C(slave.ticks));
            switch (slave.state)
            {
                case State::Idle:
                    send_poll_message_(slave.id);
                    change_state_(State::WaitForAnswer);
                    break;
                case State::WaitForAnswer:
                    if (slave.ticks > timeout_ticks_)
                        change_state_(State::Timeout);
                    break;
            }
            MSS_END();
        }
        bool process_(std::byte byte)
        {
            MSS_BEGIN(bool, "");
            auto &slave = slaves_[slave_ix_];
            ++slave.ticks;
            switch (slave.state)
            {
                case State::WaitForAnswer:
                    slave.is_online = true;
                    change_state_(State::Idle);
                    break;
            }
            MSS_END();
        }
        void change_state_(State new_state)
        {
            auto &slave = slaves_[slave_ix_];
            if (slave.state == new_state)
                return;

            //Exit
            switch (slave.state)
            {
            }

            std::cout << "Changing state from " << slave.state << " to " << new_state << std::endl;
            slave.state = new_state;

            //Enter
            switch (slave.state)
            {
                case State::Idle:
                    //Switch to the next slave
                    ++slave_ix_;
                    slave_ix_ %= slaves_.size();
                    break;
                case State::Timeout:
                    std::cout << "TIMEOUT" << std::endl;
                    slave.is_online = false;
                    change_state_(State::Idle);
                    break;
            }
        }

        const unsigned int timeout_ticks_ = 3;

        gubg::rs485::Endpoint::Ptr ep_;

        std::array<master::SlaveInfo, 4> slaves_;
        size_t slave_ix_ = 0;
    };

} 

#endif
