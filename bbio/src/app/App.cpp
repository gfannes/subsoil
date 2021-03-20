#include <app/App.hpp>
#include <app/transform/WavIO.hpp>
#include <gubg/ml/adadelta/Minimizer.hpp>
#include <gubg/ml/fwd/Minimizer.hpp>
#include <gubg/Strange.hpp>
#include <gubg/mss.hpp>
#include <cmath>
#include <numeric>
#include <iomanip>

namespace app { 

    bool App::run()
    {
        MSS_BEGIN(bool, "");

        MSS(create_metadata_());
        MSS(create_transforms_());

        MSS(!!metadata_.max_block_count);
        const auto block_count = std::min(*metadata_.max_block_count, options.block_count.value_or(*metadata_.max_block_count));
        if (options.verbose_level >= 1)
            std::cout << "I will process " << block_count << " blocks" << std::endl;

        transform::Block input_block, output_block;
        for (auto bix = 0u; bix < block_count; ++bix)
        {
            if (options.verbose_level >= 1)
                std::cout << "Processing block " << bix << std::endl;

            for (auto &ptr: transforms_)
            {
                MSS(ptr->transform(input_block, output_block));
                std::swap(input_block, output_block);
            }
        }

        MSS_END();
    }

    //Privates
    bool App::create_metadata_()
    {
        MSS_BEGIN(bool);

        MSS(options.block_size > 0, std::cout << "Error: block size cannot be zero" << std::endl);
        metadata_.block_size = options.block_size;

        MSS_END();
    }
    bool App::create_transforms_()
    {
        MSS_BEGIN(bool);

        transforms_.clear();
        
        for (const auto &str: options.transforms)
        {
            gubg::Strange strange{str};

            std::string type;
            MSS(strange.pop_until(type, ':'));

            transform::Interface::Ptr ptr;
            if (false) {}
            else if (type == "wavin") ptr.reset(new transform::WavInput{});
            else if (type == "wavout") ptr.reset(new transform::WavOutput{});
            else MSS(false, std::cout << "Error: unknown type \"" << type << "\"" << std::endl);

            MSS(!!ptr);
            MSS(ptr->setup(strange.str(), metadata_));

            transforms_.push_back(ptr);
        }

        MSS_END();
    }
} 
