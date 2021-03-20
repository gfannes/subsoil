#ifndef HEADER_app_transform_Interface_hpp_ALREADY_INCLUDED
#define HEADER_app_transform_Interface_hpp_ALREADY_INCLUDED

#include <app/transform/types.hpp>
#include <app/transform/Metadata.hpp>
#include <memory>

namespace app { namespace transform { 

    class Interface
    {
    public:
        using Ptr = std::shared_ptr<Interface>;

        virtual ~Interface() {}

        virtual bool setup(const KeyValues &, Metadata &) = 0;
        virtual bool transform(const Block &input, Block &output) = 0;
    };

} } 

#endif
