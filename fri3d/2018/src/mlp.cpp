#include "gubg/imgui/SelectFile.hpp"
#include "gubg/mlp/Structure.hpp"
#include "gubg/mlp/Parameters.hpp"
#include "gubg/neural/Simulator.hpp"
#include "gubg/neural/setup.hpp"
#include "gubg/s11n.hpp"
#include "gubg/mss.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "gubg/std/filesystem.hpp"
#include "gubg/std/optional.hpp"
#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <cmath>

class Linear
{
public:
    Linear(float from, float to): b_(to/2), a_(b_/from) {}
    float operator()(float v) const {return a_*v+b_;}
private:
    float b_;
    float a_;
};

class Transform
{
public:
    template <typename Window>
    Transform(const Window &wnd, float x, float y): x_(x, wnd.getSize().x), y_(y, wnd.getSize().y) {}
    sf::Vector2f operator()(float x, float y) const {return sf::Vector2f(x_(x), y_(y));}
private:
    Linear x_, y_;
};

class Line
{
public:
    Line(float width, sf::Color color): width_(width), color_(color) {}

    Line &point(const sf::Vector2f &pos)
    {
        auto cur = pos;
        if (prev_)
        {
            const auto &prev = *prev_;
            auto diff = cur - prev;
            auto norm = sf::Vector2f(-diff.y, diff.x);
            norm *= width_/std::sqrt(norm.x*norm.x + norm.y*norm.y);
            vertices_.push_back(sf::Vertex(prev+norm, color_));
            vertices_.push_back(sf::Vertex(prev-norm, color_));
            vertices_.push_back(sf::Vertex(cur+norm, color_));
            vertices_.push_back(sf::Vertex(cur-norm, color_));
        }
        prev_ = cur;
        return *this;
    }

    template <typename Window>
    void draw(Window &wnd) const
    {
        wnd.draw(vertices_.data(), vertices_.size(), sf::TriangleStrip);
    }

private:
    float width_;
    sf::Color color_;
    std::optional<sf::Vector2f> prev_;
    std::vector<sf::Vertex> vertices_;
};

class App
{
public:
    bool imgui()
    {
        MSS_BEGIN(bool);
        /* ImGui::ShowDemoWindow(); */

        messages_.resize(0);

        {
            const auto &str = error_.str();
            if (!str.empty())
                ImGui::Text("Error: %s", str.c_str());
        }

        if (gubg::imgui::select_file("MLP structure", structure_fn_))
        {
            std::cout << "Selected structure file " << structure_fn_ << std::endl;
            info_.reset();
            error_.str("");
            gubg::mlp::Structure structure;
            MSS(gubg::s11n::read_object_from_file(structure_fn_, ":mlp.Structure", structure), error_ << "Could not read MLP structure from " << structure_fn_.string());
            info_.emplace();
            info_->structure = structure;
            info_->parameters.setup_from(info_->structure);
            gubg::neural::setup(info_->simulator, info_->structure, info_->first_input, info_->bias, info_->first_output);
            info_->weights.resize(info_->simulator.nr_weights());
            info_->states.resize(info_->simulator.nr_states());
            info_->states[info_->bias] = 1.0;
        }
        ImGui::SameLine();
        ImGui::Text(structure_fn_.string().c_str());

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

            nn_.goc();
            {
                auto &wnd = io_.goc();
                Transform t(wnd, 3,1);
                io_.line(1, sf::Color::Red, [&](auto &line){ line.point(t(-3.0,0.0)).point(t(3.0,0.0)); });
                io_.line(1, sf::Color::Red, [&](auto &line){ line.point(t(0.0,-1.0)).point(t(0.0,1.0)); });
                gubg::neural::setup(info.weights, info.parameters);
                auto draw_io = [&](auto &line){
                    for (auto x = -3.0; x <= 3.0; x += 0.01)
                    {
                        info.states[info.first_input] = x;
                        info.simulator.forward(info.states.data(), info.weights.data());
                        const auto y = info.states[info.first_output];
                        line.point(t(x, y));
                    }
                };
                io_.line(1, sf::Color::Red, draw_io);
            }
        }

        MSS_END();
    }

    void run()
    {
        S("");

        {
            font_.emplace();
            std::string fn = std::getenv("gubg");
            fn += "/fonts/GenBasR.ttf";
            if (!font_->loadFromFile(fn))
            {
                error_ << "Could not load the font from " << fn;
                font_.reset();
            }
        }

        const bool is_hires = sf::VideoMode::getDesktopMode().width >= 3000;
        L(C(is_hires));

        const auto vm = (is_hires ? sf::VideoMode(3000, 1500) : sf::VideoMode(1600, 800));
        nn_.setup("Structure", vm.width/2, vm.height, 0.0, sf::Color(20, 0, 0), font_);
        io_.setup("Input/Output", vm.width/2, vm.height, vm.width/2, sf::Color(0, 20, 0), font_);
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
            if (nn_.valid)
                nn_.draw(window);
            if (io_.valid)
                io_.draw(window);
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

    std::ostringstream error_;

    std::filesystem::path structure_fn_;
    struct Info
    {
        gubg::mlp::Structure structure;
        gubg::mlp::Parameters parameters;
        unsigned int lix = 0;
        unsigned int nix = 0;
        gubg::neural::Simulator<float> simulator;
        size_t first_input, bias, first_output;
        std::vector<float> weights, states;
    };
    std::optional<Info> info_;

    struct Pane
    {
        bool valid = false;
        sf::Color color;
        std::string caption;
        float xpos, ypos;
        sf::Text text;
        sf::RenderTexture rt;
        sf::Sprite sprite;
        std::list<Line> lines;
        void setup(const std::string &caption, unsigned int width, unsigned int height, float xpos, sf::Color color, const std::optional<sf::Font> &font)
        {
            this->caption = caption;
            this->color = color;
            this->xpos = xpos;
            this->ypos = height;
            if (font)
            {
                text.setFont(*font);
                text.setString(caption);
                text.setCharacterSize(24);
            }
            sprite.setPosition(sf::Vector2f(xpos, ypos));
            sprite.setScale(1.0, -1.0);
            rt.create(width, height);
        }
        sf::RenderTexture &goc()
        {
            valid = true;
            rt.clear(color);
            rt.draw(text);
            return rt;
        }
        template <typename Ftor>
        void line(float width, const sf::Color &color, Ftor &&ftor)
        {
            lines.emplace_back(width, color);
            ftor(lines.back());
        }
        void draw(sf::RenderWindow &wnd)
        {
            for (const auto &line: lines)
                line.draw(rt);
            sprite.setTexture(rt.getTexture());
            wnd.draw(sprite);
            valid = false;
        }
    };
    Pane nn_;
    Pane io_;

    std::optional<sf::Font> font_;
};

int main()
{
    App app;
    app.run();
    return 0;
}
