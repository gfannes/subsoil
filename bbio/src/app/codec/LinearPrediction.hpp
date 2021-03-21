#ifndef HEADER_app_codec_LinearPrediction_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_LinearPrediction_hpp_ALREADY_INCLUDED

#include <app/codec/Interface.hpp>
#include <gubg/ml/adadelta/Minimizer.hpp>
#include <gubg/ml/fwd/Minimizer.hpp>
#include <gubg/mss.hpp>
#include <cmath>
#include <numeric>
#include <iomanip>

namespace app { namespace codec { namespace lp { 

    class Difference: public Interface
    {
    public:
        bool setup(const KeyValues &kv, Metadata &md) override
        {
            MSS_BEGIN(bool);
            MSS_END();
        }

        bool operator()(const Block &input, Block &output) override
        {
            MSS_BEGIN(bool);

            output = input;
            for (auto &vec: output)
                for (auto &val: vec)
                {
                    const auto orig = val;
                    val = orig-prev_;
                    prev_ = orig;
                }

            /* std::cout << "Input: " << model.avg_cost_diffs(input_data) << " " << model.max_cost_diffs(input_data) << std::endl; */

            /* MSS(model.learn(input_data)); */

            /* output_data[0] = input_data[0]; */
            /* for (auto ix = 0u; ix < model.order(); ++ix) */
            /* { */
            /*     output_data[ix] = input_data[ix]; */
            /*     output_data[ix] = 0.0; */
            /* } */
            /* for (auto ix = model.order(); ix < input_data.size(); ++ix) */
            /*     output_data[ix] = input_data[ix]-model.predict(&input_data[ix-model.order()]); */

            /* const auto my_avg_cost = model.avg_cost_diffs(output_data); */
            /* total_avg_cost += my_avg_cost; */
            /* std::cout << "Output: " << my_avg_cost << " " << model.max_cost_diffs(output_data) << std::endl; */

            MSS_END();
        }

    private:
        float prev_ = 0;
    };

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

} } } 

#endif
