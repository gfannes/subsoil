#ifndef HEADER_laurot_Options_hpp_ALREADY_INCLUDED
#define HEADER_laurot_Options_hpp_ALREADY_INCLUDED

#include "gubg/mss.hpp"
#include "gubg/log.hpp"
#include <sstream>

namespace app { 

    class Options
    {
    public:
        bool print_help = false;
        std::string tty;
        unsigned int baud_rate = 9600;
        unsigned int nr_slaves = 0;

        bool parse(int argc, const char ** argv)
        {
            MSS_BEGIN(bool);

            int arg_it = 1;
            auto pop_arg = [&](){ return std::string{arg_it >= argc ? "" : argv[arg_it++]}; };

            for (std::string key = pop_arg(); !key.empty(); key = pop_arg())
            {
                if (false) {}
                else if (key == "-h")
                    print_help = true;
                else if (key == "-t")
                    tty = pop_arg();
                else if (key == "-b")
                    baud_rate = std::stoi(pop_arg());
                else if (key == "-s")
                    nr_slaves = std::stoi(pop_arg());
            }

            MSS_END();
        }

        std::string help() const
        {
            std::ostringstream oss;
            oss << "\t-h\t\tprint this help\n";
            oss << "\t-t <filename>\ttty filename\n";
            oss << "\t-b <baudrate>\ttty baudrate\n";
            oss << "\t-s <count>\tslave count\n";
            return oss.str();
        }

        void stream(std::ostream &os) const
        {
            TAG(os, "Options");
            ATTR(print_help);
            ATTR(tty);
            ATTR(baud_rate);
            ATTR(nr_slaves);
        }
    private:
    };

    std::ostream &operator<<(std::ostream &os, const Options &options)
    {
        options.stream(os);
        return os;
    }

} 

#endif
