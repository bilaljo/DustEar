#ifndef DUSTEAR_PROCESSING_FILTER_BUTTERWORTH_H
#define DUSTEAR_PROCESSING_FILTER_BUTTERWORTH_H

#include <vector>

#include <iir/Butterworth.h>

namespace dustear::processing::filter {
    class Butterworth {
    public:
        Butterworth() = default;

        void filter(std::vector<double>& data) noexcept;

        [[nodiscard]] std::pair<std::size_t, std::size_t> get_cut_off_frequencies() const noexcept;

       void set_cut_off_frequencies(const std::pair<std::size_t, std::size_t>& cut_off_frequencies) noexcept;

        void set_order(int order) noexcept;

        [[nodiscard]] int get_order() const noexcept;

    private:
        static constexpr int MAXIMUM_ORDER = 10;

        static constexpr double SAMPLE_RATE = 2e6;

        Iir::Butterworth::BandPass<MAXIMUM_ORDER> band_pass;

        double center_frequency;

        double bandwith;

        int order;
    };
}

#endif  // DUSTEAR_PROCESSING_FILTER_BUTTERWORTH_H
