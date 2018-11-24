#include "Arduino.h"

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(8, OUTPUT);
    Serial.begin(9600, SERIAL_8O2);
    /* Serial1.begin(9600, SERIAL_8O2); */
    Serial1.begin(9600, SERIAL_8N2);
}

const auto bufsize = 10;
char buffer[bufsize+1];

void loop()
{
    digitalWrite(8, LOW);
    delay(1);
    auto ix = 0u;
    for (; ix < bufsize && Serial1.available(); ++ix)
    {
        buffer[ix] = Serial1.read();
    }
    buffer[ix] = 0;
    delay(1);

    if (ix > 0)
    {
        digitalWrite(8, HIGH);
        delay(1);
        if (false)
            Serial1.write("\x12\x23");
        else
        {
            for (auto i = 0u; i < ix; ++i)
            {
                Serial1.write(buffer[i]+1);
                delay(1);
            }
        }
        delay(3);
    }
}
