#pragma once

#include <array>
#include <algorithm>
#include <type_traits>
#include <execution>
#include <ranges>
#include <numbers>

#include <Eigen/Dense>

#include <cminpack.h>


namespace dustear::processing::parameter_calculation {
    template<typename T>
    concept LeastSquaresModel = requires(T t) {
        { t.target } -> std::convertible_to<std::shared_ptr<const std::vector<double>>>;

        { t(std::declval<const double*>(), std::declval<const int>()) } noexcept -> std::convertible_to<double>;
    };

    template<typename T>
    concept LeastSquaresGradient = requires(T t) {
        { t(std::declval<const double*>(), std::declval<const int>(), std::declval<const int>()) } noexcept -> std::convertible_to<double>;
    } || std::is_same_v<T, void*>;

    template<int Parameters, LeastSquaresModel Model, LeastSquaresGradient Gradient = void*>
    class LeastSquares {
    public:
        inline static Model model{};

        inline static Gradient gradient{};

    public:
        explicit LeastSquares(const int samples): samples(samples) {
            set_samples(samples);
            covariance = Eigen::MatrixXd::Zero(Parameters, Parameters);
            vector_gradient = Eigen::VectorXd::Zero(Parameters);
        }

        int fit(std::array<double, Parameters>& x)  {
            if constexpr (!std::is_same_v<Gradient, void*>) {
                return lmder1(fcn, nullptr, samples, Parameters, x.data(), fvec.data(), fjac.data(), ldfjac,
                              tolerance, int_work_array.data(), work_array.data(), lwa);
            } else {
                return lmdif1(fcn, nullptr, samples, Parameters, x.data(), fvec.data(),
                             tolerance, int_work_array.data(), work_array.data(), lwa);
            }
        }

        double get_variance_estimation(const std::array<double, Parameters>& x) {
            double residual = 0;
            for (int i = 0; i < samples; i++) {
                residual += std::pow(model(x.data(), i) - (*model.target)[i], 2);
            }
            return residual / (samples - Parameters);
        }

        void calculate_covariance(const std::array<double, Parameters>& x) {
            if constexpr (! std::is_same_v<Gradient, void*>) {
                covariance = Eigen::MatrixXd::Zero(Parameters, Parameters);
                residual = get_variance_estimation(x);
                std::ranges::iota_view indexes(0, samples);
                std::for_each(indexes.begin(), indexes.end(), [this, &x](int i) {
                    for (int j = 0; j < Parameters; j++) {
                        vector_gradient(j) = gradient(x.data(), j, i);
                    }
                    covariance += vector_gradient * vector_gradient.transpose();
                });
                covariance = residual * covariance.inverse();
            }
        }

        auto get_covariance() const noexcept {
            return covariance;
        }

        [[nodiscard]] double get_variance() const noexcept {
            return std::sqrt(residual);
        }

        void set_samples(int samples) noexcept {
            this->samples = samples;
            ldfjac = samples;
            fvec.resize(samples);
            fjac.resize(samples * Parameters);  // m x n matrix
            // See https://devernay.github.io/cminpack/lmder_.htmlnt.transp
            lwa = samples * Parameters + 5 * Parameters + samples;
            work_array.resize(lwa);
            int_work_array.resize(Parameters);
        }


    private:
        static int fcn(void*, int m, [[maybe_unused]] int n, const double *x, double *fvec, double *fjac, [[maybe_unused]] int ldfjac, int iflag)  {
            std::ranges::iota_view indexes(0, m);
            if constexpr (!std::is_same_v<Gradient, void*>) {
                if (iflag == 1) {
                    std::for_each(std::execution::unseq, indexes.begin(), indexes.end(), [&fvec, &x](int i){fvec[i] = model(x, i);});
                }
                else if (iflag == 2) {
                    std::for_each(std::execution::unseq, indexes.begin(), indexes.end(), [&fjac, &m, &x](int i) {
                        for (int k = 0; k < Parameters; k++) {
                            fjac[i + k * m] = gradient(x, k, i);
                        }
                    });
                }
            }
            return 0;
        }

        static int fcn(void*, int m, [[maybe_unused]] int n, const double *x, double *fvec, [[maybe_unused]] int iflag)  {
            std::ranges::iota_view indexes(0, m);
            std::for_each(std::execution::unseq, indexes.begin(), indexes.end(), [&fvec, &x](int i){fvec[i] = model(x, i);});
            return 0;
        }

    private:
        int samples{};

        std::vector<double> fvec;

        std::vector<double> fjac;

        std::vector<int> int_work_array;

        std::vector<double> work_array;

        Eigen::MatrixXd covariance{};

        Eigen::VectorXd vector_gradient{};

        int lwa{};

        double tolerance = 1e-8;

        int ldfjac = 1;

        int m = 1;

        double residual = 0.0;
    };
}
