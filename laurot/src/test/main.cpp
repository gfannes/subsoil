#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/arduino/Clock.hpp>
#include <gubg/std/array.hpp>
#include <gubg/std/algorithm.hpp>

#define gubg_no_log 0
#include "gubg/log.hpp"

#include "Arduino.h"

using Clock = gubg::arduino::MillisClock;

class Button
{
public:
    struct Event
    {
        bool pressed = false;
        bool is_long = false;
    };

    const static std::uint8_t DebounceCount = 16;
    const static std::uint8_t UpIX = 0u;
    const static std::uint8_t DownIX = 1u;
    const static unsigned int LongDuration_ms = 500;

    bool get_event(Event &event) const
    {
        if (!has_event_)
            return false;
        event.pressed = is_pressed_;
        event.is_long = is_long_event_;
        return true;
    }
    bool is_pressed() const
    {
        return is_pressed_;
    }

    void process(std::uint8_t state_ix, unsigned int elapse_ms)
    {
        has_event_ = false;
        if (debounce_counts_[state_ix] < Button::DebounceCount)
        {
            //Button is still debouncing
            ++debounce_counts_[state_ix];
            debounce_counts_[1u-state_ix] = 0;
        }
        else
        {
            //Button reached stable state
            const auto new_pressed = (state_ix == Button::DownIX);
            if (new_pressed != is_pressed_)
            {
                const auto now = Clock::now();
                is_pressed_ = new_pressed;
                has_event_ = true;
                is_long_event_ = ((now-event_timepoint_) >= Button::LongDuration_ms);

                event_timepoint_ = now;
            }
        }
    }

private:
    bool has_event_ = false;
    bool is_long_event_ = false;
    Clock::TimePoint event_timepoint_ = 0;

    bool is_pressed_ = false;
    std::array<std::uint8_t, 2> debounce_counts_{};
};

class Relay
{
public:
    void setup(unsigned int short_duration_ms, unsigned int long_duration_ms)
    {
        short_duration_ms_ = short_duration_ms;
        long_duration_ms_ = long_duration_ms;
    }
    void set_peer(Relay *peer)
    {
        peer_ = peer;
    }

    bool is_active() const
    {
        return active_ms_ > 0;
    }

    void activate(bool use_short_duration)
    {
        active_ms_ = use_short_duration ? short_duration_ms_ : long_duration_ms_;
        if (peer_)
            peer_->deactivate();
    }
    void deactivate()
    {
        active_ms_ = 0;
    }

    void process(unsigned int elapse_ms)
    {
        elapse_ms = std::min(elapse_ms, active_ms_);
        active_ms_ -= elapse_ms;
    }

private:
    unsigned int short_duration_ms_ = 2000;
    unsigned int long_duration_ms_ = 10000;
    unsigned int active_ms_ = 0;
    Relay *peer_ = nullptr;
};

class IO
{
public:
    enum class Type {None, Button, Relay};

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
        return button_.is_pressed();
    }
    bool get_button_event(Button::Event &event) const
    {
        if (!is(Type::Button))
            return false;
        return button_.get_event(event);
    }

    //Relay API
    void setup_relay(std::uint8_t pin, unsigned int short_duration_ms, unsigned int long_duration_ms)
    {
        type_ = Type::Relay;
        pin_.set_pin(pin).set_output(true);
        relay_.setup(short_duration_ms, long_duration_ms);
    }
    bool relay_is_active() const
    {
        if (!is(Type::Relay))
            return false;
        return relay_.is_active();
    }
    void activate_relay(bool use_short_duration = true)
    {
        if (!is(Type::Relay))
            return;
        relay_.activate(use_short_duration);
    }
    void deactivate_relay()
    {
        if (!is(Type::Relay))
            return;
        relay_.deactivate();
    }
    void set_peer(IO *io)
    {
        if (!is(Type::Relay))
            return;
        if (io)
        {
            if (!io->is(Type::Relay))
                return;
            relay_.set_peer(&io->relay_);
        }
        else
            relay_.set_peer(nullptr);
    }

    //Common API
    bool is(Type type) const {return type_ == type;}

    void process(unsigned int elapse_ms)
    {
        switch (type_)
        {
            case Type::Button:
                {
                    pin_.check();

                    const auto state_ix = pin_.is_input(true) ? Button::UpIX : Button::DownIX;
                    button_.process(state_ix, elapse_ms);
                }
                break;
            case Type::Relay:
                {
                    pin_.set_output(!relay_is_active());

                    relay_.process(elapse_ms);
                }
                break;
            default:
                break;
        }
    }

private:
    gubg::arduino::Pin pin_;

    Type type_ = Type::None;
    Button button_;
    Relay relay_;
};


std::array<IO, 63> ios{};

gubg::arduino::Elapsed<Clock::TimePoint> elapsed_ms;

void setup()
{
    const unsigned int nr_buttons = 14;
    const unsigned int nr_relays = 14;
    for (auto i = 0u; i < nr_buttons; ++i)
    {
        const unsigned int my_ix = i;
        ios[my_ix].setup_button(26+i);
    }
    for (auto i = 0u; i < nr_relays; ++i)
    {
        const unsigned int my_ix = nr_buttons+i;
        auto &io = ios[my_ix];
        io.setup_relay(40+i, 2000, 10000);
        if (i > 0 && i%2 == 1)
        {
            auto &other_io = ios[my_ix-1];
            io.set_peer(&other_io);
            other_io.set_peer(&io);
        }
    }
}

void loop()
{
    elapsed_ms.process(Clock::now());

    for (auto ix = 0u; ix < ios.size(); ++ix)
    {
        auto &io = ios[ix];

        Button::Event button_event;
        if (io.get_button_event(button_event))
        {
            const auto relay_ix = ix+2*7;
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
                    if (button_event.is_long && relay.relay_is_active())
                        relay.activate_relay(false);
                }
            }
        }

        io.process(elapsed_ms());
    }
}

