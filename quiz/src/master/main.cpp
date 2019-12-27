#include <quiz/App.hpp>
#include <iostream>

int main(int argc, const char **argv)
{
    quiz::App app;
    std::string error;
    while (app(error)) { }
    if (!error.empty())
        std::cout << "Error: " << error << std::endl;
    else
        std::cout << "Everything went OK" << std::endl;
    return 0;
}
