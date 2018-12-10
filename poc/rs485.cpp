#include "Arduino.h"
#include "gubg/arduino/Pin.hpp"
#include "gubg/arduino/Morse.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/Elapsed.hpp"
#include "gubg/arduino/rs485/Endpoint.hpp"

namespace  { 
    using namespace gubg::arduino;
    
    rs485::Endpoint ep;

    const auto BufferSize = 16;
    Morse<BufferSize> morse;
    Pin pin13{13};

    using Micros = decltype(micros());
    Elapsed<Micros> elapsed_time;
    Timer<Micros> morse_tick_timer;
    const Micros morse_tick_duration = 200000;

    std::array<char, BufferSize+1> buffer;
    size_t buffer_size = 0;
} 

void setup()
{
    ep.init(Serial1, 8, 9600, SERIAL_8N1);
    morse_tick_timer.start(morse_tick_duration);

    Serial.begin(9600);
}

void loop()
{
    elapsed_time.process(micros());

    ep.process();

    ep.receive(buffer_size, buffer.data(), BufferSize);

    if (morse.empty() && buffer_size > 0)
    {
        buffer[buffer_size] = '\0';
        Serial.print("Sending data to morse code: ");Serial.println(buffer.data());
        morse.set_message(buffer.data());
        buffer_size = 0;
    }

    {
        auto lambda = [&](){
            morse_tick_timer.add(morse_tick_duration);
            pin13.set_output(morse.process());
        };
        morse_tick_timer.process(elapsed_time(), lambda);
    }
}
