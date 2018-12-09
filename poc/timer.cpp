#include "Arduino.h"
#include "gubg/std/cstdint.hpp"
#include "gubg/arduino/Elapsed.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/Pin.hpp"

namespace  { 
    using Micros = decltype(micros());
    using Elapsed = gubg::arduino::Elapsed<Micros>;

    Elapsed elapsed;

    gubg::arduino::Pin pin13{13};
    gubg::arduino::Timer<Micros> toggle_led13;
} 

void setup()
{
    toggle_led13.start(100000ul);
}

void loop()
{
    elapsed.process(micros());

    toggle_led13.process(elapsed(), [&](){
            pin13.toggle();
            toggle_led13.start(100000ul);
            });
}
