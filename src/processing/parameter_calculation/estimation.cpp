#include "processing/parameter_calculation/estimation.h"

#include <algorithm>

#include "processing/parameter_calculation/least_squares.h"


double dustear::processing::parameter_calculation::esimtate_amplitude(const std::vector<double>& pulse) noexcept {
    const double maximum = *std::ranges::max_element(pulse);
    const double minimum = *std::ranges::min_element(pulse);
    return (maximum - minimum) / 2;
}

dustear::processing::parameter_calculation::EstimateFrequency::EstimateFrequency(const std::size_t size) noexcept:
fft(signal_processing::FFT<double>(size))
{
}

std::array<double, 4> dustear::processing::parameter_calculation::EstimateFrequency::operator()(const std::vector<double> &data) noexcept {
    const auto max = std::ranges::max_element(data);
    //const auto argmax = static_cast<double>(std::distance(data.begin(), max)) / 2e6 * 1e-3 + 80;
    fft.rfft(data);
    // LSQ::model.target = fft.get_abs_spectrum(static_cast<int>(80e3), static_cast<int>(100e3));
    std::array x{92.0, 1.015 * 92.0, 100.0, 300.0, *max, 0.3 * *max};
    lsq.fit(x);
    return {x[0], x[1], x[2], x[3]};
}
