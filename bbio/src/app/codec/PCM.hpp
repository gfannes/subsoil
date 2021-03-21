#ifndef HEADER_app_codec_WavIO_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_WavIO_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <app/kv/Parser.hpp>
#include <gubg/wav/Reader.hpp>
#include <gubg/wav/Writer.hpp>
#include <gubg/mss.hpp>

namespace app { namespace codec { namespace pcm { 

    class Reader: public Interface
    {
    public:
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);

            std::optional<std::string> filepath;
            {
                kv::Parser parser;
                MSS(parser.on("fp", [&](auto v){filepath = v; return true;}));
                MSS(parser.on("begin", [&](auto v){block_index_ = std::stoul(v); return true;}));
                MSS(parser.on("end", [&](auto v){block_index_end_ = std::stoul(v); return true;}));
                MSS(parser.on_other([&](auto k, auto v){ MSS_BEGIN(bool); MSS(v.empty()); filepath = k; MSS_END(); }));
                MSS(parser(kvs));
            }

            MSS(!!filepath);

            MSS(reader_.load(*filepath, md.block_size));
            MSS(!!reader_.format);
            md.sample_rate = reader_.format->sample_rate;
            md.bits_per_sample = reader_.format->bits_per_sample;
            if (!block_index_end_)
                block_index_end_ = reader_.block_count();
            block_index_end_ = std::min(*block_index_end_, reader_.block_count());
            MSS(block_index_ <= *block_index_end_);
            md.max_block_count = *block_index_end_-block_index_;

            MSS_END();
        }

        bool operator()(const Block &input_block, Block &output_block) override
        {
            MSS_BEGIN(bool);

            MSS(reader_.valid());
            MSS(!!block_index_end_);
            MSS(block_index_ < *block_index_end_);

            output_block.resize(1);
            auto &vec = output_block[0];
            vec.resize(reader_.block_size());
            MSS(reader_.read_block(block_index_, vec.data()));
            ++block_index_;

            MSS_END();
        }
    private:
        gubg::wav::Reader reader_;
        unsigned int block_index_ = 0;
        std::optional<unsigned int> block_index_end_;
    };

    class Writer: public Interface
    {
    public:
        bool setup(const kv::KeyValues &kvs, Metadata &md) override
        {
            MSS_BEGIN(bool);

            std::optional<std::string> filepath;
            {
                kv::Parser parser;
                MSS(parser.on("fp", [&](auto v){filepath = v; return true;}));
                MSS(parser.on_other([&](auto k, auto v){ MSS_BEGIN(bool); MSS(v.empty()); filepath = k; MSS_END(); }));
                MSS(parser(kvs));
            }
            MSS(!!filepath);

            MSS(writer_.open(*filepath, md.block_size, 1, md.sample_rate, md.bits_per_sample));

            MSS_END();
        }

        bool operator()(const Block &input_block, Block &output_block) override
        {
            MSS_BEGIN(bool);

            MSS(writer_.valid());

            MSS(input_block.size() == 1);
            const auto &vec = input_block[0];
            MSS(vec.size() == writer_.block_size());

            MSS(writer_.write_mono(vec.data()));

            //There might be a codec after us that wants to continue the chain
            output_block = input_block;

            MSS_END();
        }
    private:
        gubg::wav::Writer writer_;
    };

} } } 

#endif
