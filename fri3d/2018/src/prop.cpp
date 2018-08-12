#include "Arduino.h"
#include "Servo.h"

template <unsigned int Pin, unsigned int Min, unsigned int Max>
class PPMPin
{
public:
    void setup()
    {
        pinMode(Pin, INPUT);
        attachInterrupt(digitalPinToInterrupt(Pin), process_rising_, RISING);
        prev_ = 0;
        pulse_duration_ = 0;
    }

    unsigned int diff() const { return (pulse_duration_ < Min ? Min : (pulse_duration_ > Max ? Max : pulse_duration_)); }
    float as_float() const { return float(diff()-Min)/float(Max-Min); }

private:
    static void process_rising_()
    {
        attachInterrupt(digitalPinToInterrupt(Pin), process_falling_, FALLING);
        prev_ = micros();
    }
    static void process_falling_()
    {
        attachInterrupt(digitalPinToInterrupt(Pin), process_rising_, RISING);
        pulse_duration_ = micros() - prev_;
    }

    static volatile unsigned int prev_;
    static volatile unsigned int pulse_duration_;
};
template <unsigned int Pin, unsigned int Min, unsigned int Max>
volatile unsigned int PPMPin<Pin, Min, Max>::prev_ = 0;
template <unsigned int Pin, unsigned int Min, unsigned int Max>
volatile unsigned int PPMPin<Pin, Min, Max>::pulse_duration_ = 0;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
PPMPin<2 , Min, Max> ch1;
PPMPin<3 , Min, Max> ch2;

Servo servo1;
Servo servo2;

void setup() {
    Serial.begin(9600);
    ch1.setup();
    ch2.setup();

    servo1.attach(4);
    servo2.attach(5);
}

void loop() {
    //Brushless motor
    {
        int v = ch1.as_float()*100+25;
        servo1.write(v);
    }

    //Servo motor
    {
        servo2.write(ch2.as_float()*179);
    }
}
