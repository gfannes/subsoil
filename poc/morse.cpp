#include "Arduino.h"
#include "gubg/arduino/Morse.hpp"
#include "gubg/arduino/Pin.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/Elapsed.hpp"

namespace  { 
    using Morse = gubg::arduino::Morse<16>;
    Morse morse;

    using Micros = decltype(micros());
    using Timer = gubg::arduino::Timer<Micros>;
    Timer morse_tick_timer;

    const Micros tick_duration = 200000;

    gubg::arduino::Elapsed<Micros> elapsed_time;

    gubg::arduino::Pin pin13{13};
} 

void setup()
{
    morse_tick_timer.start(tick_duration);
    Serial.begin(9600);
}

void loop()
{
    elapsed_time.process(micros());

    auto lambda = [&](){
        morse_tick_timer.add(tick_duration);

        if (morse.empty())
            morse.set_message("sos ");
        else
        {
            const auto signal = morse.process();
            pin13.set_output(signal);
        }
    };
    morse_tick_timer.process(elapsed_time(), lambda);
}
