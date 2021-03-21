#ifndef HEADER_app_codec_lp_Difference_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Difference_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <gubg/mss.hpp>

namespace app { namespace codec { namespace lp { 

    class Difference: public Interface
    {
    public:
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);
            MSS_END();
        }

        bool operator()(const Block &input, Block &output) override
        {
            MSS_BEGIN(bool);

            output = input;
            for (auto &vec: output)
                for (auto &val: vec)
                {
                    const auto orig = val;
                    val = orig-prev_;
                    prev_ = orig;
                }

            MSS_END();
        }

    private:
        float prev_ = 0;
    };

} } } 

#endif
