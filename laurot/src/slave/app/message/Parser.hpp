#ifndef HEADER_app_message_Parser_hpp_ALREADY_INCLUDED
#define HEADER_app_message_Parser_hpp_ALREADY_INCLUDED

#include "laurot/Id.hpp"
#include "gubg/t2/Parser.hpp"

#define gubg_no_log 1
#include "gubg/log.hpp"

namespace app { namespace message { 

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

} } 

#endif
