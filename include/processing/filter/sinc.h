#pragma once

#include <complex>
#include <vector>
#include <string>
#include <cstdint>

#include "helper/fft.h"


namespace dustear::processing::filter {
    class Sinc {
    public:
        Sinc() noexcept;

        explicit Sinc(const std::pair<std::size_t, std::size_t>& bandwith) noexcept;

        void operator()(std::vector<double>& data) noexcept;

        void update_bandwith(const std::pair<std::size_t, std::size_t>& bandwith) noexcept;

    public:
        static constexpr std::size_t BUFFER_SIZE{1 << 21};

    private:

        alignas(32) std::vector<std::complex<double>> rectangle;

        std::string file_path;

        signal_processing::FFT<double> fft{BUFFER_SIZE, signal_processing::FFT<double>::WindowType::NONE};
    };

} // processing
