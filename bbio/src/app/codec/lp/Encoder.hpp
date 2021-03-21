#ifndef HEADER_app_codec_lp_Encoder_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Encoder_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <app/codec/lp/Model.hpp>
#include <gubg/mss.hpp>
#include <optional>

namespace app { namespace codec { namespace lp { 

    class Encoder: public Interface
    {
    public:
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);

            std::optional<unsigned int> order;
            {
                kv::Parser parser;
                MSS(parser.on("order", [&](auto v){order = std::stoul(v); return true;}));
                MSS(parser(kvs));
            }
            MSS(!!order);

            model_.emplace(*order);

            MSS_END();
        }

        bool operator()(const Block &input_block, Block &output_block) override
        {
            MSS_BEGIN(bool);

            MSS(input_block.size() == 1);
            const auto &input_vec = input_block[0];

            MSS(!!model_);
            auto &model = *model_;

            MSS(model.learn(input_vec));

            output_block.resize(3);
            auto &coeffs = output_block[0];
            auto &seeds = output_block[1];
            auto &errors = output_block[2];

            coeffs = model.coefficients;

            MSS(model.order() <= input_vec.size());
            seeds.assign(input_vec.begin(), input_vec.begin()+model.order());

            MSS(model.prediction_errors(errors, input_vec));

            MSS_END();
        }

    private:
        std::optional<Model> model_;
    };

} } } 

#endif
