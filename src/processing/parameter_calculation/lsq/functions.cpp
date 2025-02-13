#include "processing/parameter_calculation/lsq/functions.h"

#include <cmath>
#include <numbers>
#include <numeric>

double dustear::processing::parameter_calculation::lsq::functions::sample_to_time(const int sample) noexcept {
    return static_cast<double>(sample) / SAMPLE_FREQUENCY;
}

double dustear::processing::parameter_calculation::lsq::functions::DampedPulse::operator()(const double *x, const int sample) const noexcept {
    const double t = sample_to_time(sample);
    const double time_shift =  t - x[2] * 1e-6;
    const double w_1 = 2.0 * std::numbers::pi_v<double> * x[1] * 1e3;
    const double w_2 = 2.0 * std::numbers::pi_v<double> * x[1] * 1e3 * scales.frequency;
    const double pulse = x[0] * std::exp(-time_shift / (time_constant * 1e-6)) * std::sin(w_1 * time_shift);
    const double second_pulse = x[0] * scales.amplitude * std::exp(-time_shift / (time_constant * 1e-6 * scales.time_constant)) * std::sin(w_2 * time_shift);
    return pulse + second_pulse;
}

void dustear::processing::parameter_calculation::lsq::functions::DampedPulseResidual::set_target(const std::shared_ptr<const std::vector<double>>& target) noexcept {
    this->target = target;
    offset = std::reduce(std::execution::par_unseq, target->begin(), target->end(), 0.0) / static_cast<double>(target->size());
}

double dustear::processing::parameter_calculation::lsq::functions::DampedPulseResidual::operator()(const double *x, const int sample) const noexcept {
    return damped_pulse(x, sample) - (*target)[sample] + offset;
}

double dustear::processing::parameter_calculation::lsq::functions::DampedPulseGradient::operator()(const double* x, const int i, const int sample) const noexcept {
    const double t = sample_to_time(sample);
    const double time_shift =  t - x[2] * 1e-6;
    const double exp1 = std::exp(-time_shift / (time_constant * 1e-6));
    const double sin1 = std::sin(2.0 * std::numbers::pi_v<double> * x[1] * 1e3 * time_shift);
    const double cos1 = std::cos(2.0 * std::numbers::pi_v<double> * x[1] * 1e3 * time_shift);
    const double exp2 = std::exp(-time_shift / (time_constant * 1e-6 * scales.time_constant));
    const double sin2 = std::sin(2.0 * std::numbers::pi_v<double> * x[1] * 1e3 * time_shift * scales.frequency);
    const double cos2 = std::cos(2.0 * std::numbers::pi_v<double> * x[1] * 1e3 * time_shift * scales.frequency);
    switch (i) {
        case 0:
            return -exp1 * sin1 - scales.amplitude * exp2 * sin2;
        /*
        case 1:
            return -x[0] * time_shift / std::pow(x[1], 2) * exp1 * sin1 * 1e6
                   -x[0] * scales.amplitude / scales.time_constant * time_shift / std::pow(x[1], 2) * exp2 * sin2 * 1e6;
        */
        case 1:
            return -x[0] * exp1 * 2.0 * std::numbers::pi_v<double> * time_shift * cos1 * 1e3
                   -x[0] * scales.amplitude * scales.frequency * exp2 * 2.0 * std::numbers::pi_v<double> * time_shift * cos2 * 1e3;
        case 2:
            return (x[0] * sin1 * exp1 / time_constant * 1e6
                    - 2 * std::numbers::pi_v<double> * x[0] * x[1] * exp1 * cos1 * 1e3
                    + x[0] * scales.amplitude / scales.time_constant * sin2 * exp2 / time_constant * 1e6
                    - 2 * std::numbers::pi_v<double> * x[0] * x[1] * scales.amplitude * scales.frequency * exp2 * cos2 * 1e3) * 1e-6;
        default:
            return 0;
    }
}

double dustear::processing::parameter_calculation::lsq::functions::BeatSpectrumResidual::operator()(const double* x, const int sample) const noexcept {
    using namespace std::complex_literals;
    const double f = sample / SAMPLE_FREQUENCY + frequency_offset;
    const double f0 = x[0] * 1e3;
    const double f1 = x[1] * 1e3;
    const double gamma1 = 1 / x[2] * 1e6;
    const double gamma2 = 1 / x[3] * 1e6;
    const double scaling1 = x[4] * std::pow(gamma1 * f0, 2);
    const double scaling2 = x[5] * std::pow(gamma2 * f1, 2);
    const auto pulse_1 = scaling1 / (-std::pow(f, 2) + 1.0i * f * gamma1 + std::pow(f0, 2));
    const auto pulse_2 = scaling2 / (-std::pow(f, 2) + 1.0i * f * gamma2 + std::pow(f1, 2));
    return std::abs(pulse_1 + pulse_2) - (*target)[sample];
}
