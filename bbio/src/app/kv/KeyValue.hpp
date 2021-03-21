#ifndef HEADER_app_kv_KeyValue_hpp_ALREADY_INCLUDED
#define HEADER_app_kv_KeyValue_hpp_ALREADY_INCLUDED

#include <string>
#include <list>
#include <ostream>

namespace app { namespace kv { 

    struct KeyValue
    {
        std::string key;
        std::string value;
    };

    inline std::ostream &operator<<(std::ostream &os, const KeyValue &kv)
    {
        return os << kv.key << '=' << kv.value << ';';
    }

    using KeyValues = std::list<KeyValue>;
} } 

#endif
