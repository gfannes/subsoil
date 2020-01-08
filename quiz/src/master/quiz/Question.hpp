#ifndef HEADER_quiz_Question_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Question_hpp_ALREADY_INCLUDED

#include <string>
#include <vector>
#include <optional>

namespace quiz { 

    class Question
    {
    public:
        std::string title;
        std::vector<std::string> descriptions;
        std::string image_fn;
        std::string music_fn;
        float offset = 0.0f;
        float pitch = 1.0f;
        char group = '?';
        unsigned int ix = 0;
        bool done = false;

    private:
    };

} 

#endif
