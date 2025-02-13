#include "processing/filter/butterworth.h"

std::pair<std::size_t, std::size_t> dustear::processing::filter::Butterworth::get_cut_off_frequencies() const noexcept {
    return {center_frequency - bandwith, center_frequency + bandwith};
}

void dustear::processing::filter::Butterworth::set_cut_off_frequencies(const std::pair<std::size_t, std::size_t> &new_cut_off_frequencies) noexcept {
    bandwith = new_cut_off_frequencies.second - new_cut_off_frequencies.first;
    center_frequency = std::sqrtf(new_cut_off_frequencies.first * new_cut_off_frequencies.second);
    band_pass.setup(SAMPLE_RATE, center_frequency, bandwith);
}

void dustear::processing::filter::Butterworth::set_order(const int order) noexcept {
    this->order = order;
    band_pass.setup(order, SAMPLE_RATE, center_frequency, bandwith);
}

int dustear::processing::filter::Butterworth::get_order() const noexcept {
    return order;
}

void dustear::processing::filter::Butterworth::filter(std::vector<double>& data) noexcept {
    for (auto& sample: data) {
        sample = band_pass.filter(sample);
    }
}
