#ifndef HEADER_app_transform_types_hpp_ALREADY_INCLUDED
#define HEADER_app_transform_types_hpp_ALREADY_INCLUDED

#include <vector>
#include <list>
#include <utility>
#include <string>

namespace app { namespace transform { 

    using Vector = std::vector<float>;
    using Block = std::vector<Vector>;

    using KeyValue = std::pair<std::string, std::string>;
    using KeyValues = std::list<KeyValue>;

} } 

#endif
