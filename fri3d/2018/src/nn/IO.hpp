#ifndef HEADER_nn_IO_hpp_ALREADY_INCLUDED
#define HEADER_nn_IO_hpp_ALREADY_INCLUDED

#include "nn/Mode.hpp"
#include "gubg/neural/Network.hpp"
#include "gubg/mss.hpp"
#include "imgui.h"
#include <iostream>

namespace nn { 
    class IO: public Mode
    {
    public:
        IO(int nr_layers)
        {
            input_ = network_.add_external(1);
            bias_ = network_.add_external(1);
            switch (nr_layers)
            {
                case 1:
                    network_.add_neuron(gubg::neural::Transfer::Tanh, Inputs{input_, bias_}, output_, weight_);
                    break;
                case 2:
                    network_.add_neuron(gubg::neural::Transfer::Tanh, Inputs{input_, bias_}, output_, weight_);
                    break;
            }
            states_.resize(network_.nr_states());
            states_[bias_] = 1.0;
            weights_.resize(network_.nr_weights());
        }
        virtual ~IO() {}

        bool imgui() override
        {
            MSS_BEGIN(bool);
            ImGui::Text("Network has %d states and %d weights", network_.nr_states(), network_.nr_weights());
            ImGui::SliderFloat("Weight", &weights_[weight_], -2.0, 2.0);
            ImGui::SliderFloat("Bias", &weights_[weight_+1], -2.0, 2.0);
            ImGui::SliderFloat("Input", &input_value_, -2.0, 2.0);
            states_[input_] = input_value_;
            network_.forward(states_.data(), weights_.data());
            auto output = states_[output_];
            const auto height = 100;
            compute_outputs_(100);
            ImGui::PlotLines("", outputs_.data(), outputs_.size(), 0, "", -1.0, 1.0, ImVec2(0,height));
            ImGui::SameLine();
            ImGui::VSliderFloat("Output", ImVec2(40,height), &output, -1.0, 1.0);
            MSS_END();
        }
    private:
        void compute_outputs_(size_t nr)
        {
            if (outputs_.size() != nr)
                outputs_.resize(nr);

            const auto min = -2.0, max = 2.0;
            const auto d = (max-min)/nr;
            auto &input = states_[input_];
            input = min;
            for (auto &output: outputs_)
            {
                network_.forward(states_.data(), weights_.data());
                output = states_[output_];
                input += d;
            }
        }

        using Float = float;
        using Floats = std::vector<Float>; 
        using Inputs = std::vector<size_t>;
        gubg::neural::Network<Float> network_;
        Floats states_;
        size_t input_, bias_, output_;
        float input_value_ = 0.0f;
        Floats weights_;
        size_t weight_;
        Floats outputs_;
    };
} 

#endif
