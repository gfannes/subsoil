#ifndef HEADER_app_codec_lp_Writer_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Writer_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
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
    };

} } } 

#endif
