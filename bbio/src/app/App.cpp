#include <app/App.hpp>
#include <app/codec/PCM.hpp>
#include <app/codec/LinearPrediction.hpp>
#include <gubg/Strange.hpp>
#include <gubg/mss.hpp>

namespace app { 

    bool App::run()
    {
        MSS_BEGIN(bool, "");

        MSS(create_metadata_());
        MSS(create_codecs_());

        MSS(!!metadata_.max_block_count);
        const auto block_count = std::min(*metadata_.max_block_count, options.block_count.value_or(*metadata_.max_block_count));
        if (options.verbose_level >= 1)
            std::cout << "I will process " << block_count << " blocks" << std::endl;

        codec::Block input_block, output_block;
        for (auto bix = 0u; bix < block_count; ++bix)
        {
            if (options.verbose_level >= 1)
                std::cout << "Processing block " << bix << std::endl;

            for (auto &ptr: codecs_)
            {
                auto &codec = *ptr;
                MSS(codec(input_block, output_block));
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
    bool App::create_codecs_()
    {
        MSS_BEGIN(bool);

        codecs_.clear();
        
        for (const auto &str: options.codecs)
        {
            gubg::Strange strange{str};

            auto pop_kv = [&](auto &kv){
                MSS_BEGIN(bool);
                gubg::Strange kv_strange;
                MSS(strange.pop_until(kv_strange, ';') || strange.pop_all(kv_strange));
                MSS(kv_strange.pop_until(kv.first, '=') || kv_strange.pop_all(kv.first));
                kv_strange.pop_all(kv.second);
                MSS_END();
            };

            codec::KeyValue type_kv;
            MSS(pop_kv(type_kv), std::cout << "Error: could not parse the codec type" << std::endl);

            codec::KeyValues kvs;
            for (codec::KeyValue kv; pop_kv(kv); )
                kvs.push_back(kv);

            codec::Interface::Ptr ptr;
            if (false) {}
            else if (type_kv.first == "pcm")
            {
                if (false) {}
                else if (type_kv.second == "reader") ptr.reset(new codec::pcm::Reader{});
                else if (type_kv.second == "writer") ptr.reset(new codec::pcm::Writer{});
                else MSS(false, std::cout << "Error: unknown pcm operation \"" << type_kv.second << "\"" << std::endl);
            }
            else if (type_kv.first == "lp")
            {
                if (false) {}
                else if (type_kv.second == "diff") ptr.reset(new codec::lp::Difference{});
                else MSS(false, std::cout << "Error: unknown lp operation \"" << type_kv.second << "\"" << std::endl);
            }
            else MSS(false, std::cout << "Error: unknown codec family \"" << type_kv.first << "\"" << std::endl);

            MSS(!!ptr);
            MSS(ptr->setup(kvs, metadata_));

            codecs_.push_back(ptr);
        }

        MSS_END();
    }
} 
