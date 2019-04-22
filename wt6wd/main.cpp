#include <gubg/arduino/ppm/Pin.hpp>
#include "Arduino.h"
#include "Servo.h"
using namespace gubg::arduino;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
ppm::Pin<2 , Min, Max> ch1;
ppm::Pin<3 , Min, Max> ch2;
ppm::Pin<18, Min, Max> ch3;
ppm::Pin<19, Min, Max> ch4;

Servo servo;

void setup() {
    Serial.begin(9600);
    ch1.setup();
    ch2.setup();
    ch3.setup();
    ch4.setup();

    /* servo.attach(9); */
    pinMode(9, OUTPUT);
}

void loop() {
    /* Serial.print(ch1.as_float()); Serial.print("\t"); */
    /* Serial.print(ch2.as_float()); Serial.print("\t"); */
    /* Serial.print(ch3.as_float()); Serial.print("\t"); */
    /* Serial.print(ch4.as_float()); Serial.println(""); */
    /* servo.write(ch1.as_float()*180); */
    analogWrite(9, ch1.as_float()*255);
    /* delay(200); */
}
