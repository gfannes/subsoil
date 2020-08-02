#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/std/array.hpp>
#include <gubg/std/algorithm.hpp>

#define gubg_no_log 0
#include "gubg/log.hpp"

#include "Arduino.h"

enum class Type {None, Button, Relay};

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
    bool get_button_event(bool &pressed) const
    {
        if (!is(Type::Button))
            return false;
        if (!button_.has_event)
            return false;
        pressed = button_.is_pressed;
        return true;
    }

    //Relay API
    void setup_relay(std::uint8_t pin, unsigned int ms)
    {
        type_ = Type::Relay;
        pin_.set_pin(pin).set_output(true);
        relay_.duration_ms = ms;
    }
    bool relay_is_active() const
    {
        if (!is(Type::Relay))
            return;
        return relay_.active_ms > 0;
    }
    void activate_relay()
    {
        if (!is(Type::Relay))
            return;
        relay_.active_ms = relay_.duration_ms;
    }
    void deactivate_relay()
    {
        if (!is(Type::Relay))
            return;
        relay_.active_ms = 0;
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
                            button_.is_pressed = new_pressed;
                            button_.has_event = true;
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

        bool has_event = false;
        bool is_pressed = false;
        std::array<std::uint8_t, 2> debounce_counts{};
    };
    Button button_;

    struct Relay
    {
        unsigned int duration_ms = 2000;
        unsigned int active_ms = 0;
    };
    Relay relay_;
};


std::array<IO, 63> ios{};

gubg::arduino::Elapsed<unsigned int> elapsed_ms;

void setup()
{
    for (auto ix = 0u; ix < 7; ++ix)
        ios[ix].setup_button(A0+ix);
    for (auto ix = 7u; ix < 14; ++ix)
        ios[ix].setup_relay(A0+ix, 2000);
}

void loop()
{
    elapsed_ms.process(millis());

    for (auto ix = 0u; ix < ios.size(); ++ix)
    {
        auto &io = ios[ix];

        bool pressed;
        if (io.get_button_event(pressed))
        {
            const auto relay_ix = ix+7;
            if (relay_ix < ios.size())
            {
                auto &relay = ios[relay_ix];
                if (pressed)
                {
                    if (relay.relay_is_active())
                        relay.deactivate_relay();
                    else
                        relay.activate_relay();
                }
            }
        }

        io.process(elapsed_ms());
    }
}

