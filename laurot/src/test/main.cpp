#define gubg_no_log 0
#include "gubg/log.hpp"

#include "Arduino.h"

template <typename Ftor>
void each_pin(Ftor &&ftor)
{
    for (auto pin = 46u; pin < 54u; ++pin)
        ftor(pin);
}

void setup()
{
    each_pin([](auto pin){pinMode(pin, OUTPUT);});
    each_pin([](auto pin){digitalWrite(pin, true);});
}

void loop()
{
    auto blink = [](auto pin)
    {
        digitalWrite(pin, false);
        delay(500);
        digitalWrite(pin, true);
        delay(500);
    };
    each_pin(blink);
}

