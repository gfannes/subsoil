#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/std/array.hpp>
#include <gubg/std/algorithm.hpp>

#define gubg_no_log 0
#include "gubg/log.hpp"

#include "Arduino.h"

class Relay
{
public:
    void set_pin(std::uint8_t pin) { pin_.set_pin(pin).set_output(true); }

    void set_duration(unsigned int ms) {duration_ms_ = ms;}

    void activate() { active_ms_ = duration_ms_; }

    void process(unsigned int elapse_ms)
    {
        const bool active = active_ms_ > 0;
        pin_.set_output(!active);

        elapse_ms = std::min(elapse_ms, active_ms_);
        active_ms_ -= elapse_ms;
    }
private:
    gubg::arduino::Pin pin_;
    unsigned int duration_ms_ = 2000;
    unsigned int active_ms_ = 0;
};

class Button
{
public:
    void set_pin(std::uint8_t pin) { pin_.set_pin(pin).set_input(true); }

    bool pressed() { return pin_.check().is_input(false); }
private:
    gubg::arduino::Pin pin_;
};

std::array<Relay, 2*7> relays{};
std::array<Button, 2*7> buttons{};

gubg::arduino::Elapsed<unsigned int> elapsed_ms;

void setup()
{
    for (auto ix = 0u; ix < buttons.size(); ++ix)
        buttons[ix].set_pin(26+ix);
    for (auto ix = 0u; ix < relays.size(); ++ix)
        relays[ix].set_pin(40+ix);;
}

void loop()
{
    elapsed_ms.process(millis());

    for (auto ix = 0u; ix < std::min(buttons.size(), relays.size()); ++ix)
    {
        auto &button = buttons[ix];
        auto &relay = relays[ix];

        if (button.pressed())
            relay.activate();

        relay.process(elapsed_ms());
    }
}

