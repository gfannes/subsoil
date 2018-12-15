#ifndef HEADER_laurot_CallCenter_hpp_ALREADY_INCLUDED
#define HEADER_laurot_CallCenter_hpp_ALREADY_INCLUDED

#include "app/Options.hpp"
#include "app/Queue.hpp"
#include "laurot/Id.hpp"
#include "gubg/serial/Endpoint.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/t2/Segmenter.hpp"
#include "gubg/string/Std.hpp"
#include "gubg/mss.hpp"
#include <chrono>
#include <array>

namespace app { 

    class CallCenter
    {
    public:
        bool setup(const Options &options, Queue &in, Queue &out)
        {
            MSS_BEGIN(bool);
            gubg::serial::Settings settings;
            settings.baud_rate = options.baud_rate;
            MSS(rs485_ep_.open(options.tty.c_str(), settings));
            in_ = &in;
            out_ = &out;
            MSS_END();
        }

        bool process()
        {
            MSS_BEGIN(bool, "");
            L(C(state_, int));
            switch (state_)
            {
                case State::Idle:
                    MSS(prepare_out_msg_());
                    break;
                case State::SendingQuestion:
                    MSS(rs485_ep_.send(out_offset_, out_msg_str_.data(), out_msg_str_.size()));
                    if (out_offset_ == out_msg_str_.size())
                        change_state_(State::WaitForAnswer);
                    break;
                case State::WaitForAnswer:
                    if (Clock::now() > timeout_)
                        change_state_(State::Timeout);
                    std::array<gubg::t2::Byte, 1024> buffer;
                    size_t offset = 0;
                    MSS(rs485_ep_.receive(offset, buffer.data(), buffer.size()));
                    L(C(offset));
                    auto lamdba = [&](gubg::t2::Byte *begin, gubg::t2::Byte *end)
                    {
                        L("Received t2 message of size " << end-begin);
                        change_state_(State::Idle);
                        return true;
                    };
                    for (auto i = 0u; i < offset; ++i)
                        segmenter_.process(buffer[i], lamdba);
                    break;
            }
            MSS_END();
        }

    private:
        using Clock = std::chrono::steady_clock;

        bool prepare_out_msg_()
        {
            MSS_BEGIN(bool, "");

            if (out_->empty())
                //Nothing to send
                MSS_RETURN_OK();

            const auto msg = out_->front();
            out_->pop();

            out_msg_str_.resize(0);
            T2Doc doc{out_msg_};

            message_id_ = (message_id_+1)%64;

            if (false) {}
            else if (msg.poll)
            {
                const auto &poll = *msg.poll;
                auto question = doc.tag(laurot::id::Question);
                question.attr(laurot::id::To, poll.to);
                question.attr(laurot::id::Id, message_id_);
            }

            L(out_msg_str_.size());

            change_state_(State::SendingQuestion);

            MSS_END();
        }

        enum class State {Idle, SendingQuestion, WaitForAnswer, Timeout};
        State state_ = State::Idle;
        void change_state_(State new_state)
        {
            if (state_ == new_state)
                return;

            //Exit
            switch (state_)
            {
            }

            state_ = new_state;

            //Enter
            switch (state_)
            {
                case State::SendingQuestion:
                    out_offset_ = 0;
                    break;
                case State::WaitForAnswer:
                    timeout_ = Clock::now()+std::chrono::milliseconds(1000);
                    break;
                case State::Timeout:
                    std::cout << "Timeout deteced" << std::endl;
                    change_state_(State::Idle);
                    break;
            }
        }

        gubg::serial::Endpoint rs485_ep_;
        Queue *in_;
        Queue *out_;

        using String = gubg::string::Std;
        using T2Doc = gubg::t2::Document<String>;
        std::string out_msg_str_;
        String out_msg_{out_msg_str_};
        size_t out_offset_;

        unsigned int message_id_ = 0;

        Clock::time_point timeout_;

        std::array<gubg::t2::Byte, 1024> t2_buffer_;
        gubg::t2::Segmenter segmenter_{t2_buffer_.data(), t2_buffer_.data()+t2_buffer_.size()};
    };

} 

#endif
