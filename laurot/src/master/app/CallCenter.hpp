#ifndef HEADER_laurot_CallCenter_hpp_ALREADY_INCLUDED
#define HEADER_laurot_CallCenter_hpp_ALREADY_INCLUDED

#include "app/Options.hpp"
#include "app/Queue.hpp"
#include "laurot/Id.hpp"
#include "gubg/serial/Endpoint.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/t2/Segmenter.hpp"
#include "gubg/t2/Range.hpp"
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
            MSS_BEGIN(bool);
            L(C(state_, int));
            switch (state_)
            {
                case State::Idle:
                    {
                        MSS(change_state_(State::SendQuestion));
                    }
                    break;
                case State::SendQuestion:
                    {
                        MSS(rs485_ep_.send(out_offset_, out_msg_str_.data(), out_msg_str_.size()));
                        if (out_offset_ == out_msg_str_.size())
                            //Message is sent
                            return change_state_(State::WaitForAnswer);
                    }
                    break;
                case State::WaitForAnswer:
                    {
                        if (Clock::now() > timeout_)
                            return error_("Timeout");
                        std::array<gubg::t2::Byte, 1024> buffer;
                        size_t offset = 0;
                        MSS(rs485_ep_.receive(offset, buffer.data(), buffer.size()));
                        L(C(offset));
                        auto lamdba = [&](gubg::t2::Byte *begin, gubg::t2::Byte *end)
                        {
                            MSS_BEGIN(bool, "");
                            L("Received t2 message of size " << end-begin);
                            gubg::t2::Range range{begin, end};
                            MSS(range.pop_tag(laurot::id::Answer), error_("Expected an answer"));
                            gubg::t2::Data key, value;
                            while (range.pop_attr(key, value))
                            {
                                switch (key)
                                {
                                    case laurot::id::Id:
                                        MSS(value == message_id_, error_("Message id mismatch"));
                                        break;
                                }
                            }
                            change_state_(State::SendUnderstood);
                            offset = 0;
                            MSS_END();
                        };
                        for (auto i = 0u; i < offset; ++i)
                            segmenter_.process(buffer[i], lamdba);
                    }
                    break;
                case State::SendUnderstood:
                    {
                        MSS(rs485_ep_.send(out_offset_, out_msg_str_.data(), out_msg_str_.size()));
                        if (out_offset_ == out_msg_str_.size())
                            //Message is sent
                            return change_state_(State::Idle);
                    }
                    break;
            }
            MSS_END();
        }

    private:
        using Clock = std::chrono::steady_clock;

        const char * error_msg_;
        bool error_(const char *msg)
        {
            error_msg_ = msg;
            return change_state_(State::Error);
        }

        bool prepare_question_()
        {
            MSS_BEGIN(bool, "");

            if (out_->empty())
            {
                L("Nothing to send");
                MSS_RETURN_OK();
            }

            const auto msg = out_->front();
            out_->pop();
            msg.event.emit(Message::Popped);

            message_id_ = (message_id_+1)%64;

            out_msg_str_.resize(0);
            T2Doc doc{out_msg_};

            if (false) {}
            else if (msg.poll)
            {
                const auto &poll = *msg.poll;
                auto question = doc.tag(laurot::id::Question);
                to_ = poll.to;
                L(C(to_));
                question.attr(laurot::id::To, to_);
                question.attr(laurot::id::Id, message_id_);
            }

            L(out_msg_str_.size());

            MSS_END();
        }
        bool prepare_understood_()
        {
            MSS_BEGIN(bool);

            out_msg_str_.resize(0);
            T2Doc doc{out_msg_};

            {
                auto understood = doc.tag(laurot::id::Understood);
                understood.attr(laurot::id::To, to_);
                understood.attr(laurot::id::Id, message_id_);
            }

            L(out_msg_str_.size());

            MSS_END();
        }

        enum class State {Idle, SendQuestion, WaitForAnswer, SendUnderstood, Error,};
        State state_ = State::Idle;
        bool change_state_(State new_state)
        {
            MSS_BEGIN(bool);

            if (state_ == new_state)
                MSS_RETURN_OK();

            //Exit
            switch (state_)
            {
            }

            state_ = new_state;

            //Enter
            switch (state_)
            {
                case State::Idle:
                    rs485_ep_.flush_receive();
                    break;
                case State::SendQuestion:
                    MSS(prepare_question_(), error_("Failed to prepare question"));
                    out_offset_ = 0;
                    break;
                case State::WaitForAnswer:
                    timeout_ = Clock::now()+std::chrono::milliseconds(100);
                    break;
                case State::SendUnderstood:
                    MSS(prepare_understood_(), error_("Failed to prepare understood"));
                    out_offset_ = 0;
                    break;
                case State::Error:
                    std::cout << "Error: " << error_msg_ << std::endl;
                    change_state_(State::Idle);
                    break;
            }
            MSS_END();
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
        unsigned int to_;

        Clock::time_point timeout_;

        std::array<gubg::t2::Byte, 1024> t2_buffer_;
        gubg::t2::Segmenter segmenter_{t2_buffer_.data(), t2_buffer_.data()+t2_buffer_.size()};
    };

} 

#endif
