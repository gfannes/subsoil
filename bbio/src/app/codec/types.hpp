#ifndef HEADER_app_codec_types_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_types_hpp_ALREADY_INCLUDED

#include <vector>
#include <list>
#include <string>

namespace app { namespace codec { 

    using Vector = std::vector<float>;
    using Block = std::vector<Vector>;

    //Sets the same structure in `dst` as in `src`
    inline void restructure(Block &dst, const Block &src)
    {
        dst.resize(src.size());
        for (auto ix = 0u; ix < src.size(); ++ix)
            dst[ix].resize(src[ix].size());
    }

} } 

#endif
