#ifndef HEADER_app_codec_lp_Writer_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Writer_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <gubg/bit/oor/Codec.hpp>
#include <gubg/bit/gr/Sequence.hpp>
#include <gubg/bit/Writer.hpp>
#include <gubg/Range_macro.hpp>
#include <gubg/hr.hpp>
#include <gubg/mss.hpp>
#include <algorithm>
#include <numeric>
#include <cmath>

namespace app { namespace codec { namespace lp { 

    class Writer: public Interface
    {
    public:
        virtual ~Writer()
        {
            if (!costs_.empty())
            {
                const auto total_cost = std::accumulate(RANGE(costs_), 0.0);
                std::cout << "Total cost: " << (unsigned int)total_cost << std::endl;
                const auto total_upperbound = sample_count_*16;
                std::cout << "Upperbound cost: " << total_upperbound << " " << total_cost/total_upperbound << std::endl;
                std::cout << "Average cost: " << total_cost/costs_.size() << std::endl;
            }
        }
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);

            block_size_ = md.block_size;

            std::optional<std::string> filepath;
            {
                kv::Parser parser;
                MSS(parser.on("fp", [&](auto v){filepath = v; return true;}));
                MSS(parser.on_other([&](auto k, auto v){ MSS_BEGIN(bool); MSS(v.empty()); filepath = k; MSS_END(); }));
                MSS(parser(kvs));
            }

            if (filepath)
            {
                fo_.open(*filepath, std::ios::binary);
                MSS(fo_.is_open());
            }

            MSS_END();
        }

        bool operator()(const Block &input_block, Block &output_block) override
        {
            MSS_BEGIN(bool);

            sample_count_ += block_size_;

            MSS(input_block.size() == 3);
            const auto &coeffs = input_block[0];
            const auto &seeds = input_block[1];
            const auto &errors = input_block[2];

            bit_writer_.clear();
            bit_writer_.uint(coeffs.size(), 5);
            bit_writer_.uint(seeds.size(), 5);
            {
                const auto size = errors.size();
                errors_i_.resize(size);
                for (auto ix = 0u; ix < size; ++ix)
                    errors_i_[ix] = errors[ix];
                std::cout << errors[0] << std::endl;
#if 0
                gubg::bit::oor::Metadata md;
                gubg::bit::oor::Codec<int> cdc;
                cdc.find_optimal_metadata(md, errors_i_.data(), errors_i_.size());
                std::cout << "max_bw: " << md.max_bw << " min_bw: " << md.min_bw << std::endl;
                bit_writer_.uint(size, 12);
                bit_writer_.uint(md.max_bw, 5);
                bit_writer_.uint(md.min_bw, 5);
                cdc.encode(bit_writer_, md, errors_i_.data(), errors_i_.size());
#else
                gubg::bit::gr::Sequence<gubg::bit::gr::Type::Normal> cdc;
                cdc.encode(bit_writer_, size);
                cdc.encode(bit_writer_, errors_i_.data(), errors_i_.size());
#endif
            }
            bit_writer_.to_bytes(bytes_);
            fo_.write((const char *)bytes_.data(), bytes_.size());

            auto mm_coeffs = std::minmax_element(RANGE(coeffs));
            auto mm_errors = std::minmax_element(RANGE(errors));

            std::cout << "Coeffs: " << gubg::hr(coeffs) << std::endl;
            std::cout << "Errors: " << *mm_errors.first << ", " << *mm_errors.second << std::endl;
            const auto cost = std::accumulate(RANGE(errors), 0.0, [](auto &sum, auto error){return sum += std::log2(std::abs(error)+1+1);});
            costs_.push_back(cost);
            std::cout << "Cost: " << cost << std::endl;

            output_block = input_block;

            MSS_END();
        }

    private:
        Model model_;
        std::list<float> costs_;
        unsigned int block_size_ = 0;
        unsigned int sample_count_ = 0;
        gubg::bit::Writer bit_writer_;
        std::vector<int> errors_i_;
        std::vector<std::uint8_t> bytes_;
        std::ofstream fo_;
    };

} } } 

#endif
