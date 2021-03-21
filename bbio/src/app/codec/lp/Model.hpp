#ifndef HEADER_app_codec_lp_Model_hpp_ALREADY_INCLUDED
#define HEADER_app_codec_lp_Model_hpp_ALREADY_INCLUDED

#include <gubg/ml/fwd/Minimizer.hpp>
#include <gubg/ml/scg/Minimizer.hpp>
#include <gubg/Range_macro.hpp>
#include <cmath>
#include <numeric>
#include <vector>
#include <iomanip>

namespace app { namespace codec { namespace lp { 

    class Model
    {
    public:
        using Vector = std::vector<float>;

        Vector coefficients;

        Model() {}
        Model(unsigned int order): coefficients(order, 1.0/order) {}
        Model(const Vector &coeffs): coefficients(RANGE(coeffs)) {}

        unsigned int order() const {return coefficients.size();}

        template <typename It>
        static float predict(It sample_it, const Vector &coeffs)
        {
            float sum = 0.0;
            for (auto ix = 0u; ix < coeffs.size(); ++ix)
                sum += coeffs[ix]*sample_it[ix];
            return sum;
        }
        template <typename It>
        float predict(It sample_it) const {return predict(sample_it, coefficients);}

        template <typename Errors>
        bool prediction_errors(Errors &errors, const Vector &signal) const
        {
            MSS_BEGIN(bool);

            auto input = signal.begin();
            auto actual = input+order();

            errors.resize(signal.size()-order());

            for (auto error = errors.begin(); error != errors.end(); ++input, ++actual, ++error)
            {
                const auto prediction = predict(input);
                *error = *actual - prediction;
            }

            MSS_END();
        }

        bool learn(const Vector &signal)
        {
            MSS_BEGIN(bool, "");

            gubg::ml::fwd::Minimizer<float> fwd_minimizer;
            gubg::ml::scg::Minimizer<float> scg_minimizer;
            auto nr_iter = 1000000;
            nr_iter = 100;
            /* nr_iter = 261; */
            for (auto ix = 0u; ix < nr_iter; ++ix)
            {
                switch (1)
                {
                    case 0:
                        {
                            auto my_compute_gradient = [&](auto &gradient){
                                return compute_gradient_(gradient, coefficients, signal);
                            };
                            MSS(fwd_minimizer.update(coefficients, my_compute_gradient));
                        }
                        break;
                    case 1:
                        {
                            auto my_compute_gradient = [&](auto &gradient, const auto &coeffs){
                                return compute_gradient_(gradient, coeffs, signal);
                            };
                            auto my_compute_cost = [&](const auto &coeffs){
                                return compute_cost_(coeffs, signal);
                            };
                            MSS(scg_minimizer.update(coefficients, my_compute_gradient, my_compute_cost));
                        }
                        break;
                }
            }

            L(gubg::hr(coefficients));

            MSS_END();
        }

    private:
        float compute_cost_(const Vector &coeffs, const Vector &signal)
        {
            float cost = 0;

            auto input = signal.begin();
            auto actual = input+order();
            auto actual_end = signal.end();
            for (; actual != actual_end; ++input, ++actual)
            {
                const auto diff = predict(input, coeffs) - *actual;

#if 0
#else
                //Quadratic cost
                cost += diff*diff;
#endif
            }

            return cost/(signal.size() - order());
        }

        template <typename Gradient>
        bool compute_gradient_(Gradient &gradient, const Vector &coeffs, const Vector &signal)
        {
            MSS_BEGIN(bool);

            MSS(signal.size() > order());

            const auto size = coeffs.size();
            gradient.resize(size);
            std::fill(RANGE(gradient), 0.0);

            auto input = signal.begin();
            auto actual = input+order();
            auto actual_end = signal.end();
            for (; actual != actual_end; ++input, ++actual)
            {
                const auto diff = predict(input, coeffs) - *actual;

#if 0
                const auto prefix = (diff >= 0 ? 1.0 : -1.0)/(std::abs(diff)+1);
                L(C(diff)C(prefix));
                /* const auto prefix = (diff >= 0 ? 1.0 : -1.0); */
                /* const auto prefix = diff; */
#else
                //Quadratic cost
                const auto prefix = 2.0*diff;
#endif

                for (auto ix = 0u; ix < size; ++ix)
                    gradient[ix] += prefix*input[ix];
            }

            const auto factor = 1.0/(signal.size()-order());
            for (auto &g: gradient)
                g *= factor;

            MSS_END();
        }
    };

} } } 

#endif
