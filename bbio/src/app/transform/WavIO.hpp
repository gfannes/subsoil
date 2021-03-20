#ifndef HEADER_app_transform_WavIO_hpp_ALREADY_INCLUDED
#define HEADER_app_transform_WavIO_hpp_ALREADY_INCLUDED

#include <app/transform/Interface.hpp>
#include <gubg/wav/Reader.hpp>
#include <gubg/wav/Writer.hpp>
#include <gubg/mss.hpp>

namespace app { namespace transform { 

    class WavInput: public Interface
    {
    public:
        bool setup(const std::string &str, Metadata &md) override
        {
            MSS_BEGIN(bool);

            MSS(reader_.load(str, md.block_size));
            MSS(!!reader_.format);
            md.sample_rate = reader_.format->sample_rate;
            md.bits_per_sample = reader_.format->bits_per_sample;
            md.max_block_count = reader_.block_count();

            MSS_END();
        }

        bool transform(const Block &input, Block &output) override
        {
            MSS_BEGIN(bool);

            MSS(reader_.valid());

            output.resize(1);
            auto &vec = output[0];
            vec.resize(reader_.block_size());
            MSS(reader_.read_block(block_index_, vec.data()));
            ++block_index_;

            MSS_END();
        }
    private:
        gubg::wav::Reader reader_;
        unsigned int block_index_ = 0;
    };

    class WavOutput: public Interface
    {
    public:
        bool setup(const std::string &str, Metadata &md) override
        {
            MSS_BEGIN(bool);
            MSS(writer_.open(str, md.block_size, 1, md.sample_rate, md.bits_per_sample));
            MSS_END();
        }

        bool transform(const Block &input, Block &output) override
        {
            MSS_BEGIN(bool);

            MSS(writer_.valid());

            MSS(input.size() == 1);
            const auto &vec = input[0];
            MSS(vec.size() == writer_.block_size());

            MSS(writer_.write_mono(vec.data()));

            MSS_END();
        }
    private:
        gubg::wav::Writer writer_;
    };

} } 

#endif
