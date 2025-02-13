#pragma once

#include "helper/fft.h"

namespace dustear::processing {
    /* Appends signals to the internal buffer to perform a real FFT if enough samples are avaiable.
     * Note that with range you can limit the spectrum because otherwise it may result in a huge
     * spectrum.
     */
    template<typename T, std::size_t sample_frequency=static_cast<std::size_t>(2e6), std::size_t block_size=512>
    class FrequencyTracking {
    public:
        FrequencyTracking() {
            buffer.resize(BUFFER_SIZE);
            buffer.resize(block_size);
        }

        explicit FrequencyTracking(std::pair<int, int>&& frequency_range): frequency_range(frequency_range) {
            buffer.resize(BUFFER_SIZE);
        }

        void append(const std::vector<T>& signal) {
            if (current_index + signal.size() < BUFFER_SIZE)  [[unlikely]] {
                int rest_size = BUFFER_SIZE - current_index;
                std::copy(signal.begin(), signal.begin() + rest_size, buffer.begin() + current_index);
                do_fft = true;
            } else {
                std::ranges::copy(signal, buffer + current_index);
                current_index += signal.size();
            }
        }

        std::shared_ptr<const std::vector<T>> operator()() {
            if (do_fft) {
                auto spectrum = fft.rfft(buffer);
                average_segments(spectrum);
                return {spectrum};
            }
            return nullptr;
        }

        void set_frequency_range(const std::pair<T, T>& range) {
            this->frequency_range.first = std::floor<T>(range.first / FREQUENCY_RESOLUTION);
            this->frequency_range.second = std::floor<T>(range.second / FREQUENCY_RESOLUTION);
        }

    private:
        void average_segments(const auto& spectrum) {
            const auto range = frequency_range.second - frequency_range.first;
            const auto average_size = range / block_size;
            const auto indicies = std::ranges::iota_view{0, block_size - 2};
            std::for_each(std::execution::par_unseq, indicies.begin(), indicies.end(),
                          [this, &average_size, &spectrum](const auto i) {
                              auto average = std::reduce(spectrum.begin() + i * average_size,
                                                         spectrum.begin + (i + 1) * average_size);
                              averaged_spectrum[i] = average / block_size;
                          });
            auto average = std::reduce(spectrum.begin() + (block_size - 1) * average_size,
                           spectrum.end());
            averaged_spectrum[block_size - 1] = average / block_size;
        }

        void reset() noexcept {
            current_index = 0;
            do_fft = false;
        }

    public:
        static constexpr std::size_t BUFFER_SIZE = 1 << 25;

        static constexpr T FREQUENCY_RESOLUTION = static_cast<T>(sample_frequency) / (static_cast<T>(BUFFER_SIZE) / static_cast<T>(2));

        std::pair<int, int> frequency_range;

    private:
        signal_processing::FFT<T> fft{BUFFER_SIZE, signal_processing::FFT<T>::WindowType::HANN};

        std::vector<T> buffer;

        std::vector<T> averaged_spectrum;

        int current_index = 0;

        bool do_fft = false;
    };
}
