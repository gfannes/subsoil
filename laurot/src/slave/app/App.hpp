#ifndef HEADER_app_App_hpp_ALREADY_INCLUDED
#define HEADER_app_App_hpp_ALREADY_INCLUDED

#include "app/message/Parser.hpp"
#include "app/message/Builder.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/Blinker.hpp"
#include "gubg/arduino/rs485/Endpoint.hpp"
#include "gubg/std/array.hpp"

namespace app { 

    class App
    {
    public:
        void setup()
        {
            ep_.init(Serial1, 8, 9600, SERIAL_8N1);

            blinker_.set_mode(Mode::SlowDark);
        }

        void process(unsigned long elapsed_us)
        {
            ep_.process();

            blinker_.process(elapsed_us);

            online_timer_.process(elapsed_us, [&](){
                    blinker_.set_mode(Mode::SlowDark);
                    });

            switch (state_)
            {
                case State::Init:
                    change_state_(State::Idle);
                    break;
                case State::Idle:
                    change_state_(State::Receiving);
                    break;
                case State::Receiving:
                    {
                        buffer_offset_ = 0;
                        ep_.receive(buffer_offset_, buffer_.data(), BufferSize);

                        for (auto i = 0u; i < buffer_offset_; ++i)
                        {
                            TAG("ch")ATTR(i)
                            parser_.process(buffer_[i]);
                        }

                        if (parser_.ready())
                        {
                            switch (parser_.tag1)
                            {
                                case laurot::id::Question:
                                    {
                                        TAG("received a question")
                                        app::message::create_answer(buffer_, buffer_size_, parser_.message_id);
                                        buffer_offset_ = 0;
                                        ATTR(buffer_size_)
                                        change_state_(State::Sending);
                                    }
                                    break;
                                case laurot::id::Understood:
                                    {
                                        TAG("received an understood")
                                        blinker_.set_mode(Mode::Bright);
                                        online_timer_.add(1000000u);
                                        change_state_(State::Idle);
                                    }
                                    break;
                                default:
                                    {
                                        TAG("UNKNOWN TAG RECEIVED")ATTR(parser_.tag1)
                                        change_state_(State::Error);
                                    }
                                    break;
                            }
                        }
                    }
                    break;

                case State::Sending:
                    {
                        ep_.send(buffer_offset_, buffer_.data(), buffer_size_);
                        if (!ep_.is_sending())
                            change_state_(State::Idle);
                    }
                    break;

                case State::Error:
                    change_state_(State::Idle);
                    break;
            }
        }

    private:
        using Micros = decltype(micros());

        enum class State {Init, Idle, Receiving, Sending, Error};
        State state_ = State::Init;
        void change_state_(State new_state)
        {
            if (state_ == new_state)
                return;

            //Exit
            switch (state_)
            {
                case State::Receiving:
                    parser_.reset();
                    break;
            }

            state_ = new_state;

            //Enter
            switch (state_)
            {
                case State::Error:
                    blinker_.set_mode(Mode::FastEven);
                    break;
            }
        }

        gubg::arduino::rs485::Endpoint ep_;
        message::Parser parser_;

        bool is_online_ = false;
        gubg::arduino::Timer<Micros> online_timer_;

        using Mode = gubg::arduino::Blinker::Mode;
        gubg::arduino::Blinker blinker_{13};

        static const auto BufferSize = 16;
        std::array<char, BufferSize> buffer_;
        size_t buffer_offset_ = 0;
        size_t buffer_size_ = 0;
    };


} 

#endif
