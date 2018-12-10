#include "Arduino.h"
#include "gubg/string/Buffer.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/std/array.hpp"
#include "gubg/Range_macro.hpp"

namespace my { 
    using String = gubg::string::Buffer<char>;
    using T2Doc = gubg::t2::Document<String>;
} 

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    std::array<char, 16> buffer;
    my::String string{RANGE(buffer)};
    {
        my::T2Doc doc{string};
        auto n = doc.tag(0);
        n.attr(0xaaaa,0xbbbb);
        n.attr(3,4);
    }

    Serial.print("t2:");
    for (auto i = 0; i < string.size(); ++i)
    {
        Serial.print(' ');Serial.print(string[i], HEX);
    }
    Serial.println("");
}
