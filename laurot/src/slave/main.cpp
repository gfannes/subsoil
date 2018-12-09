#include "Arduino.h"
#include "gubg/arduino/Elapsed.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/arduino/Pin.hpp"
#include "gubg/arduino/rs485/Endpoint.hpp"
#include "gubg/platform.h"

namespace  { 

    using Micros = decltype(micros());

    gubg::arduino::Elapsed<Micros> elapsed_time;

    class Blinker
    {
    public:
        enum Mode {Fast, Pinkle, On};

        void init()
        {
            toggle_timer_.start(timeout_us_());
        }

        void set_mode(Mode mode)
        {
            mode_ = mode;
            toggle_timer_.start(timeout_us_());
        }

        void process(unsigned long elapsed)
        {
            toggle_timer_.process(elapsed, [&](){
                    pin_.toggle();
                    toggle_timer_.start(timeout_us_());
                    });
        }

    private:
        unsigned long timeout_us_()
        {
            switch (mode_)
            {
                case Mode::Fast: return 50000;
                case Mode::Pinkle: return pin_.is_output(true) ? 50000 : 450000;
                case Mode::On: return pin_.is_output(true) ? 50000 : 1;
            }
        }

        Mode mode_ = Mode::Pinkle;
        gubg::arduino::Pin pin_{13};
        gubg::arduino::Timer<Micros> toggle_timer_;
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

void change_state(State new_state)
{
    if (new_state == state)
        return;
    //Exit
    switch (state)
    {
    }
    state = new_state;
    //Enter
    switch (state)
    {
        case State::WaitUntilSent:
            blinker.set_mode(Blinker::On);
            break;
        case State::SendNewMessage:
            break;
        case State::Sending:
            blinker.set_mode(Blinker::Fast);
            break;
    }
}

const char *message = nullptr;
size_t offset, size;

void loop()
{
    elapsed_time.process(micros());

    rs485_endpoint.process();
    blinker.process(elapsed_time());

    switch (state)
    {
        case State::WaitUntilSent:
            if (!rs485_endpoint.is_sending())
                change_state(State::SendNewMessage);
            break;
        case State::SendNewMessage:
#if 0
            message = "Hello world\n";
            size = 12;
#else
            message = "\x55\x5a";
            size = 2;
#endif
            offset = 0;
            change_state(State::Sending);
            break;
        case State::Sending:
            rs485_endpoint.send(offset, message, size);
            if (offset == size)
                change_state(State::WaitUntilSent);
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
