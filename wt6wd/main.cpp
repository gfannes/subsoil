#include "Arduino.h"

template <unsigned int Pin, unsigned int Min, unsigned int Max>
class PPMPin
{
public:
    void setup()
    {
        pinMode(Pin, INPUT);
        attachInterrupt(digitalPinToInterrupt(Pin), process_pin_change_, CHANGE);
        prev_ = 0;
        pulse_duration_ = 0;
    }

    unsigned int diff() const { return pulse_duration_; }
    float as_float() const { return float(pulse_duration_-Min)/float(Max-Min); }

private:
    static void process_pin_change_()
    {
        const unsigned int now = micros();
        if (digitalRead(Pin) == HIGH)
            prev_ = now;
        else
        {
            pulse_duration_ = now-prev_;
            if (pulse_duration_ < Min)
                pulse_duration_ = Min;
            else if (pulse_duration_ > Max)
                pulse_duration_ = Max;
        }
    }

    static unsigned int prev_;
    static unsigned int pulse_duration_;
};
template <unsigned int Pin, unsigned int Min, unsigned int Max>
unsigned int PPMPin<Pin, Min, Max>::prev_ = 0;
template <unsigned int Pin, unsigned int Min, unsigned int Max>
unsigned int PPMPin<Pin, Min, Max>::pulse_duration_ = 0;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
PPMPin<2 , Min, Max> ch1;
PPMPin<3 , Min, Max> ch2;
PPMPin<18, Min, Max> ch3;
PPMPin<19, Min, Max> ch4;

void setup() {
    Serial.begin(9600);
    ch1.setup();
    ch2.setup();
    ch3.setup();
    ch4.setup();
}

void loop() {
    Serial.print(ch1.as_float()); Serial.print("\t");
    Serial.print(ch2.as_float()); Serial.print("\t");
    Serial.print(ch3.as_float()); Serial.print("\t");
    Serial.print(ch4.as_float()); Serial.println("");
    /* delay(200); */
}
