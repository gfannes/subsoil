#include <gubg/arduino/ppm/Pin11.hpp>
#include <gubg/arduino/Pin.hpp>
#include "Arduino.h"
using namespace gubg::arduino;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
ppm::Pin11<2 , Min, Max> ppm_sp;//Connect with RC channel 3
ppm::Pin11<3 , Min, Max> ppm_rl;//Connect with RC channel 1

Pin power_h_bridge{8};

class Motor
{
public:
    Motor(unsigned int pwm, unsigned int dir): pwm_(pwm), dir_(dir) {}

    void setup()
    {
        pinMode(pwm_, OUTPUT);
        pinMode(dir_, OUTPUT);

        analogWrite(pwm_, 0);
        digitalWrite(dir_, 0);
    }

    void set_speed(float speed)
    {
        if (speed >= 0)
        {
            analogWrite(pwm_, speed*256.0f);
            digitalWrite(dir_, 0);
        }
        else
        {
            analogWrite(pwm_, -speed*256.0f);
            digitalWrite(dir_, 1);
        }
    }

private:
    const unsigned int pwm_;
    const unsigned int dir_;
};

Motor motor_l{5,4};
Motor motor_r{6,7};

void setup() {
    Serial.begin(9600);

    ppm_sp.setup();
    ppm_rl.setup();

    motor_l.setup();
    motor_r.setup();

    power_h_bridge.set_output(true);
}

void loop() {
    const auto sp = ppm_sp.as_float();
    const auto rl = ppm_rl.as_float();

    const auto speed_r = (sp-rl)*0.5f;
    const auto speed_l = -(sp+rl)*0.5f;
    Serial.print(speed_r); Serial.print(' '); Serial.println(speed_l);

    motor_r.set_speed(speed_r);
    motor_l.set_speed(speed_l);
}
