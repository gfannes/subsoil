#include "Arduino.h"
#include "app/App.hpp"
#include "gubg/arduino/Elapsed.hpp"

#define gubg_no_log 1
#include "gubg/log.hpp"

namespace my { 
    using Micros = decltype(micros());
    gubg::arduino::Elapsed<Micros> elapsed;

    app::App app;
} 

void setup()
{
    my::app.setup();

    Serial.begin(9600);
}

void loop()
{
    my::elapsed.process(micros());

    my::app.process(my::elapsed());
}
