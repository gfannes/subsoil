#include <app/App.hpp>
#include <gubg/wav/Reader.hpp>
#include <gubg/wav/Writer.hpp>
#include <gubg/mss.hpp>
#include <vector>

namespace app { 
    bool App::run()
    {
        MSS_BEGIN(bool, "");

        MSS(options.block_size > 0, std::cout << "Error: block size cannot be zero" << std::endl);

        gubg::wav::Reader reader;
        if (options.input_filepath)
            MSS(reader.load(*options.input_filepath, options.block_size));

        auto block_count = options.block_count.value_or(-1);
        if (block_count < 0)
            block_count = reader.block_count();
        if (options.verbose_level >= 1)
            std::cout << "I will process " << block_count << " blocks" << std::endl;

        gubg::wav::Writer writer;
        if (options.output_filepath)
            MSS(writer.open(*options.output_filepath, options.block_size, 1, reader.format->sample_rate, reader.format->bits_per_sample));

        std::vector<float> input_data(options.block_size);
        std::vector<float> output_data(options.block_size);
        for (auto bix = 0u; bix < block_count; ++bix)
        {
            if (options.verbose_level >= 1)
                std::cout << "Processing block " << bix << std::endl;

            if (reader.valid())
                MSS(reader.read_block(bix, input_data.data()));

            output_data[0] = input_data[0];
            output_data[0] = 0.0;
            for (auto ix = 1u; ix < input_data.size(); ++ix)
                output_data[ix] = input_data[ix]-input_data[ix-1];

            if (writer.valid())
                MSS(writer.write_mono(output_data.data()));
        }

        MSS_END();
    }
} 
