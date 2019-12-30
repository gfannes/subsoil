#ifndef HEADER_quiz_View_hpp_ALREADY_INCLUDED
#define HEADER_quiz_View_hpp_ALREADY_INCLUDED

#include <gubg/sfml/layout.hpp>
#include <gubg/mss.hpp>
#include <gubg/prob/Uniform.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <sstream>
#include <iomanip>

namespace quiz { 

    class View
    {
    public:
        class Events
        {
        public:
            virtual void ve_close() = 0;
            virtual void ve_key(char) = 0;
        };

        View(unsigned int width, unsigned int height)
        : window_(sf::VideoMode{width, height}, "Quiz master")
        {
            if (!construct_(ctor_error_) && ctor_error_.empty())
                ctor_error_ = "Unknown construction error for View";
        }

        void inject_events_receiver(Events *events) {events_ = events;}
        void close() {window_.close();}
        void answer_was_correct(bool b)
        {
            Sounds &sounds = b ? ok_sounds_ : ko_sounds_;
            auto &sound = gubg::prob::select_uniform(sounds);
            stop_(playing_sounds_);
            play_(sound);
        }
        void set_description(const std::string &descr) { description_.setString(descr); }
        void set_answer_order(const std::string &order) { answer_order_.setString(order); }

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(ctor_error_.empty(), error = ctor_error_);

            MSS(window_.isOpen(), error = "window is closed");

            MSS(process_events_(error));

            MSS(draw_(error));

            MSS_END();
        }

    private:
        using SoundPtr = std::shared_ptr<sf::Sound>;
        using Sounds = std::vector<SoundPtr>;

        bool process_events_(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS(!!events_, error = "View.events_ not set");
            sf::Event event;
            while (window_.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        events_->ve_close();
                        break;
                    case sf::Event::KeyReleased:
                        switch (event.key.code)
                        {
#define l_case(key,ch) case sf::Keyboard::Key::key: events_->ve_key(ch); break
#define unroll(x) x(Q) x(X) x(V) x(A) x(B) x(C) x(D) x(E)
#define x(n) l_case(n,#n[0]+'a'-'A');
                            unroll(x)
#undef x
#undef unroll

#define unroll(x) x(0) x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9)
#define x(n) l_case(Num ## n,#n[0]);
                            unroll(x)
#undef x
#undef unroll
                        }
                        break;
                }
            }
            MSS_END();
        }
        bool draw_(std::string &error)
        {
            MSS_BEGIN(bool);
            window_.clear(sf::Color::Black);
            window_.draw(description_);
            window_.draw(answer_order_);
            window_.display();
            MSS_END();
        }
        bool load_sound_(SoundPtr &sound, const std::string &fn)
        {
            MSS_BEGIN(bool);
            auto &buffer = sound_buffers_[fn];
            MSS(buffer.loadFromFile(fn));
            sound.reset(new sf::Sound{buffer});
            MSS_END();
        }
        void stop_(Sounds &sounds)
        {
            for (auto &sound: sounds)
                sound->stop();
            sounds.clear();
        }
        void play_(SoundPtr &sound)
        {
            sound->play();
            playing_sounds_.push_back(sound);
        }

        bool construct_(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS(load_sounds_(error));
            MSS(load_font_(error));
            MSS(setup_texts_(error));
            MSS(layout_elements_(error));
            MSS_END();
        }
        bool load_sounds_(std::string &error)
        {
            MSS_BEGIN(bool);
            auto load_sound = [&](auto &sounds, auto prefix, auto ix)
            {
                MSS_BEGIN(bool);
                std::ostringstream oss;
                oss << prefix << std::setw(2) << std::setfill('0')<< ix << ".ogg";
                auto fn = oss.str();
                SoundPtr sound;
                MSS(load_sound_(sound, fn), error = std::string("Could not load sound from ")+fn);
                sounds.push_back(sound);
                MSS_END();
            };
            for (auto i = 0; i < 18; ++i)
            {
                MSS(load_sound(ok_sounds_, "quiz/media/ok/", i));
                MSS(load_sound(ko_sounds_, "quiz/media/ko/", i));
            }
            MSS_END();
        }
        bool load_font_(std::string &error)
        {
            MSS_BEGIN(bool);
            const auto fn = "quiz/media/font/GenR102.TTF";
            MSS(font_.loadFromFile(fn), error = std::string("Could not load font from ")+fn);
            MSS_END();
        }
        bool setup_texts_(std::string &error)
        {
            MSS_BEGIN(bool);
            description_.setFont(font_);
            description_.setCharacterSize(30);
            answer_order_.setFont(font_);
            answer_order_.setCharacterSize(30);
            MSS_END();
        }
        bool layout_elements_(std::string &error)
        {
            MSS_BEGIN(bool);
            sf::Rect<unsigned int> r{sf::Vector2u{}, window_.getSize()};
            gubg::sfml::pop_border(r, 20);
            {
                auto rr = gubg::sfml::pop_top(r, 200);
                gubg::sfml::set_position(description_, rr);
            }
            {
                auto rr = gubg::sfml::pop_top(r, 30);
                gubg::sfml::set_position(answer_order_, rr);
            }
            MSS_END();
        }

        std::string ctor_error_;

        Events *events_ = nullptr;
        sf::RenderWindow window_;
        sf::Font font_;

        sf::Text description_;
        sf::Text answer_order_;

        std::map<std::string, sf::SoundBuffer> sound_buffers_;
        Sounds ok_sounds_;
        Sounds ko_sounds_;
        Sounds playing_sounds_;
    };

} 

#endif
