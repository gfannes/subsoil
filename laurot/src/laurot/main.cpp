#include "laurot/App.hpp"
#include "gubg/mss.hpp"
#include <iostream>

namespace laurot { 
    bool main(int argc, const char **argv)
    {
        MSS_BEGIN(bool);

        App app;
        MSS(app.process(argc, argv));
        MSS(app.run());

        MSS_END();
    }
} 

int main(int argc, const char **argv)
{
    if (!laurot::main(argc, argv))
    {
        std::cout << "Error: main failed" << std::endl;
        return -1;
    }
    std::cout << "Everything went OK" << std::endl;
    return 0;
}
