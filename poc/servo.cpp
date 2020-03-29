#include <Arduino.h>
#include <Servo.h>

Servo a;
Servo b;
template <typename Ftor>
void each_servo(Ftor && ftor)
{
    ftor(a);
    ftor(b);
}

void setup()
{
    unsigned int port = 10;
    auto attach = [&](auto &servo){
        servo.attach(port);
        --port;
    };
    each_servo(attach);
}

void loop()
{
    unsigned int angle = 0;
    auto set_angle = [&](auto &servo){
        servo.write(angle);
    };

    angle = 0;
    each_servo(set_angle);
    delay(2000);

    angle = 180;
    each_servo(set_angle);
    delay(2000);
}
