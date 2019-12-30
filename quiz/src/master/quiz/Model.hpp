#ifndef HEADER_quiz_Model_hpp_ALREADY_INCLUDED
#define HEADER_quiz_Model_hpp_ALREADY_INCLUDED

#include <quiz/Question.hpp>
#include <gubg/mss.hpp>
#include <vector>

namespace quiz { 

    class Model
    {
    public:
        Model()
        {
            if (!construct_(ctor_error_) && ctor_error_.empty())
                ctor_error_ = "Unknown construction error for Model";
        }

        bool operator()(std::string &error)
        {
            MSS_BEGIN(bool);
            MSS(ctor_error_.empty(), error = ctor_error_);
            MSS_END();
        }

        const Question *get_question(unsigned int ix) const
        {
            if (ix >= questions_.size())
                return nullptr;
            return &questions_[ix];
        }

    private:
        bool construct_(std::string &error)
        {
            MSS_BEGIN(bool);
            Question q;
            q.description = "How many bitches can you fit in a Tesla?";
            questions_.push_back(q);
            MSS_END();
        }

        std::string ctor_error_;
        std::vector<Question> questions_;
    };

} 

#endif
