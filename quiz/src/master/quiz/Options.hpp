#ifndef HEADER_quiz_Options_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Options_hpp_ALREADY_INCLUDED

#include <gubg/mss.hpp>
#include <gubg/Strange.hpp>
#include <optional>
#include <sstream>

namespace quiz { 

    class Options
    {
    public:
        std::string app_fn;

        bool print_help = false;
        std::optional<std::string> buttons_device;
        std::optional<unsigned int> baudrate;
        std::optional<std::pair<int,int>> position;

        bool parse(int argc, const char **argv)
        {
            MSS_BEGIN(bool);

            unsigned int ix = 0;
            auto pop_arg = [&]() -> std::string
            {
                if (ix >= argc)
                    return "";
                return argv[ix++];
            };

            app_fn = pop_arg();
            for (;ix < argc;)
            {
                const auto arg = pop_arg();
                if (false) {}
                else if (arg == "-h") { print_help = true; }
                else if (arg == "-t") { buttons_device = pop_arg(); }
                else if (arg == "-b") { baudrate = std::stoul(pop_arg()); }
                else if (arg == "-p")
                {
                    gubg::Strange strange{pop_arg()};
                    std::pair<int,int> pos;
                    MSS(strange.pop_decimal(pos.first));
                    MSS(strange.pop_if(','));
                    MSS(strange.pop_decimal(pos.second));
                    position = pos;
                }
            }

            MSS_END();
        }

        std::string help() const
        {
            std::ostringstream oss;
            oss << app_fn << std::endl;
            oss << "    -h            Print this help" << std::endl;
            oss << "    -t <FILENAME> Serial device to buttons" << std::endl;
            oss << "    -b <NUMBER>   Baudrate for serial connection" << std::endl;
            return oss.str();
        }
    private:
    };

} 

#endif
