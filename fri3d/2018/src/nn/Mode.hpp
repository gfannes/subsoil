#ifndef HEADER_nn_Mode_hpp_ALREADY_INCLUDED
#define HEADER_nn_Mode_hpp_ALREADY_INCLUDED

#include <memory>

namespace nn { 
    class Mode
    {
    public:
        using Ptr = std::shared_ptr<Mode>;

        virtual ~Mode(){}

        virtual bool imgui() = 0;

    private:
    };
} 

#endif
