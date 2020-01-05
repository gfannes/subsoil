#ifndef HEADER_quiz_View_hpp_ALREADY_INCLUDED
#define HEADER_quiz_View_hpp_ALREADY_INCLUDED

#include <gubg/sfml/layout.hpp>
#include <gubg/sfml/FontMgr.hpp>
#include <gubg/sfml/Label.hpp>
#include <gubg/mss.hpp>
#include <gubg/prob/Uniform.hpp>
#include <gubg/serial/Endpoint.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <sstream>
#include <iomanip>
#include <optional>

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
        : window_(sf::VideoMode{width, height}, "Quiz master", sf::Style::Default)
        {
            if (!construct_(ctor_error_) && ctor_error_.empty())
                ctor_error_ = "Unknown construction error for View";
        }

        void inject_events_receiver(Events *events) {events_ = events;}
        void set_position(const std::pair<int,int> pos)
        {
            window_.setPosition(sf::Vector2i{pos.first, pos.second});
        }

        bool connect_to_buttons(const std::string &device, const gubg::serial::Settings &settings)
        {
            MSS_BEGIN(bool);
            buttons_.emplace();
            MSS(buttons_->open(device.c_str(), settings));
            MSS_END();
        }
        void close() {window_.close();}
        void play_sound_answer_was_correct(bool b)
        {
            Sounds &sounds = b ? ok_sounds_ : ko_sounds_;
            auto &sound = gubg::prob::select_uniform(sounds);
            stop_(playing_sounds_);
            play_(sound);
        }
        void set_background_color(sf::Color color)
        {
            background_color_ = color;
        }
        void set_title(const std::string &title) { title_.set_string(title); }
        void set_description(const std::string &description) { description_.set_string(description); }
        void set_answer_team(const std::string &team)
        {
            std::string str;
            if (!team.empty())
                str = std::string("Answer: ")+team;
            answer_team_.set_string(str);
        }
        void set_answer_order(const std::string &order)
        {
            std::string str;
            if (!order.empty())
                str = std::string("Next: ")+order;
            answer_order_.set_string(str);
        }
        void set_score(const std::string &score) { score_.set_string(score); }
        bool set_image(const std::string &fn)
        {
            MSS_BEGIN(bool);
            image_.show = !fn.empty();
            if (!fn.empty())
                MSS(image_.load_from_file(fn));
            MSS_END();
        }
        bool load_music(const std::string &fn, float offset = 0.0f, float pitch = 1.0f)
        {
            MSS_BEGIN(bool);
            if (fn.empty())
            {
                if (music_)
                {
                    music_->stop();
                    music_.reset();
                }
            }
            else
            {
                music_.emplace();
                MSS(music_->openFromFile(fn), music_.reset());
                music_->setPlayingOffset(sf::seconds(offset));
                music_->setPitch(pitch);
            }
            MSS_END();
        }
        bool play_music(bool b, bool restart = false)
        {
            MSS_BEGIN(bool);
            if (!music_)
                return true;
            auto &music = music_.value();
            if (b)
                switch (music.getStatus())
                {
                    case sf::SoundSource::Stopped:
                    case sf::SoundSource::Paused:
                        music.play();
                        break;
                    case sf::SoundSource::Playing:
                        if (restart)
                            music.play();
                        break;
                }
            else
                music.pause();
            MSS_END();
        }

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
        using Rectu = sf::Rect<unsigned int>;

        bool process_events_(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(!!events_, error = "View.events_ not set");

            {
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
                                l_case(Space, ' ');
                                l_case(Escape, 27);
#define unroll(x) x(Q) x(X) x(V) x(A) x(B) x(C) x(D) x(E) x(R)
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
            }

            if (buttons_)
            {
                std::array<char, 16> buffer;
                size_t offset = 0;
                MSS(buttons_->receive(offset, buffer.data(), buffer.size()));
                for (auto i = 0; i < offset; ++i)
                    events_->ve_key(buffer[i]);
            }

            MSS_END();
        }
        bool draw_(std::string &error)
        {
            MSS_BEGIN(bool);
            window_.clear(background_color_);
            if (image_.show)
                window_.draw(image_.sprite);
            title_.draw(window_);
            description_.draw(window_);
            score_.draw(window_);
            answer_team_.draw(window_);
            answer_order_.draw(window_);
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
            /* const auto fn = "quiz/media/font/GenR102.TTF"; */
            const auto fn = "quiz/media/font/GenBasR.ttf";
            MSS(font_mgr_.load_font(fn, "myfont"), error = std::string("Could not load font from ")+fn);
            MSS_END();
        }
        bool setup_texts_(std::string &error)
        {
            MSS_BEGIN(bool);
            auto font = font_mgr_.get_font("myfont");
            title_.set_font(font);
            description_.set_font(font);
            score_.set_font(font);
            answer_team_.set_font(font);
            answer_order_.set_font(font);
            MSS_END();
        }
        bool layout_elements_(std::string &error)
        {
            MSS_BEGIN(bool);

            using namespace gubg::sfml;

            Rectu r{sf::Vector2u{}, window_.getSize()};

            pop_border(r, 20);
            const auto height = r.height;
            const auto width = r.width;

            {
                auto rr = pop_top(r, 0.1*height);
                set_bb(score_, rr);
                rr.top -= 0.2*rr.height;
                set_bb(title_, rr);
            }
            {
                auto rr = pop_top(r, 0.8*height);
                image_.rect = rr;
                image_.rect.height += 0.1*height;
                {
                    auto rrr = pop_left(rr, 0.7*rr.width);
                    pop_top(rrr, 0.1*rrr.height);
                    set_bb(description_, pop_top(rrr, 0.2*rrr.height));
                }
            }
            {
                auto rr = pop_top(r, 0.1*height);
                set_bb(answer_team_,  pop_left(rr, 0.3*width));
                pop_left(rr, 0.3*width);
                set_bb(answer_order_, pop_left(rr, 0.4*width));
            }

            MSS_END();
        }

        std::string ctor_error_;

        Events *events_ = nullptr;
        sf::RenderWindow window_;
        sf::Color background_color_ = sf::Color::Black;
        gubg::sfml::FontMgr font_mgr_;
        sf::Font font_;

        gubg::sfml::Label title_;
        gubg::sfml::Label description_;
        gubg::sfml::Label score_;
        gubg::sfml::Label answer_team_;
        gubg::sfml::Label answer_order_;

        struct Image
        {
            bool show = false;
            Rectu rect;
            sf::Texture texture;
            sf::Sprite sprite{texture};
            bool load_from_file(const std::string &fn)
            {
                MSS_BEGIN(bool);
                MSS(texture.loadFromFile(fn));
                texture.setSmooth(true);
                sprite.setTexture(texture, true);
                {
                    const auto size = texture.getSize();
                    const double fw = double(rect.width)/size.x;
                    const double fh = double(rect.height)/size.y;
                    const double f = std::min(fw, fh);
                    sprite.setScale(f,f);
                }
                {
                    const auto size = sprite.getGlobalBounds();
                    const auto x = rect.left+0.5*(rect.width-size.width);
                    const auto y = rect.top+0.5*(rect.height-size.height);
                    sprite.setPosition(x, y);
                }
                MSS_END();
            }
        };
        Image image_;

        std::map<std::string, sf::SoundBuffer> sound_buffers_;
        Sounds ok_sounds_;
        Sounds ko_sounds_;
        Sounds playing_sounds_;
        std::optional<sf::Music> music_;

        std::optional<gubg::serial::Endpoint> buttons_;
    };

} 

#endif
