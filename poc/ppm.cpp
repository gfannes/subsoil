#include <gubg/arduino/ppm/Pin.hpp>
using namespace gubg::arduino;

const unsigned int Min = 1050;
const unsigned int Max = 1850;
ppm::Pin<2 , Min, Max> ch1;
ppm::Pin<3 , Min, Max> ch2;

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
