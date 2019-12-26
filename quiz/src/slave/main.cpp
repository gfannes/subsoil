#include <Arduino.h>
#include <gubg/arduino/Pin.hpp>
#include <gubg/arduino/Elapsed.hpp>
#include <gubg/arduino/Timer.hpp>
#include <gubg/std/array.hpp>

namespace  { 
    using Millis = decltype(millis());
    using Timer = gubg::arduino::Timer<Millis>;
    using Elapsed = gubg::arduino::Elapsed<Millis>;

    Elapsed elapsed_time;

    gubg::arduino::Pin status_led{13};
    Timer status_timer;
    Millis status_duration = 100;

    class Button
    {
    public:
        char name() const {return ch_;}

        void setup(unsigned int pin, char ch)
        {
            pin_.set_pin(pin).set_input(true);
            ch_ = ch;
        }

        bool process(Millis millis)
        {
            bool toggled = pin_.toggled();
            if (millis-prev_toggle_timepoint_ < debounce_duration)
                return false;
            if (toggled)
                prev_toggle_timepoint_ = millis;
            return toggled;
        }

    private:
        const static Millis debounce_duration = 10;
        gubg::arduino::Pin pin_;
        char ch_ = '?';
        Millis prev_toggle_timepoint_ = 0;
    };
    std::array<Button, 5> buttons;
    unsigned int bix = 0;
} 

void setup()
{
    status_timer.start(status_duration);
    Serial.begin(9600);

    for (auto i = 0u; i < 5u; ++i)
        buttons[i].setup(8+i, 'a'+i);
}

void loop()
{
    elapsed_time.process(millis());

    auto on_status_timer = [&]()
    {
        status_led.toggle();
        status_timer.add(status_duration);
    };
    status_timer.process(elapsed_time(), on_status_timer);

    bix = (bix == 0 ? buttons.size()-1 : bix-1);
    auto &button = buttons[bix];
    if (button.process(millis()))
        Serial.write(button.name());
}
