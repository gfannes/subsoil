#include "gubg/mlp/Structure.hpp"
#include "gubg/data/Set.hpp"
#include "gubg/s11n.hpp"
#include "gubg/file/System.hpp"
#include "gubg/debug.hpp"
#include "gubg/std/optional.hpp"
#include <string>
#include <iostream>
#include <cmath>
using namespace gubg;

int main()
{
    S("");
    enum {Neuron, HiddenLayer, SineData, Nr_};
    for (auto i = 0u; i < Nr_; ++i)
    {
        std::optional<mlp::Structure> mlp;
        std::optional<data::Set<double>> data;
        std::filesystem::path fn;
        switch (i)
        {
            case Neuron:
                mlp.emplace(1);
                mlp->add_layer(neural::Transfer::Tanh, 1, 0.0, 0.0);
                fn = "mlp.neuron.naft";
                break;
            case HiddenLayer:
                mlp.emplace(1);
                mlp->add_layer(neural::Transfer::Tanh, 5, 0.0, 0.0);
                mlp->add_layer(neural::Transfer::Linear, 1, 0.0, 0.0);
                fn = "mlp.hidden_layer.naft";
                break;
            case SineData:
                data.emplace();
                data->fields.emplace_back("input", 1);
                data->fields.emplace_back("output", 1);
                for (auto x = -3.0; x <= 3.0; x += 0.1)
                {
                    auto y = std::sin(x);
                    L(C(x)C(y));
                    auto &r = data->add_record();
                    r.add_data(x);
                    r.add_data(y);
                }
                fn = "data.sine.naft";
                break;
            default:
                break;
        }

        std::cout << "Creating " << fn << " ... ";
        if (mlp)
            s11n::write_object_to_file(fn, ":mlp.Structure", *mlp);
        if (data)
            s11n::write_object_to_file(fn, ":data.Set", *data);
        std::cout << " done" << std::endl;
    }
    return 0;
}
