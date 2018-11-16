#ifndef HEADER_laurot_Slave_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Slave_hpp_ALREADY_INCLUDED

#include "laurot/Types.hpp"
#include "gubg/rs485/Endpoint.hpp"
#include "gubg/mss.hpp"

namespace laurot { 

    class Slave
    {
    public:
        Slave(Id id, gubg::rs485::Endpoint::Ptr ep): id_(id), ep_(ep) {}

        bool process()
        {
            MSS_BEGIN(bool, "Slave");
            L(C(id_));
            std::byte byte;
            if (ep_->receive(byte))
            {
                L(C(byte, char));
                if (byte == std::byte{'a'+id_})
                    send_ack_();
            }
            MSS_END();
        }

    private:
        void send_ack_()
        {
            ep_->send(std::byte{0});
        }

        const Id id_;
        gubg::rs485::Endpoint::Ptr ep_;
    };

} 

#endif
