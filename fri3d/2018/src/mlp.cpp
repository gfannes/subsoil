#include "gubg/imgui/SelectFile.hpp"
#include "gubg/mlp/Structure.hpp"
#include "gubg/mlp/Parameters.hpp"
#include "gubg/neural/Simulator.hpp"
#include "gubg/neural/setup.hpp"
#include "gubg/neural/Trainer.hpp"
#include "gubg/data/Set.hpp"
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
#include <deque>
#include <cmath>
#include <random>

std::mt19937 rng;

class Linear
{
public:
    Linear(double from, double to): b_(to/2), a_(b_/from) {}
    double operator()(double v) const {return a_*v+b_;}
private:
    double b_;
    double a_;
};

class Transform
{
public:
    template <typename Window>
    Transform(const Window &wnd, double x, double y): x_(x, wnd.getSize().x), y_(y, wnd.getSize().y) {}
    sf::Vector2f operator()(double x, double y) const {return sf::Vector2f(x_(x), y_(y));}
private:
    Linear x_, y_;
};

class Line
{
public:
    Line(double width, sf::Color color): width_(width), color_(color) {}

    Line &point(const sf::Vector2f &pos)
    {
        auto cur = pos;
        if (prev_)
        {
            const auto &prev = *prev_;
            auto diff = cur - prev;
            auto norm = sf::Vector2f(-diff.y, diff.x);
            norm *= (float)width_/std::sqrt(norm.x*norm.x + norm.y*norm.y);
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
    double width_;
    sf::Color color_;
    std::optional<sf::Vector2f> prev_;
    std::vector<sf::Vertex> vertices_;
};

class Dot
{
public:
    Dot(double width, sf::Color color, sf::Vector2f pos)
    {
        pos.x += width; pos.y += width;     vertices_[0] = sf::Vertex(pos, color);
        pos.x -= 2*width;                   vertices_[1] = sf::Vertex(pos, color);
        pos.x += 2*width; pos.y -= 2*width; vertices_[2] = sf::Vertex(pos, color);
        pos.x -= 2*width;                   vertices_[3] = sf::Vertex(pos, color);
    }

    template <typename Window>
    void draw(Window &wnd) const
    {
        wnd.draw(vertices_.data(), vertices_.size(), sf::TriangleStrip);
    }
private:
    std::array<sf::Vertex, 4> vertices_;
};

using Data = gubg::data::Set<double>;

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
            if (learn_)
                learn_->trainer.reset();
            model_.reset();
            error_.str("");
            gubg::mlp::Structure structure;
            MSS(gubg::s11n::read_object_from_file(structure_fn_, ":mlp.Structure", structure), error_ << "Could not read MLP structure from " << structure_fn_.string());
            model_.emplace();
            model_->structure = structure;
            model_->parameters.setup_from(model_->structure);
        }
        ImGui::SameLine();
        ImGui::Text(structure_fn_.string().c_str());

        if (model_)
        {
            auto &model = *model_;

            {
                model.cost_stddev = std::max(model.cost_stddev, 0.01);
                float cost_stddev = model.cost_stddev;
                if (ImGui::SliderFloat("Cost stddev", &cost_stddev, 0.01, 0.5))
                {
                    model.cost_stddev = cost_stddev;
                    model.simulator.reset();
                }
            }
            {
                model.weights_stddev = std::max(model.weights_stddev, 0.1);
                float weights_stddev = model.weights_stddev;
                if (ImGui::SliderFloat("Weights stddev", &weights_stddev, 0.1, 10.0))
                    model.weights_stddev = weights_stddev;
            }

            if (!model.simulator)
                model.simulator.emplace();

            auto &simulator = *model.simulator;

            //Create the simulator, weights and states
            {
                MSS(gubg::neural::setup(simulator, model.structure, model.input, model.bias, model.output));
                const auto nr_outputs = model.structure.nr_outputs();
                model.wanted_output = simulator.add_external(nr_outputs);
                std::vector<size_t> wanted_outputs(nr_outputs); std::iota(RANGE(wanted_outputs), model.wanted_output);
                std::vector<size_t> actual_outputs(nr_outputs); std::iota(RANGE(actual_outputs), model.output);
                MSS(simulator.add_loglikelihood(wanted_outputs, actual_outputs, model.loglikelihood, model.cost_stddev));
                model.weights.resize(simulator.nr_weights());
                model.states.resize(simulator.nr_states());
                model.states[model.bias] = 1.0;
            }

            ImGui::Text("Nr states: %d, nr weights: %d", simulator.nr_states(), simulator.nr_weights());
            ImGui::Separator();

            if (ImGui::Button("Randomize weights"))
            {
                model.init_scg = true;
                std::normal_distribution<double> gaussian(0.0, model.weights_stddev);
                for (auto &l: model.parameters.layers)
                    for (auto &n: l.neurons)
                    {
                        for (auto &w: n.weights)
                            w = gaussian(rng);
                        n.bias = gaussian(rng);
                    }
            }
            ImGui::Separator();

            {
                auto &s = model.structure;
                for (auto lix = 0u; lix < s.layers.size(); ++lix)
                {
                    ImGui::Text(cstr_("Layer ", lix));
                    auto &layer = s.layers[lix];
                    for (auto nix = 0u; nix < layer.neurons.size(); ++nix)
                    {
                        if (ImGui::RadioButton(cstr_("L", lix, "N", nix), (model.lix == lix) && (model.nix == nix)))
                        {
                            model.lix = lix;
                            model.nix = nix;
                        }
                        ImGui::SameLine();
                    }
                    for (auto &n: layer.neurons)
                        n.weight_stddev = model.weights_stddev;
                    for (auto &n: layer.neurons)
                        n.bias_stddev = model.weights_stddev;
                    ImGui::NewLine();
                    ImGui::Separator();
                }
            }

            ImGui::Text(cstr_("Selected neuron: layer ", model.lix, " neuron ", model.nix));
            {
                auto &neuron = model.structure.layers[model.lix].neurons[model.nix];
                ImGui::Text("Transfer function: %s", to_str(neuron.transfer));
            }
            {
                auto &neuron =  model.parameters.layers[model.lix].neurons[model.nix];
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
            ImGui::Separator();

            nn_.goc();
            {
                auto &wnd = io_.goc();
                Transform t(wnd, 3,1);
                io_.line(1, sf::Color::Red, [&](auto &line){ line.point(t(-3.0,0.0)).point(t(3.0,0.0)); });
                io_.line(1, sf::Color::Red, [&](auto &line){ line.point(t(0.0,-1.0)).point(t(0.0,1.0)); });
                MSS(gubg::neural::setup(model.weights, model.parameters));
                auto draw_io = [&](auto &line){
                    for (auto x = -3.0; x <= 3.0; x += 0.01)
                    {
                        model.states[model.input] = x;
                        simulator.forward(model.states.data(), model.weights.data());
                        const auto y = model.states[model.output];
                        line.point(t(x, y));
                    }
                };
                io_.line(1, sf::Color::Red, draw_io);
            }
        }

        if (gubg::imgui::select_file("Data", data_fn_))
        {
            std::cout << "Selected data file " << data_fn_ << std::endl;
            learn_.reset();
            error_.str("");
            Data data;
            MSS(gubg::s11n::read_object_from_file(data_fn_, ":data.Set", data), error_ << "Could not read data from " << data_fn_.string());
            learn_.emplace();
            learn_->data = data;
        }
        ImGui::SameLine();
        ImGui::Text(data_fn_.string().c_str());

        if (learn_)
        {
            auto &learn = *learn_;
            ImGui::Text("Nr records: %d", learn.data.records.size());

            {
                auto &wnd = io_.goc();
                Transform t(wnd, 3,1);
                for (const auto &r: learn.data.records)
                {
                    const auto x = r.data(0);
                    const auto y = r.data(1);
                    io_.dot(3, sf::Color::Green, t(x,y));
                }
            }
        }

        if (model_ && learn_)
        {
            auto &model = *model_;
            auto &learn = *learn_;
            double data_ll = 0.0;
            for (const auto &r: learn.data.records)
            {
                const auto x = r.data(0);
                const auto y = r.data(1);
                model.states[model.input] = x;
                model.states[model.wanted_output] = y;
                model.simulator->forward(model.states.data(), model.weights.data());
                data_ll += model.states[model.loglikelihood];
            }
            data_ll /= learn.data.records.size();
            ImGui::Text("data cost: %f", (float)-data_ll);
            double weights_ll = 0.0;
            for (auto lix = 0u; lix < model.structure.layers.size(); ++lix)
            {
                for (auto nix = 0u; nix < model.structure.layers[lix].neurons.size(); ++nix)
                {
                    {
                        const auto stddev = model.structure.neuron(lix, nix).weight_stddev;
                        for (const auto weight: model.parameters.neuron(lix, nix).weights)
                            weights_ll += -0.5*(weight*weight)/(stddev*stddev);
                    }
                    {
                        const auto stddev = model.structure.neuron(lix, nix).bias_stddev;
                        const auto bias = model.parameters.neuron(lix, nix).bias;
                        weights_ll += -0.5*(bias*bias)/(stddev*stddev);
                    }
                }
            }
            ImGui::Text("weight cost: %f", (float)-weights_ll);
            ImGui::Text("total cost: %f", (float)(-data_ll-weights_ll));
            {
                auto &costs = learn.costs;
                for (auto ix = 1u; ix < costs.size(); ++ix)
                    costs[ix-1] = costs[ix];
                costs.back() = -data_ll-weights_ll;
                ImGui::PlotLines("cost", costs.data(), costs.size(), 0, nullptr, 0.0, FLT_MAX, ImVec2(0,100));
                const auto p = std::minmax_element(RANGE(costs));
                ImGui::Text("min: %f, max: %f", *p.first, *p.second);
                ImGui::SameLine();
                if (ImGui::Button("reset cost"))
                    std::fill(RANGE(costs), costs.back());
            }
            ImGui::Separator();

            if (!learn.trainer)
                learn.trainer.emplace(model.structure.nr_inputs, model.structure.nr_outputs());
            {
                auto &trainer = *learn.trainer;
                for (const auto &r: learn.data.records)
                {
                    MSS(trainer.add(r.fields[0], r.fields[1]));
                }
                MSS(trainer.set(&model.simulator.value(), model.input, model.output));
                trainer.add_fixed_input(model.bias, 1.0);

                if (ImGui::RadioButton("No learning", learn.algo == Algo::NoLearn))
                    learn.algo = Algo::NoLearn;
                ImGui::SameLine();
                if (ImGui::RadioButton("Steepest descent", learn.algo == Algo::SteepestDescent))
                    learn.algo = Algo::SteepestDescent;
                ImGui::SameLine();
                if (ImGui::RadioButton("Scaled conjugate gradient", learn.algo == Algo::SCG))
                    learn.algo = Algo::SCG;
                ImGui::SameLine();
                if (ImGui::RadioButton("ADAM", learn.algo == Algo::Adam))
                    learn.algo = Algo::Adam;

                switch (learn.algo)
                {
                    case Algo::NoLearn:
                        break;
                    case Algo::SteepestDescent:
                        {
                            learn.step = std::max(learn.step, 0.0001f);
                            ImGui::SliderFloat("Steepest descent step", &learn.step, 0.0001, 0.3);

                            trainer.set_max_gradient_norm(10.0);
                            double newlp;
                            MSS(trainer.train_sd(newlp, model.weights.data(), model.cost_stddev, model.weights_stddev, learn.step));
                            ImGui::Text("New LP: %f", (float)newlp);
                        }
                        break;
                    case Algo::SCG:
                        {
                            if (model.init_scg)
                                trainer.init_scg();
                            model.init_scg = false;
                            double newlp;
                            MSS(trainer.train_scg(newlp, model.weights.data(), model.cost_stddev, model.weights_stddev, 1));
                            ImGui::Text("New LP: %f", (float)newlp);
                        }
                        break;
                    case Algo::Adam:
                        {
                            if (model.init_scg)
                            {
                                gubg::neural::Trainer<double>::AdamParams adam;
                                adam.alpha = 0.01;
                                adam.beta1 = 0.9;
                                trainer.init_adam(adam);
                            }
                            model.init_scg = false;
                            double newlp;
                            MSS(trainer.train_adam(newlp, model.weights.data(), model.cost_stddev, model.weights_stddev));
                            ImGui::Text("New LP: %f", (float)newlp);
                        }
                        break;
                }
                MSS(gubg::neural::copy_weights(model.parameters, model.weights));
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

                if (!imgui())
                    window.close();

                ImGui::End();
            }

            const auto bg_color = (model_ ? sf::Color(0, 128, 128) : sf::Color(0, 0, 0));
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
    template <typename A>
    const char *cstr_(const A &a)
    {
        oss_.str(""); oss_ << a;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }
    template <typename A, typename B>
    const char *cstr_(const A &a, const B &b)
    {
        oss_.str(""); oss_ << a << b;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }
    template <typename A, typename B, typename C>
    const char *cstr_(const A &a, const B &b, const C &c)
    {
        oss_.str(""); oss_ << a << b << c;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }
    template <typename A, typename B, typename C, typename D>
    const char *cstr_(const A &a, const B &b, const C &c, const D &d)
    {
        oss_.str(""); oss_ << a << b << c << d;
        messages_.emplace_back(oss_.str());
        return messages_.back().c_str();
    }

    std::ostringstream error_;

    std::filesystem::path structure_fn_;
    struct Model
    {
        gubg::mlp::Structure structure;
        gubg::mlp::Parameters parameters;
        unsigned int lix = 0;
        unsigned int nix = 0;
        std::optional<gubg::neural::Simulator<double>> simulator;
        size_t input, bias, output;
        std::vector<double> weights, states;
        double weights_stddev = 3.0;
        double cost_stddev = 0.1;
        size_t wanted_output, loglikelihood;
        bool init_scg = true;
    };
    std::optional<Model> model_;

    std::filesystem::path data_fn_;
    enum class Algo {NoLearn, SteepestDescent, SCG, Adam};
    struct Learn
    {
        Data data;
        std::array<float, 1000> costs{};
        std::optional<gubg::neural::Trainer<double>> trainer;
        Algo algo = Algo::NoLearn;
        float step = 0.01;
    };
    std::optional<Learn> learn_;

    struct Pane
    {
        bool valid = false;
        sf::Color color;
        std::string caption;
        double xpos, ypos;
        sf::Text text;
        sf::RenderTexture rt;
        sf::Sprite sprite;
        std::list<Line> lines;
        std::list<Dot> dots;
        void setup(const std::string &caption, unsigned int width, unsigned int height, double xpos, sf::Color color, const std::optional<sf::Font> &font)
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
            rt.create(width, height);
        }
        sf::RenderTexture &goc()
        {
            if (!valid)
            {
                valid = true;
                rt.clear(color);
                rt.draw(text);
                lines.clear();
                dots.clear();
            }
            return rt;
        }
        template <typename Ftor>
        void line(double width, const sf::Color &color, Ftor &&ftor)
        {
            lines.emplace_back(width, color);
            ftor(lines.back());
        }
        void dot(double width, const sf::Color &color, const sf::Vector2f &pos)
        {
            dots.emplace_back(width, color, pos);
        }
        void draw(sf::RenderWindow &wnd)
        {
            for (const auto &line: lines)
                line.draw(rt);
            for (const auto &dot: dots)
                dot.draw(rt);
            sprite.setPosition(sf::Vector2f(xpos, ypos));
            sprite.setScale(1.0, -1.0);
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
