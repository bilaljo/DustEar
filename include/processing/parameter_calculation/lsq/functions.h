#pragma once

#include "processing/parameter_calculation/least_squares.h"

#include <memory>

namespace dustear::processing::parameter_calculation::lsq::functions {
    constexpr double SAMPLE_FREQUENCY = 2e6;

    double sample_to_time(int sample) noexcept;

    double get_average(std::vector<double> data) noexcept;

    struct Scales {
        double amplitude = 0.220;

        double frequency = 1.026;

        double time_constant = 2.345;
    };

    class DampedPulse {
    public:
        double operator()(const double* x, int sample) const noexcept;

    public:
        Scales scales;

        double time_constant;
    };

    class DampedPulseResidual {
    public:
        double operator()(const double* x, int sample) const noexcept;

        void set_target(const std::shared_ptr<const std::vector<double>>& target) noexcept;

        std::shared_ptr<const std::vector<double>> target;

    public:
        double offset{};

        DampedPulse damped_pulse;
    };

    class DampedPulseGradient {
    public:
        double operator()(const double* x, int i, int sample) const noexcept;

    public:
        Scales scales;

        double time_constant;
    };

    class BeatSpectrumResidual {
    public:
        double operator()(const double* x, int sample) const noexcept;

        std::shared_ptr<const std::vector<double>> target;

        double frequency_offset = 80e3;
    };

}
