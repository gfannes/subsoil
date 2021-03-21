#include <app/Options.hpp>
#include <gubg/mss.hpp>
#include <sstream>

namespace app { 
    bool Options::parse(int argc, const char **argv)
    {
        MSS_BEGIN(bool);

        std::size_t arg_ix = 0;
        auto pop_string = [&](std::string &str){
            MSS_BEGIN(bool);
            MSS_Q(arg_ix < argc);
            str = argv[arg_ix++];
            MSS_END();
        };
        auto pop_number = [&](auto &nbr){
            MSS_BEGIN(bool);
            std::string str;
            MSS(pop_string(str));
            nbr = std::stod(str);
            MSS_END();
        };

        MSS(pop_string(exe_name));

        for (std::string arg; pop_string(arg); )
        {
            auto is = [&](auto sh, auto lh){ return arg == sh || arg == lh; };

            if (false) {}
            else if (is("-h", "--help")) {print_help = true;}
            else if (is("-V", "--verbose"))     { MSS(pop_number(verbose_level),         std::cout << "Error: verbosity_level" << std::endl); }
            else if (is("-s", "--block_size"))  { MSS(pop_number(block_size),            std::cout << "Error: expected block size" << std::endl); }
            else if (is("-c", "--codec"))       { MSS(pop_string(codecs.emplace_back()), std::cout << "Error: expected codec description" << std::endl); }
            else if (is("-n", "--block_count"))
            {
                int count;
                MSS(pop_number(count), std::cout << "Error: expected block count" << std::endl);
                if (count >= 0)
                    block_count = count;
            }
            else {MSS(false, std::cout << "Error: unknown option \"" << arg << "\"" << std::endl);}
        }

        MSS_END();
    }

    std::string Options::help() const
    {
        std::ostringstream oss;
        oss << "Help for " << exe_name << std::endl;
        oss << "    -h --help                   Print this help" << std::endl;
        oss << "    -V --verbose     LEVEL      Verbosity level" << std::endl;
        oss << "    -s --block_size  NUMBER     Block size to use during processing" << std::endl;
        oss << "    -t --codec       STRING     Codec description" << std::endl;
        oss << "    -n --block_count NUMBER     Number of blocks to process" << std::endl;
        return oss.str();
    }
} 
