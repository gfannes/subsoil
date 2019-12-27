#ifndef HEADER_quiz_View_hpp_ALREADY_INCLUDED
#define HEADER_quiz_View_hpp_ALREADY_INCLUDED

#include <gubg/mss.hpp>
#include <SFML/Graphics.hpp>

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
        }

        void inject_events_receiver(Events *events) {events_ = events;}
        void close() {window_.close();}

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);

            MSS(window_.isOpen(), error = "window is closed");

            MSS(process_events_(error));

            window_.clear();
            window_.display();
            MSS_END();
        }

    private:
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

        Events *events_ = nullptr;
        sf::RenderWindow window_;
    };

} 

#endif
