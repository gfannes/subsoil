#ifndef HEADER_app_Inputs_hpp_ALREADY_INCLUDED
#define HEADER_app_Inputs_hpp_ALREADY_INCLUDED

#include "Arduino.h"
#include "gubg/std/array.hpp"

namespace app { 

    struct InputInfo
    {
        size_t ix;
        bool up;
        unsigned long elapse_ms;
    };

    template <size_t Offset, size_t Size>
    class Inputs
    {
    public:
        using Millis = decltype(millis());

        void setup()
        {
            const auto now = millis();
            auto setup_pin = [&](size_t pin, Data &data)
            {
                pinMode(pin, INPUT_PULLUP);
                data.state = State::ToUp;
                data.timepoint_ms = now;
            };
            each_pin_(setup_pin);
            prev_dirty_ = Size;
        }

        void process()
        {
            auto lamdba = [&](size_t pin, Data &data)
            {
                if (digitalRead(pin) == HIGH)
                {
                    //Pullup: button is up
                    if (data.state != State::ToUp && data.state != State::Up)
                    {
                        data.state = State::ToUp;
                        data.timepoint_ms = millis();
                    }
                }
                else
                {
                    //Ground: button is down
                    if (data.state != State::ToDown && data.state != State::Down)
                    {
                        data.state = State::ToDown;
                        data.timepoint_ms = millis();
                    }
                }
            };
            each_pin_(lamdba);
        }

        bool pop_dirty(InputInfo &info)
        {
            auto lambda = [&](size_t ix)
            {
                auto &data = data_[ix];
                switch (data.state)
                {
                    case State::ToUp:
                        info.up = true;
                        data.state = State::Up;
                        break;
                    case State::ToDown:
                        info.up = false;
                        data.state = State::Down;
                        break;
                    default:
                        return false;
                        break;
                }
                info.ix = ix;
                info.elapse_ms = millis()-data.timepoint_ms;
                prev_dirty_ = ix;
                return true;
            };
            for (auto i = prev_dirty_+1; i < Size; ++i)
                if (lambda(i))
                    return true;
            for (auto i = 0; i <= prev_dirty_; ++i)
                if (lambda(i))
                    return true;
            return false;
        }

    private:
        template <typename Ftor>
        void each_pin_(Ftor &&ftor)
        {
            for (auto ix = 0; ix < Size; ++ix)
                ftor(ix+Offset, data_[ix]);
        }

        enum class State: std::uint8_t
        {
            ToUp, ToDown, Up, Down,
        };

        struct Data
        {
            State state;
            Millis timepoint_ms;
        };
        std::array<Data, Size> data_;
        size_t prev_dirty_;
    };

} 

#endif
