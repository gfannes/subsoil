#include "gubg/imgui/SelectFile.hpp"
#include "gubg/mlp/Structure.hpp"
#include "gubg/mlp/Parameters.hpp"
#include "gubg/s11n.hpp"
#include "gubg/mss.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "gubg/std/filesystem.hpp"
#include <iostream>
#include <sstream>
#include <optional>

class App
{
public:
    bool imgui()
    {
        MSS_BEGIN(bool);
        /* ImGui::ShowDemoWindow(); */

        messages_.resize(0);

        if (!error_.empty())
            ImGui::Text(error_.c_str());

        if (gubg::imgui::select_file("MLP structure", structure_fn_))
        {
            std::cout << "Selected structure file " << structure_fn_ << std::endl;
            info_.reset();
            error_.clear();
            gubg::mlp::Structure structure;
            MSS(gubg::s11n::read_object_from_file(structure_fn_, ":mlp.Structure", structure), error_ = "Error: Could not read MLP structure from " + structure_fn_.string());
            info_.emplace();
            info_->structure = structure;
            info_->parameters.setup_from(info_->structure);
        }
        ImGui::SameLine();
        ImGui::Text(structure_fn_.c_str());

        if (info_)
        {
            auto &info = *info_;
            if (ImGui::BeginCombo("layers", cstr_("Layer ", info.lix)))
            {
                const auto &s = info.structure;
                for (auto lix = 0u; lix < s.layers.size(); ++lix)
                    if (ImGui::Selectable(cstr_("Layer ", lix), lix == info.lix))
                    {
                        info.lix = lix;
                        info.nix = 0;
                    }
                ImGui::EndCombo();
            }
            if (ImGui::BeginCombo("neurons", cstr_("Neuron ", info.nix)))
            {
                const auto &s = info.structure;
                for (auto nix = 0u; nix < s.layers[info.lix].neurons.size(); ++nix)
                    if (ImGui::Selectable(cstr_("Neuron ", nix), nix == info.nix))
                        info.nix = nix;
                ImGui::EndCombo();
            }

            {
                const auto &neuron = info.structure.layers[info.lix].neurons[info.nix];
                ImGui::Text("Transfer function: %s", to_str(neuron.transfer));
            }
            {
                auto &neuron =  info.parameters.layers[info.lix].neurons[info.nix];
                for (auto wix = 0u; wix < neuron.weights.size(); ++wix)
                {
                    float weight = neuron.weights[wix];
                    ImGui::SliderFloat(cstr_("Weight ", wix), &weight, -3.0, 3.0);
                    neuron.weights[wix] = weight;
                }
                {
                    float bias = neuron.bias;
                    ImGui::SliderFloat(cstr_("Bias"), &bias, -3.0, 3.0);
                    neuron.bias = bias;
                }
            }
        }

        MSS_END();
    }

    void run()
    {
        const bool is_hires = sf::VideoMode::getDesktopMode().width >= 3000;

        const auto vm = (is_hires ? sf::VideoMode(3000, 1500) : sf::VideoMode(1600, 800));
        sf::RenderWindow window(vm, "");
        window.setVerticalSyncEnabled(true);
        ImGui::SFML::Init(window);
        if (is_hires)
            ImGui::GetIO().FontGlobalScale = 2.0;

        window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

        sf::Clock clock;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            ImGui::SFML::Update(window, clock.restart());

            {
                ImGui::Begin("Neural Network: Multi-Layer Perceptron");

                imgui();

                ImGui::End();
            }

            const auto bg_color = (info_ ? sf::Color(0, 128, 128) : sf::Color(0, 0, 0));
            window.clear(bg_color);
            sf::CircleShape n(50);
            window.draw(n);
            ImGui::SFML::Render(window);
            window.display();
        }

        ImGui::SFML::Shutdown();
    }
private:
    std::vector<std::string> messages_;
    std::ostringstream oss_;
    const char *cstr_(const std::string &prefix)
    {
        oss_.str(""); oss_ << prefix;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }
    const char *cstr_(const std::string &prefix, unsigned int ix)
    {
        oss_.str(""); oss_ << prefix << ix;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }

    std::string error_;

    std::filesystem::path structure_fn_;
    struct Info
    {
        gubg::mlp::Structure structure;
        gubg::mlp::Parameters parameters;
        unsigned int lix = 0;
        unsigned int nix = 0;
    };
    std::optional<Info> info_;
};

int main()
{
    App app;
    app.run();
    return 0;
}
