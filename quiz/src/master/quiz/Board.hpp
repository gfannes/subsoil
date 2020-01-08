#ifndef HEADER_quiz_Board_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Board_hpp_ALREADY_INCLUDED

#include <vector>
#include <map>
#include <string>

namespace quiz { 

    struct Board
    {
        struct Category
        {
            std::string name;
            std::vector<char> status;
        };
        std::map<char, Category> group__categories;
    };

} 

#endif
