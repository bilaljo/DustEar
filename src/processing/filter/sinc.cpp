#include "processing/filter/sinc.h"

#include <numbers>
#include <complex>

#include "nlohmann/json.hpp"

dustear::processing::filter::Sinc::Sinc() noexcept {
    rectangle.resize(BUFFER_SIZE / 2 + 1);
}

dustear::processing::filter::Sinc::Sinc(const std::pair<std::size_t, std::size_t>& bandwith) noexcept {
    rectangle.resize(BUFFER_SIZE / 2 + 1);
    update_bandwith(bandwith);
}

void dustear::processing::filter::Sinc::operator()(std::vector<double>& data) noexcept {
    auto spectrum = fft.rfft(data);
    std::ranges::transform(rectangle, spectrum, spectrum.begin(), std::multiplies());
    data = fft.irfft(spectrum);
}

void dustear::processing::filter::Sinc::update_bandwith(const std::pair<std::size_t, std::size_t>& bandwith) noexcept {
    std::ranges::fill(rectangle, 0);
    std::ranges::fill_n(rectangle.begin() + bandwith.first, bandwith.second - bandwith.first, 1);
}
