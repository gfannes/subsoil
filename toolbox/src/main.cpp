#include "subsoil/Toolbox.hpp"
#include <iostream>

int main()
{
    subsoil::toolbox::Toolbox tb([](auto &params){
            params.width = 50;
            });
    std::cout << tb;
    std::cout << "Everything went OK" << std::endl;
    return 0;
}
