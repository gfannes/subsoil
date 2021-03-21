#include <app/App.hpp>
#include <app/codec/PCM.hpp>
#include <app/codec/LinearPrediction.hpp>
#include <gubg/Strange.hpp>
#include <gubg/mss.hpp>

namespace app { 

    bool App::run()
    {
        MSS_BEGIN(bool);

        MSS(create_metadata_());
        {
            std::list<Type_KeyValues> tkvs_list;
            MSS(parse_codec_tkvs_(tkvs_list));
            MSS(create_codecs_(tkvs_list));
        }

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

    //Convert options.codecs into a std::list<Type_KeyValues>, parsing response files when encountered
    bool App::parse_codec_tkvs_(std::list<Type_KeyValues> &tkvs_list)
    {
        MSS_BEGIN(bool);

        for (const auto &str: options.codecs)
        {
            auto parse = [](Type_KeyValues &type_kvs, const std::string &str){
                MSS_BEGIN(bool);

                gubg::Strange strange{str};

                auto pop_kv = [&](auto &kv){
                    MSS_BEGIN(bool);
                    if (strange.empty())
                        return false;
                    gubg::Strange kv_strange;
                    MSS(strange.pop_until(kv_strange, ';') || strange.pop_all(kv_strange));
                    MSS(kv_strange.pop_until(kv.key, '=') || kv_strange.pop_all(kv.key));
                    kv_strange.pop_all(kv.value);
                    MSS_END();
                };

                MSS(pop_kv(type_kvs.first), std::cout << "Error: could not parse the codec type from \"" << str << "\"" << std::endl);

                for (kv::KeyValue kv; pop_kv(kv); )
                    type_kvs.second.push_back(kv);

                MSS_END();
            };

            Type_KeyValues type_kvs;
            MSS(parse(type_kvs, str));

            if (type_kvs.first.key == "fp")
            {
                const auto &filepath = type_kvs.first.value;
                std::ifstream fi{filepath};
                MSS(fi.is_open(), std::cout << "Error: could not open file \"" << filepath << "\" for reading codecs" << std::endl);
                for (std::string line; std::getline(fi, line); )
                {
                    if (line.empty() || line[0] == '#')
                        continue;

                    //For now, we do not support recursive response files
                    Type_KeyValues my_type_kvs;
                    MSS(parse(my_type_kvs, line));
                    tkvs_list.push_back(my_type_kvs);
                }
            }
            else
                tkvs_list.push_back(type_kvs);
        }

        MSS_END();
    }

    //Create the codec instances based on the std::list<Type_KeyValues>
    //All response files are already parsed
    bool App::create_codecs_(const std::list<Type_KeyValues> &tkvs_list)
    {
        MSS_BEGIN(bool);

        codecs_.clear();
        for (const auto &type_kvs: tkvs_list)
        {
            const auto &type = type_kvs.first;
            const auto &kvs = type_kvs.second;

            codec::Interface::Ptr ptr;
            if (false) {}
            else if (type.key == "pcm")
            {
                if (false) {}
                else if (type.value == "reader") ptr.reset(new codec::pcm::Reader{});
                else if (type.value == "writer") ptr.reset(new codec::pcm::Writer{});
                else MSS(false, std::cout << "Error: unknown pcm operation \"" << type.value << "\"" << std::endl);
            }
            else if (type.key == "lp")
            {
                if (false) {}
                else if (type.value == "diff") ptr.reset(new codec::lp::Difference{});
                else if (type.value == "encode") ptr.reset(new codec::lp::Encoder{});
                else MSS(false, std::cout << "Error: unknown lp operation \"" << type.value << "\"" << std::endl);
            }
            else MSS(false, std::cout << "Error: unknown codec family \"" << type << "\"" << std::endl);

            MSS(!!ptr);
            MSS(ptr->setup(kvs, metadata_));

            codecs_.push_back(ptr);
        }

        MSS_END();
    }
} 
