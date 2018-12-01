#include "Arduino.h"
#include "gubg/arduino/Elapsed.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/platform.h"
#include "gubg/mss.hpp"

namespace gubg { namespace arduino { namespace rs485 { 

    class Endpoint: public Timer_crtp<unsigned long, Endpoint>
    {
    public:
        void init(HardwareSerial &hws, unsigned int tx_enable_pin)
        {
            hws_ = &hws;
            tx_enable_pin_ = tx_enable_pin;

            pinMode(tx_enable_pin_, OUTPUT);
            digitalWrite(tx_enable_pin_, false);
        }

        bool process(unsigned long elapse)
        {
            MSS_BEGIN(bool);
            MSS(!!hws_);
            MSS_END();
        }

    private:
        HardwareSerial *hws_ = nullptr;
        unsigned int tx_enable_pin_;
    };

} } } 

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

unsigned int write_buffer_size = 0;

void setup()
{
    Serial.println("Starting the APP");

    elapsed_time.process(micros());
    blinker.init();
#if GUBG_PLATFORM_ARDUINO_MEGA
    rs485_endpoint.init(Serial1, 8);
#endif

    Serial.begin(9600);
#if GUBG_PLATFORM_ARDUINO_MEGA
    /* Serial1.begin(9600, SERIAL_8O2); */
    Serial1.begin(9600, SERIAL_8N2);

    write_buffer_size = Serial1.availableForWrite();
#endif

}

const auto bufsize = 10;
char buffer[bufsize+1];

void loop()
{
    elapsed_time.process(micros());

    rs485_endpoint.process(elapsed_time());
    blinker.process(elapsed_time());

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
