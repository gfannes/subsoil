#include "Arduino.h"

template <unsigned int Pin>
class PPMPin
{
public:
    void setup()
    {
        pinMode(Pin, INPUT);
        attachInterrupt(digitalPinToInterrupt(Pin), process_pin_change_, CHANGE);
    }
private:
    static void process_pin_change_()
    {
    }
};

PPMPin<2> pin2;

void setup() {
    Serial.begin(9600);
    pin2.setup();
}

void loop() {
    Serial.println("loop");
    delay(200);
}
