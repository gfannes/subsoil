#include "Arduino.h"
#include "gubg/arduino/Periodic.hpp"

namespace  { 
    class Blinker: public gubg::arduino::Periodic_crtp<Blinker, 50000>
    {
    public:
        void periodic_init()
        {
            pinMode(13, OUTPUT);
            digitalWrite(13, state_);
        }
        void periodic_run()
        {
            state_ = !state_;
            digitalWrite(13, state_);
        }

    private:
        bool state_ = true;
    };

    Blinker blinker;
} 

unsigned int write_buffer_size = 0;

void setup()
{
    pinMode(8, OUTPUT);
    Serial.begin(9600);
    /* Serial1.begin(9600, SERIAL_8O2); */
    Serial1.begin(9600, SERIAL_8N2);

    write_buffer_size = Serial1.availableForWrite();
}

const auto bufsize = 10;
char buffer[bufsize+1];

void loop()
{
    blinker.process(micros());

#if 0
    const auto available_for_write = Serial1.availableForWrite();
    Serial.print(available_for_write);
    Serial.print("\r\n");
    const auto is_writing = (write_buffer_size > available_for_write);
    delay(2);
    digitalWrite(8, is_writing);

    if (!is_writing)
    {
        auto ix = 0u;
        for (; ix < bufsize && Serial1.available(); ++ix)
        {
            buffer[ix] = Serial1.read();
            delay(1);
        }
        buffer[ix] = 0;
        if (ix > 0)
        {
            digitalWrite(8, HIGH);
            for (auto i = 0u; i < ix; ++i)
                Serial1.write(buffer[i]+1);
        }
    }
#endif
}
