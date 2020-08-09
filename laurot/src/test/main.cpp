#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/arduino/Clock.hpp>
#include <gubg/arduino/Timer.hpp>
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
    };

    const static std::uint8_t DebounceCount = 16;
    const static std::uint8_t UpIX = 0u;
    const static std::uint8_t DownIX = 1u;
    const static unsigned int LongDuration_ms = 500;

    void setup(std::uint8_t pin)
    {
        pin_.set_pin(pin).set_input(true);
    }

    bool get_event(Event &event) const
    {
        if (!has_event_)
            return false;
        event.pressed = is_pressed_;
        return true;
    }

    void process(unsigned int elapse_ms)
    {
        has_event_ = false;

        pin_.check();

        const auto state_ix = pin_.is_input(true) ? Button::UpIX : Button::DownIX;
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
                is_pressed_ = new_pressed;
                has_event_ = true;
            }
        }
    }

private:
    gubg::arduino::Pin pin_;
    bool has_event_ = false;
    bool is_pressed_ = false;
    std::array<std::uint8_t, 2> debounce_counts_{};
};

class Relay
{
public:
    void setup(std::uint8_t pin, unsigned int short_duration_ms, unsigned int long_duration_ms)
    {
        pin_.set_pin(pin);
        activate_(false);
        short_duration_ms_ = short_duration_ms;
        long_duration_ms_ = long_duration_ms;
    }
    void set_peer(Relay *peer)
    {
        peer_ = peer;
    }

    bool is_active() const
    {
        return is_active_state_(state_);
    }

    void deactivate()
    {
        change_state_(State::Off);
    }

    void process(unsigned int elapse_ms)
    {
        auto on_timer = [&]()
        {
            switch (state_)
            {
                case State::DeactivatePeer:
                    change_state_(State::ShortPress);
                    break;
                case State::ShortPress:
                    change_state_(State::LongPress);
                    break;
                case State::On:
                    change_state_(State::Off);
                    break;
                default:
                    break;
            }
        };
        timer_.process(elapse_ms, on_timer);
    }
    void process(const Button::Event &button_event)
    {
        switch (state_)
        {
            case State::Off:
                if (button_event.pressed)
                    change_state_((peer_ && peer_->is_active()) ? State::DeactivatePeer : State::ShortPress);
                break;
            case State::ShortPress:
                change_state_(button_event.pressed ? State::Off : State::On);
                break;
            case State::On:
                if (button_event.pressed)
                    change_state_(State::Off);
                break;
        }
    }

private:
    enum class State {Off, DeactivatePeer, ShortPress, LongPress, On};

    static bool is_active_state_(State state)
    {
        switch (state)
        {
            case State::ShortPress:
            case State::LongPress:
            case State::On:
                return true;
            default:
                break;
        }
        return false;
    }

    void change_state_(State new_state)
    {
        if (new_state == state_)
            return;

        //Exit actions
        timer_.stop();
        switch (state_)
        {
            default:
                break;
        }

        state_ = new_state;

        //Enter actions
        switch (state_)
        {
            case State::DeactivatePeer:
                if (peer_)
                    peer_->deactivate();
                timer_.start(200);
                break;
            case State::ShortPress:
                active_ms_ = short_duration_ms_;
                timer_.start(200);
                break;
            case State::LongPress:
                active_ms_ = long_duration_ms_;
                change_state_(State::On);
                break;
            case State::On:
                timer_.start(active_ms_);
                break;
            default:
                break;
        }
        activate_(is_active_state_(state_));
    }

    void activate_(bool b)
    {
        //Pulling pin from relay down to 0V will activate the relay
        pin_.set_output(!b);
    }

    gubg::arduino::Pin pin_;
    State state_ = State::Off;
    gubg::arduino::Timer<unsigned int> timer_;
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
        button_.setup(pin);
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
        relay_.setup(pin, short_duration_ms, long_duration_ms);
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
            case Type::Button: button_.process(elapse_ms); break;
            case Type::Relay:  relay_.process(elapse_ms);  break;
            default: break;
        }
    }
    void process(const Button::Event &button_event)
    {
        switch (type_)
        {
            case Type::Button: break;
            case Type::Relay:  relay_.process(button_event);  break;
            default: break;
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
                relay.process(button_event);
            }
        }

        io.process(elapsed_ms());
    }
}

