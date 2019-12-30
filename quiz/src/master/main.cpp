#include <quiz/Options.hpp>
#include <quiz/App.hpp>
#include <gubg/mss.hpp>
#include <iostream>

namespace  { 
    bool main_(int argc, const char **argv, std::string &error)
    {
        MSS_BEGIN(bool);

        quiz::Options options;
        MSS(options.parse(argc, argv));

        if (options.print_help)
        {
            std::cout << options.help();
            return true;
        }

        quiz::App app;
        MSS(app.process(options, error));
        while (app(error)) { }

        MSS_END();
    }
} 

int main(int argc, const char **argv)
{
    MSS_BEGIN(int);

    std::string error;
    MSS(main_(argc, argv, error), std::cout << "Error: " << error << std::endl);

    std::cout << "Everything went OK" << std::endl;

    MSS_END();
}
