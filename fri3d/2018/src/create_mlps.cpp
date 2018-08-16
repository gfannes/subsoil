#include "gubg/mlp/Structure.hpp"
#include "gubg/data/Set.hpp"
#include "gubg/s11n.hpp"
#include "gubg/file/System.hpp"
#include "gubg/debug.hpp"
#include "gubg/std/optional.hpp"
#include "gubg/math/constants.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <random>
using namespace gubg;

std::mt19937 rng;

int main()
{
    S("");
    enum {Neuron, HiddenLayer, DeepNetwork, SineData, NoisySineData, CirleData, Nr_};
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
            case DeepNetwork:
                mlp.emplace(1);
                mlp->add_layer(neural::Transfer::Tanh, 3, 0.0, 0.0);
                mlp->add_layer(neural::Transfer::Tanh, 3, 0.0, 0.0);
                mlp->add_layer(neural::Transfer::Tanh, 3, 0.0, 0.0);
                mlp->add_layer(neural::Transfer::Tanh, 3, 0.0, 0.0);
                mlp->add_layer(neural::Transfer::Linear, 1, 0.0, 0.0);
                fn = "mlp.deep_network.naft";
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
            case NoisySineData:
                {
                    std::normal_distribution<> normal(0, 0.1);
                    data.emplace();
                    data->fields.emplace_back("input", 1);
                    data->fields.emplace_back("output", 1);
                    for (auto x = -3.0; x <= 3.0; x += 0.2)
                    {
                        auto y = 0.7*std::sin(x)+normal(rng);
                        L(C(x)C(y));
                        auto &r = data->add_record();
                        r.add_data(x);
                        r.add_data(y);
                    }
                    fn = "data.noisy_sine.naft";
                }
                break;
            case CirleData:
                data.emplace();
                data->fields.emplace_back("input", 2);
                data->fields.emplace_back("output", 2);
                for (auto angle = 0.0; angle <= gubg::math::tau; angle += gubg::math::tau/30)
                {
                    auto &r = data->add_record();
                    r.add_data(std::cos(angle), std::sin(angle));
                    r.add_data(-std::sin(angle), std::cos(angle));
                }
                    fn = "data.circle.naft";
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
