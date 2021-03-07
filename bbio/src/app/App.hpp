#ifndef HEADER_app_App_hpp_ALREADY_INCLUDED
#define HEADER_app_App_hpp_ALREADY_INCLUDED

#include <app/Options.hpp>

namespace app { 
    class App
    {
    public:
        const Options options;
        App(const Options &options): options(options) {}

        bool run();

    private:
    };
} 

#endif
