#ifndef HEADER_app_message_Builder_hpp_ALREADY_INCLUDED
#define HEADER_app_message_Builder_hpp_ALREADY_INCLUDED

#include "laurot/Id.hpp"
#include "gubg/string/Buffer.hpp"
#include "gubg/t2/Builder.hpp"
#include "gubg/Range_macro.hpp"

namespace app { namespace message { 

    using String = gubg::string::Buffer<char>;
    using T2Doc = gubg::t2::Document<String>;

    template <typename Buffer, typename Ftor>
    void create_answer(Buffer &buffer, size_t &size, unsigned int message_id, Ftor &&ftor)
    {
        String string{RANGE(buffer)};

        {
            T2Doc doc{string};
            auto answer = doc.tag(laurot::id::Answer);
            answer.attr(laurot::id::Id, message_id);
            ftor(answer);
        }

        size = string.size();
    }

} } 

#endif
