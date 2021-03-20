#ifndef HEADER_app_transform_Metadata_hpp_ALREADY_INCLUDED
#define HEADER_app_transform_Metadata_hpp_ALREADY_INCLUDED

#include <optional>

namespace app { namespace transform { 

    struct Metadata
    {
        unsigned int block_size = 0;
        unsigned int sample_rate = 0;
        unsigned int bits_per_sample = 0;
        std::optional<unsigned int> max_block_count;
    };

} } 

#endif
