#include "Arduino.h"
#include "gubg/string/Buffer.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/t2/Parser.hpp"
#include "gubg/t2/Segmenter.hpp"
#include "gubg/t2/Range.hpp"
#include "gubg/std/array.hpp"
#include "gubg/Range_macro.hpp"

namespace my { 
    using String = gubg::string::Buffer<char>;
    using T2Doc = gubg::t2::Document<String>;

    class Parser: public gubg::t2::Parser_crtp<Parser>
    {
    public:
        template <typename Tag, typename Level>
        void t2_open(Tag tag, Level level)
        {
            S("open");L(tag);L(level);
        }
        template <typename Key, typename Value>
        void t2_attr(Key key, Value value)
        {
            S("attr");L(key);L(value);
        }
        template <typename Level>
        void t2_close(Level level)
        {
            S("close");L(level);
        }
    private:
    };
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

    my::Parser parser;
    Serial.print("t2:");
    for (auto i = 0; i < string.size(); ++i)
    {
        const auto ch = string[i];
        Serial.print("Processing ");Serial.println(ch, HEX);
        parser.process(ch);
    }
}
