#include <app/Options.hpp>
#include <app/App.hpp>
#include <gubg/mss.hpp>
#include <iostream>

namespace app { 
    bool main(int argc, const char **argv)
    {
        MSS_BEGIN(bool);

        Options options;
        MSS(options.parse(argc, argv));
        if (options.print_help)
        {
            std::cout << options.help();
            return true;
        }

        App app{options};

        MSS(app.run());

        MSS_END();
    }
} 

int main(int argc, const char **argv)
{
    if (!app::main(argc, argv))
    {
        std::cout << "Error: something went wrong" << std::endl;
        return -1;
    }
    std::cout << "Everything went OK" << std::endl;
    return 0;
}
