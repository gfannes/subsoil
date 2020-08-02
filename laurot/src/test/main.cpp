#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/arduino/Clock.hpp>
#include <gubg/std/array.hpp>
#include <gubg/std/algorithm.hpp>

#define gubg_no_log 0
#include "gubg/log.hpp"

#include "Arduino.h"

using Clock = gubg::arduino::MillisClock;

enum class Type {None, Button, Relay};

struct ButtonEvent
{
    bool pressed = false;
    bool is_long = false;
};

class IO
{
public:
    //Button API
    void setup_button(std::uint8_t pin)
    {
        type_ = Type::Button;
        pin_.set_pin(pin).set_input(true);
    }
    bool button_is_pressed() const
    {
        if (!is(Type::Button))
            return false;
        return button_.is_pressed;
    }
    bool get_button_event(ButtonEvent &event) const
    {
        if (!is(Type::Button))
            return false;
        if (!button_.has_event)
            return false;
        event.pressed = button_.is_pressed;
        event.is_long = button_.is_long_event;
        return true;
    }

    //Relay API
    void setup_relay(std::uint8_t pin, unsigned int short_duration_ms, unsigned int long_duration_ms)
    {
        type_ = Type::Relay;
        pin_.set_pin(pin).set_output(true);
        relay_.short_duration_ms = short_duration_ms;
        relay_.long_duration_ms = long_duration_ms;
    }
    bool relay_is_active() const
    {
        if (!is(Type::Relay))
            return false;
        return relay_.active_ms > 0;
    }
    void activate_relay(bool use_short_duration = true)
    {
        if (!is(Type::Relay))
            return;
        relay_.active_ms = use_short_duration ? relay_.short_duration_ms : relay_.long_duration_ms;
        if (exclusive_io_)
            exclusive_io_->deactivate_relay();
    }
    void deactivate_relay()
    {
        if (!is(Type::Relay))
            return;
        relay_.active_ms = 0;
    }
    void set_exclusive(IO *io)
    {
        exclusive_io_ = io;
    }

    //Common API
    bool is(Type type) const {return type_ == type;}

    void process(unsigned int elapse_ms)
    {
        switch (type_)
        {
            case Type::Button:
                {
                    button_.has_event = false;

                    pin_.check();
                    const auto state_ix = pin_.is_input(true) ? Button::UpIX : Button::DownIX;
                    if (button_.debounce_counts[state_ix] < Button::DebounceCount)
                    {
                        //Button is still debouncing
                        ++button_.debounce_counts[state_ix];
                        button_.debounce_counts[1u-state_ix] = 0;
                    }
                    else
                    {
                        //Button reached stable state
                        const auto new_pressed = (state_ix == Button::DownIX);
                        if (new_pressed != button_.is_pressed)
                        {
                            const auto now = Clock::now();
                            button_.is_pressed = new_pressed;
                            button_.has_event = true;
                            button_.is_long_event = ((now-button_.event_timepoint) >= Button::LongDuration_ms);

                            button_.event_timepoint = now;
                        }
                    }
                }
                break;
            case Type::Relay:
                {
                    pin_.set_output(!relay_is_active());

                    elapse_ms = std::min(elapse_ms, relay_.active_ms);
                    relay_.active_ms -= elapse_ms;
                }
                break;
            default:
                break;
        }
    }

private:
    gubg::arduino::Pin pin_;

    Type type_ = Type::None;

    struct Button
    {
        const static std::uint8_t DebounceCount = 16;
        const static std::uint8_t UpIX = 0u;
        const static std::uint8_t DownIX = 1u;
        const static unsigned int LongDuration_ms = 500;

        bool has_event = false;
        bool is_long_event = false;
        Clock::TimePoint event_timepoint = 0;

        bool is_pressed = false;
        std::array<std::uint8_t, 2> debounce_counts{};
    };
    Button button_;

    struct Relay
    {
        unsigned int short_duration_ms = 2000;
        unsigned int long_duration_ms = 10000;
        unsigned int active_ms = 0;
    };
    Relay relay_;

    IO *exclusive_io_ = nullptr;
};


std::array<IO, 63> ios{};

gubg::arduino::Elapsed<Clock::TimePoint> elapsed_ms;

void setup()
{
    for (auto ix = 0u; ix < 7; ++ix)
        ios[ix].setup_button(A0+ix);
    for (auto ix = 7u; ix < 14; ++ix)
    {
        auto &io = ios[ix];
        io.setup_relay(A0+ix, 2000, 10000);
        if (ix > 0 && ix%2 == 0)
        {
            auto &other_io = ios[ix-1];
            io.set_exclusive(&other_io);
            other_io.set_exclusive(&io);
        }
    }
}

void loop()
{
    elapsed_ms.process(Clock::now());

    for (auto ix = 0u; ix < ios.size(); ++ix)
    {
        auto &io = ios[ix];

        ButtonEvent button_event;
        if (io.get_button_event(button_event))
        {
            const auto relay_ix = ix+7;
            if (relay_ix < ios.size())
            {
                auto &relay = ios[relay_ix];
                if (button_event.pressed)
                {
                    if (relay.relay_is_active())
                        relay.deactivate_relay();
                    else
                        relay.activate_relay(true);
                }
                else
                {
                    /* if (button_event.is_long && relay.relay_is_active()) */
                    if (button_event.is_long)
                        relay.activate_relay(false);
                }
            }
        }

        io.process(elapsed_ms());
    }
}

