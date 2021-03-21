#ifndef HEADER_app_codec_lp_Decoder_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Decoder_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <app/codec/lp/Model.hpp>
#include <gubg/mss.hpp>

namespace app { namespace codec { namespace lp { 

    class Decoder: public Interface
    {
    public:
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);
            MSS_END();
        }

        bool operator()(const Block &input_block, Block &output_block) override
        {
            MSS_BEGIN(bool);

            MSS(input_block.size() == 3);
            const auto &coeffs = input_block[0];
            const auto &seeds = input_block[1];
            const auto &errors = input_block[2];

            model_.coefficients = coeffs;

            output_block.resize(1);
            output_block[0].resize(seeds.size()+errors.size());
            auto output = output_block[0].begin();
            auto input = output;
            auto error = errors.begin();
            auto output_end = output_block[0].end();

            for (auto ix = 0u; ix < model_.order() && output != output_end; ++ix, ++output)
                *output = seeds[ix];

            for (; output != output_end; ++output, ++input, ++error)
                *output = model_.predict(input) + *error;

            MSS_END();
        }

    private:
        Model model_;
    };

} } } 

#endif
