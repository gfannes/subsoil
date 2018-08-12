#include "gubg/imgui/SelectFile.hpp"
#include "gubg/mlp/Structure.hpp"
#include "gubg/s11n.hpp"
#include "gubg/mss.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "gubg/std/filesystem.hpp"
#include <iostream>
#include <optional>

class App
{
public:
    bool imgui()
    {
        MSS_BEGIN(bool);
        /* ImGui::ShowDemoWindow(); */
        if (gubg::imgui::select_file("MLP structure", structure_fn_))
        {
            std::cout << "Selected structure file " << structure_fn_ << std::endl;
            structure_.emplace();
            structure_error_.clear();
            if (!gubg::s11n::read_object_from_file(structure_fn_, ":mlp.Structure", *structure_))
            {
                structure_error_ = "Error: Could not read MLP structure from " + structure_fn_.string();
                structure_.reset();
            }
        }
        ImGui::SameLine();
        ImGui::Text(structure_fn_.c_str());
        if (!structure_error_.empty())
            ImGui::Text(structure_error_.c_str());
        MSS_END();
    }

    void run()
    {
        sf::RenderWindow window(sf::VideoMode(1600, 800), "");
        window.setVerticalSyncEnabled(true);
        ImGui::SFML::Init(window);

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

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }

        ImGui::SFML::Shutdown();
    }
private:
    std::filesystem::path structure_fn_;
    std::string structure_error_;
    std::optional<gubg::mlp::Structure> structure_;
};

int main()
{
    App app;
    app.run();
    return 0;
}
