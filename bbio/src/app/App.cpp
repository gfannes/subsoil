#include <app/App.hpp>
#include <gubg/wav/Reader.hpp>
#include <gubg/wav/Writer.hpp>
#include <gubg/ml/adadelta/Minimizer.hpp>
#include <gubg/ml/fwd/Minimizer.hpp>
#include <gubg/mss.hpp>
#include <vector>
#include <cmath>
#include <numeric>
#include <iomanip>

namespace app { 

    class Model
    {
    public:
        Model(unsigned int order): coefficients_(order, 1.0/order), gradient_(order) {}

        unsigned int order() const {return coefficients_.size();}

        template <typename T>
        double predict(const T *ptr) const
        {
            double sum = 0.0;
            for (auto ix = 0u; ix < coefficients_.size(); ++ix)
                sum += coefficients_[ix]*ptr[ix];
            return sum;
        }

        double cost_diff(double diff) const
        {
            return std::log2(std::abs(diff)+1.0);
        }
        template <typename T>
        double cost(const T *ptr) const
        {
            const auto diff = predict(ptr)-ptr[order()];
            return cost_diff(diff);
        }

        template <typename Data>
        double avg_cost(const Data &data) const
        {
            double avg = 0.0;
            if (order() < data.size())
            {
                for (auto ix = order(); ix < data.size(); ++ix)
                    avg += cost(data.data()+ix-order());
                avg /= data.size()-order();
            }
            return avg;
        }
        template <typename Data>
        double max_cost(const Data &data) const
        {
            double max = 0.0;
            if (order() < data.size())
            {
                for (auto ix = order(); ix < data.size(); ++ix)
                    max = std::max<double>(max, cost(data.data()+ix-order()));
            }
            return max;
        }
        template <typename Diffs>
        double avg_cost_diffs(const Diffs &diffs) const
        {
            return std::accumulate(diffs.begin(), diffs.end(), 0.0, [&](auto acc, auto diff){return acc+cost_diff(diff);})/diffs.size();
        }
        template <typename Diffs>
        double max_cost_diffs(const Diffs &diffs) const
        {
            return std::accumulate(diffs.begin(), diffs.end(), 0.0, [&](auto acc, auto diff){return std::max<float>(acc,cost_diff(diff));});
        }

        template <typename Data>
        bool learn(const Data &data)
        {
            MSS_BEGIN(bool);
            gubg::ml::adadelta::Minimizer adadelta_minimizer{order(), 0.9, 0.9};
            gubg::ml::fwd::Minimizer<double> fwd_minimizer;
            auto nr_iter = 1000000;
            nr_iter = 10000;
            for (auto ix = 0u; ix < nr_iter; ++ix)
            {
#if 0
                MSS(compute_gradient_(gradient_, data));
                MSS(adadelta_minimizer.update(coefficients_, gradient_));
#else
                auto my_compute_gradient = [&](auto &gradient){
                    return compute_gradient_(gradient, data);
                };
                MSS(fwd_minimizer.update(coefficients_, my_compute_gradient));
#endif
                for (auto &c: coefficients_)
                    std::cout << std::setw(10) << c << "\t";
                std::cout << std::endl;
            }
            MSS_END();
        }

    private:
        template <typename Gradient, typename Data>
        bool compute_gradient_(Gradient &gradient, const Data &data)
        {
            MSS_BEGIN(bool, "");

            MSS(data.size() > order());
            std::fill(gradient.begin(), gradient.end(), 0.0);
            for (auto ix = order(); ix < data.size(); ++ix)
            {
                const auto *x = &data[ix-order()];
                const auto diff = predict(x)-data[ix];
#if 1
                const auto prefix = (diff >= 0 ? 1.0 : -1.0)/(std::abs(diff)+1);
                L(C(diff)C(prefix));
                /* const auto prefix = (diff >= 0 ? 1.0 : -1.0); */
                /* const auto prefix = diff; */
#else
                const auto prefix = 2.0*diff;
#endif
                for (auto i = 0u; i < gradient.size(); ++i)
                    gradient[i] += prefix*x[i];
            }

            const auto factor = 1.0/(data.size()-order());
            for (auto &g: gradient)
            {
                g *= factor;
                /* std::cout << g << "\t"; */
            }
            std::cout << std::endl;

            MSS_END();
        }

        std::vector<float> coefficients_;
        std::vector<float> gradient_;
    };

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
        Model model{3};
        double total_avg_cost = 0.0;
        for (auto bix = 0u; bix < block_count; ++bix)
        {
            if (options.verbose_level >= 1)
                std::cout << "Processing block " << bix << std::endl;

            if (reader.valid())
                MSS(reader.read_block(bix, input_data.data()));

            std::cout << "Input: " << model.avg_cost_diffs(input_data) << " " << model.max_cost_diffs(input_data) << std::endl;

            MSS(model.learn(input_data));

            output_data[0] = input_data[0];
            for (auto ix = 0u; ix < model.order(); ++ix)
            {
                output_data[ix] = input_data[ix];
                output_data[ix] = 0.0;
            }
            for (auto ix = model.order(); ix < input_data.size(); ++ix)
                output_data[ix] = input_data[ix]-model.predict(&input_data[ix-model.order()]);

            const auto my_avg_cost = model.avg_cost_diffs(output_data);
            total_avg_cost += my_avg_cost;
            std::cout << "Output: " << my_avg_cost << " " << model.max_cost_diffs(output_data) << std::endl;

            if (writer.valid())
                MSS(writer.write_mono(output_data.data()));
        }

        std::cout << "Total cost " << total_avg_cost/block_count << std::endl;

        MSS_END();
    }
} 
