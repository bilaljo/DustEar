#pragma once

#include <vector>

#include "helper/signal_processing.hpp"


namespace dustear::processing::detection {
    template<std::ranges::range T>
    class CrossCorrelation {
    public:
        using ValueType = std::ranges::range_value_t<T>;

        CrossCorrelation() = default;

        CrossCorrelation(T&& reference, ValueType trigger_level):
        trigger_level(trigger_level),reference_signal(reference) {
        }

        std::vector<std::pair<T, ValueType>> detect_pulses(const T& data) noexcept {
            std::vector<std::pair<T, ValueType>> pulses;
            std::size_t start = 0;
            if (last_index) {
                std::size_t buffer_rest_size = 2 * frame_size - (last_index - frame_size);
                std::ranges::copy_n(data.begin(), buffer_rest_size, buffer.begin() + last_index - frame_size);
                offset_correction(buffer);
                norm_to_energy(normed_signal);
                signal_processing::correlate(reference_signal, normed_signal, correlation_buffer);
                auto it = correlation_buffer.begin();
                auto [max_index, max] = argmax({it, it + frame_size});
                int occurrence = max_index - frame_size + 1;
                if (occurrence <= 0) {
                    occurrence = max_index;
                }
                auto it_segment = buffer.begin() + occurrence;
                std::vector pulse(it_segment, it_segment + frame_size);
                pulses.emplace_back(std::move(pulse), sigmoid(max));
                start = max_index + frame_size - last_index;
                last_index = 0;
                std::ranges::fill(buffer, 0);
            }
            offset_correction(data);
            norm_to_energy(normed_signal);
            signal_processing::correlate(reference_signal, normed_signal, correlation_buffer);
            for (std::size_t i = start; i < correlation_buffer.size(); i++) {
                if (sigmoid(correlation_buffer[i]) > trigger_level) {
                    if (correlation_buffer.size() - i < frame_size) [[unlikely]] {
                        break;
                    }
                    auto it = correlation_buffer.begin() + i;
                    auto [max_index, max] = argmax({it, it + frame_size});
                    int occurrence = i + max_index - frame_size + 1;
                    if (occurrence <= 0) {
                        occurrence = i + max_index;
                    }
                    if (data.size() - occurrence < frame_size) {
                        last_index = correlation_buffer.size() - i;
                        std::ranges::copy_n(data.begin() + i, last_index - frame_size, buffer.begin());
                        break;
                    }
                    std::vector pulse(data.begin() + occurrence, data.begin() + occurrence + frame_size);
                    pulses.emplace_back(std::move(pulse), sigmoid(std::abs(max)));
                    i += max_index + frame_size - 1;
                }
            }
            return pulses;
        }

        void update_parameters(std::size_t frame_size, T&& reference) noexcept {
            this->frame_size = frame_size;
            this->reference_signal = reference;
            buffer.resize(2 * frame_size);
            correlation_buffer.resize(frame_size + BUFFER_SIZE - 1);
            normed_signal.resize(BUFFER_SIZE);
            std::ranges::fill(correlation_buffer, 0);
            std::ranges::fill(buffer, 0);
            std::ranges::fill(normed_signal, 0);
        }

    private:
        void offset_correction(const T& signal) noexcept {
            ValueType offset = std::reduce(signal.begin(), signal.end(), static_cast<ValueType>(0.0), std::plus<ValueType>()) / signal.size();
            std::ranges::transform(signal, normed_signal.begin(), [offset](auto& x){return x - offset;});
        }

        void norm_to_energy(const T& signal) noexcept {
            const ValueType energy = std::reduce(signal.begin(), signal.end(), 0.0, [](const ValueType acc, const ValueType x) {return acc + x * x;});
            if (energy > epsilon * signal.size()) {
                std::ranges::transform(signal, normed_signal.begin(), [&](const ValueType x){return x / energy;});
            }
        }

        [[nodiscard]] static std::pair<std::size_t, ValueType> argmax(const T& data) noexcept {
            const auto max = std::ranges::max_element(data);
            return {std::ranges::distance(data.begin(), max), *max};
        }

        [[nodiscard]] static ValueType sigmoid(const ValueType value) noexcept {
            return 1 / (1 + std::exp(-value));
        }

    public:
        ValueType trigger_level = 1;

        std::size_t frame_size = 300;

    private:
        T reference_signal;

        T buffer;

        T normed_signal;

        T correlation_buffer;

        std::size_t last_index = 0;

        double epsilon = 1e-6;

    public:
        static constexpr std::size_t BUFFER_SIZE = 10000;
    };
}
