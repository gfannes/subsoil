#include <gubg/arduino/ppm/Pin.hpp>
#include "Arduino.h"
#include "Servo.h"
using namespace gubg::arduino;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
ppm::Pin<2 , Min, Max> speed;//Connect with RC channel 3
ppm::Pin<3 , Min, Max> rl;   //Connect with RC channel 1

Servo servo;

void setup() {
    Serial.begin(9600);

    speed.setup();
    rl.setup();
}

void loop() {
    Serial.print(speed.as_float()); Serial.print(' ');
    Serial.print(rl.as_float()); Serial.println(' ');
}
