#include "gubg/mlp/Structure.hpp"
#include "gubg/s11n.hpp"
#include "gubg/file/System.hpp"
#include <string>
#include <iostream>
using namespace gubg;

int main()
{
    enum {Neuron, HiddenLayer, Nr_};
    for (auto i = 0u; i < Nr_; ++i)
    {
        mlp::Structure s;
        std::filesystem::path fn;
        switch (i)
        {
            case Neuron:
                s.nr_inputs = 1;
                s.add_layer(neural::Transfer::Tanh, 1, 0.0, 0.0);
                fn = "neuron.naft";
                break;
            case HiddenLayer:
                s.nr_inputs = 1;
                s.add_layer(neural::Transfer::Tanh, 5, 0.0, 0.0);
                s.add_layer(neural::Transfer::Linear, 1, 0.0, 0.0);
                fn = "hidden_layer.naft";
                break;
        }

        std::cout << "Creating " << fn << " ... ";
        std::string content;
        s11n::write_object(content, ":mlp.Structure", s);

        file::write(content, fn);
        std::cout << " done" << std::endl;
    }
    return 0;
}
