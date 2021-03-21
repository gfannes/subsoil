#ifndef HEADER_app_App_hpp_ALREADY_INCLUDED
#define HEADER_app_App_hpp_ALREADY_INCLUDED

#include <app/Options.hpp>
#include <app/codec/Interface.hpp>
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
        bool create_codecs_();

        codec::Metadata metadata_;
        std::list<codec::Interface::Ptr> codecs_;
    };
} 

#endif
