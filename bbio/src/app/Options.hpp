#ifndef HEADER_app_Options_hpp_ALREADY_INCLUDED
#define HEADER_app_Options_hpp_ALREADY_INCLUDED

#include <string>
#include <optional>

namespace app { 

    class Options
    {
    public:
        std::string exe_name;
        bool print_help = false;
        int verbose_level = 0;
        unsigned int block_size = 0;
        std::optional<int> block_count;
        std::optional<std::string> input_filepath;
        std::optional<std::string> output_filepath;

        bool parse(int argc, const char **argv);

        std::string help() const;
    };

} 

#endif
