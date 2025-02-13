#pragma once

#include <vector>
#include <complex>

#include "helper/fft.h"
#include "lsq/functions.h"
#include "processing/parameter_calculation/least_squares.h"

namespace dustear::processing::parameter_calculation {
    double esimtate_amplitude(const std::vector<double>& pulse) noexcept;

    class EstimateFrequency {
    public:
        explicit EstimateFrequency(std::size_t size) noexcept;

        std::array<double, 4> operator()(const std::vector<double>& data) noexcept;

    private:
        signal_processing::FFT<double> fft;

        using LSQ = LeastSquares<6, lsq::functions::BeatSpectrumResidual>;

        LSQ lsq{1};
    };
}
