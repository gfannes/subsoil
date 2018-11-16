#ifndef HEADER_laurot_Options_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Options_hpp_ALREADY_INCLUDED

#include "gubg/mss.hpp"

namespace laurot { 

    class Options
    {
    public:
        unsigned int nr_slaves = 0;
        unsigned int nr_iterations = 10;

        bool parse(int argc, const char ** argv)
        {
            MSS_BEGIN(bool);

            int arg_it = 1;
            auto pop_arg = [&](){ return std::string{arg_it >= argc ? "" : argv[arg_it++]}; };

            for (std::string key = pop_arg(); !key.empty(); key = pop_arg())
            {
                if (false) {}
                else if (key == "-s")
                    nr_slaves = std::stoi(pop_arg());
                else if (key == "-i")
                    nr_iterations = std::stoi(pop_arg());
            }

            MSS_END();
        }
    private:
    };

} 

#endif
