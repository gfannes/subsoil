#include "nn/Mode.hpp"
#include "nn/IO.hpp"
#include "gubg/mss.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <vector>

class App
{
public:
    App()
    {
        update_();
    }
    bool imgui()
    {
        MSS_BEGIN(bool);
        ImGui::ShowDemoWindow();
        if (ImGui::Combo("Select mode", &item_, items_.data(), items_.size()))
            update_();
        if (mode_)
            MSS(mode_->imgui());
        MSS_END();
    }
    void run()
    {
        sf::RenderWindow window(sf::VideoMode(640, 480), "");
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
                ImGui::Begin("Neural Network");

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
    void update_()
    {
        switch (item_)
        {
            case 1: mode_.reset(new nn::IO); break;
        }
    }

    nn::Mode::Ptr mode_;
    int item_ = 1;
    std::vector<const char *> items_ = {"<mode>", "NN IO"};
};

int main()
{
    App app;
    app.run();
    return 0;
}
