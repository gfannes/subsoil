#include <gubg/arduino/ppm/Pin01.hpp>
#include <gubg/arduino/ppm/Pin11.hpp>
using namespace gubg::arduino;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
ppm::Pin01<2 , Min, Max> ch1;
ppm::Pin11<3 , Min, Max> ch2;

void setup()
{
    Serial.begin(9600);

    ch1.setup();
    ch2.setup();
}

void loop()
{
    Serial.print(ch1.as_float());
    Serial.print(' ');
    Serial.println(ch2.as_float());
}
