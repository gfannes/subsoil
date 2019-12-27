#ifndef HEADER_quiz_View_hpp_ALREADY_INCLUDED
#define HEADER_quiz_View_hpp_ALREADY_INCLUDED

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
        };

        View(unsigned int width, unsigned int height)
        : window_(sf::VideoMode{width, height}, "Quiz master")
        {
            auto load_sound = [&](auto &sounds, auto prefix, auto ix)
            {
                std::ostringstream oss;
                oss << prefix << std::setw(2) << std::setfill('0')<< ix << ".ogg";
                auto fn = oss.str();
                SoundPtr sound;
                if (load_sound_(sound, fn))
                    sounds.push_back(sound);
            };
            for (auto i = 0; i < 18; ++i)
            {
                load_sound(ok_sounds_, "quiz/media/ok/", i);
                load_sound(ko_sounds_, "quiz/media/ko/", i);
            }
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

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

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
                if (event.type == sf::Event::Closed)
                    events_->ve_close();
            }
            MSS_END();
        }
        bool draw_(std::string &error)
        {
            MSS_BEGIN(bool);
            window_.clear();
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

        Events *events_ = nullptr;
        sf::RenderWindow window_;

        std::map<std::string, sf::SoundBuffer> sound_buffers_;
        Sounds ok_sounds_;
        Sounds ko_sounds_;
        Sounds playing_sounds_;
    };

} 

#endif
