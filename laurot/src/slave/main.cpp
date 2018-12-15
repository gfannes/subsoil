#include "Arduino.h"
#include "laurot/Id.hpp"
#include "gubg/arduino/rs485/Endpoint.hpp"
#include "gubg/string/Buffer.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/t2/Parser.hpp"
#include "gubg/t2/Segmenter.hpp"
#include "gubg/t2/Range.hpp"
#include "gubg/std/array.hpp"
#include "gubg/Range_macro.hpp"

#define gubg_no_log 1
#include "gubg/log.hpp"

namespace my { 
    using String = gubg::string::Buffer<char>;
    using T2Doc = gubg::t2::Document<String>;

    class Parser: public gubg::t2::Parser_crtp<Parser>
    {
    public:
        laurot::id::Type tag1;
        laurot::id::Type message_id;
        laurot::id::Type to;

        bool ready() const { return state_ == 2; }
        void reset() {state_ = 0;}

        template <typename Tag, typename Level>
        void t2_open(Tag tag, Level lvl)
        {
            TAG("open")ATTR(tag)ATTR(lvl)
            level_ = lvl;
            switch (level_)
            {
                case 0:
                    //Start of message
                    break;
                case 1:
                    state_ = 1;
                    tag1 = tag;
                    break;
            }
        }
        template <typename Key, typename Value>
        void t2_attr(Key key, Value value)
        {
            TAG("attr")ATTR(key)ATTR(value)
            switch (level_)
            {
                case 1:
                    switch (key)
                    {
                        case laurot::id::Id: message_id = value; break;
                        case laurot::id::To: to         = value; break;
                    }
                    break;
            }
        }
        template <typename Level>
        void t2_close(Level lvl)
        {
            TAG("close")ATTR(lvl)
            switch (lvl)
            {
                case 0:
                    //End of message
                    if (state_ == 1)
                        state_ = 2;
                    break;
            }
            level_ = --lvl;
        }

    private:
        int level_ = -1;
        unsigned int state_ = 0;
    };
    Parser parser;
}

namespace {
    gubg::arduino::rs485::Endpoint ep;

    const auto BufferSize = 16;
    std::array<char, BufferSize> buffer;
    size_t buffer_offset = 0;
    size_t buffer_size = 0;

    enum class State {Receiving, Sending};
    State state_ = State::Receiving;
    void change_state_(State new_state)
    {
        if (state_ == new_state)
            return;

        TAG("change_state_")ATTR((int)state_)ATTR((int)new_state)

        //Exit
        switch (state_)
        {
        }

        state_ = new_state;

        //Enter
        switch (state_)
        {
        }
    }
} 

void setup()
{
    ep.init(Serial1, 8, 9600, SERIAL_8N1);

    Serial.begin(9600);
}

void loop()
{
    ep.process();

    switch (state_)
    {
        case State::Receiving:
            {
                buffer_offset = 0;
                ep.receive(buffer_offset, buffer.data(), BufferSize);

                for (auto i = 0u; i < buffer_offset; ++i)
                {
                    TAG("ch")ATTR(i)
                    my::parser.process(buffer[i]);
                }

                if (my::parser.ready())
                {
                    TAG("received a message")

                    my::String string{RANGE(buffer)};
                    {
                        my::T2Doc doc{string};
                        auto answer = doc.tag(laurot::id::Answer);
                        answer.attr(laurot::id::Id, my::parser.message_id);
                    }
                    buffer_offset = 0;
                    buffer_size = string.size();
                    ATTR(buffer_size)

                    my::parser.reset();

                    change_state_(State::Sending);
                }
            }
            break;

        case State::Sending:
            {
                ep.send(buffer_offset, buffer.data(), buffer_size);
                if (!ep.is_sending())
                    change_state_(State::Receiving);
            }
            break;
    }

}
