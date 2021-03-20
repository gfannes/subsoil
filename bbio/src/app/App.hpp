#ifndef HEADER_app_App_hpp_ALREADY_INCLUDED
#define HEADER_app_App_hpp_ALREADY_INCLUDED

#include <app/Options.hpp>
#include <app/transform/Interface.hpp>
#include <list>

namespace app { 
    class App
    {
    public:
        const Options options;
        App(const Options &options): options(options) {}

        bool run();

    private:
        bool create_metadata_();
        bool create_transforms_();

        transform::Metadata metadata_;
        std::list<transform::Interface::Ptr> transforms_;
    };
} 

#endif
