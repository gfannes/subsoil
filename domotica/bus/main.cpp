#include "gubg/ip/Socket.hpp"
#include "gubg/mss.hpp"
#include <iostream>

namespace app
{
    class App
    {
    public:
        bool process(int argc, const char **argv)
        {
            MSS_BEGIN(bool);
            MSS_END();
        }
    private:
    };
}

int main(int argc, const char **argv)
{
    app::App app;
    if (!app.process(argc, argv))
    {
        std::cout << "Could not process the LCI arguments" << std::endl;
        return -1;
    }
    std::cout << "Everything went OK" << std::endl;
    return 0;
}
