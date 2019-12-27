#ifndef HEADER_quiz_Model_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Model_hpp_ALREADY_INCLUDED

#include <gubg/mss.hpp>

namespace quiz { 

    class Model
    {
    public:
        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS_END();
        }

    private:
    };

} 

#endif
