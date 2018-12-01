#include "Arduino.h"
#include "gubg/arduino/Elapsed.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/rs485/Endpoint.hpp"
#include "gubg/platform.h"

namespace  { 

    using micros_t = decltype(micros());

    gubg::arduino::Elapsed<micros_t> elapsed_time;

    class Blinker: public gubg::arduino::Timer_crtp<micros_t, Blinker>
    {
    public:
        void init()
        {
            pinMode(13, OUTPUT);
            timer_run();
        }
        void timer_run()
        {
            state_ = !state_;
            digitalWrite(13, state_);
            start_timer(state_ ? 50000ul : 450000ul);
        }

    private:
        bool state_ = false;
    };

    Blinker blinker;

    gubg::arduino::rs485::Endpoint rs485_endpoint;
} 

void setup()
{
    Serial.println("Starting the APP");

    elapsed_time.process(micros());
    blinker.init();
#if GUBG_PLATFORM_ARDUINO_MEGA
    rs485_endpoint.init(Serial1, 8, 9600, SERIAL_8N1);
#endif

    Serial.begin(9600);
}

enum class State {WaitUntilSent, SendNewMessage, Sending};
State state = State::WaitUntilSent;

const char *message = nullptr;
size_t offset, size;

void loop()
{
    elapsed_time.process(micros());

    rs485_endpoint.process(elapsed_time());
    blinker.process(elapsed_time());

    switch (state)
    {
        case State::WaitUntilSent:
            if (!rs485_endpoint.is_sending())
                state = State::SendNewMessage;
            break;
        case State::SendNewMessage:
            message = "Hello world\n";
            size = 12;
            offset = 0;
            state = State::Sending;
            break;
        case State::Sending:
            rs485_endpoint.send(offset, message, size);
            if (offset == size)
                state = State::WaitUntilSent;
            break;
    }

#if GUBG_PLATFORM_ARDUINO_MEGA
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
#endif
}
