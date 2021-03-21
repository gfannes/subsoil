#ifndef HEADER_app_codec_Interface_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_Interface_hpp_ALREADY_INCLUDED

#include <app/codec/types.hpp>
#include <app/kv/KeyValue.hpp>
#include <app/codec/Metadata.hpp>
#include <memory>

namespace app { namespace codec { 

    class Interface
    {
    public:
        using Ptr = std::shared_ptr<Interface>;

        virtual ~Interface() {}

        virtual bool setup(const kv::KeyValues &, Metadata &) = 0;
        virtual bool operator()(const Block &input, Block &output) = 0;
    };

} } 

#endif
