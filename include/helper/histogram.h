#pragma once

#include <cmath>
#include <ranges>
#include <execution>
#include <iostream>

namespace helper {
    template<typename T>
    concept Numerical = requires {
        std::is_floating_point_v<T> || std::is_integral_v<T>;
    };

    template<Numerical T>
    class Histogram {
    public:
        struct Range {
            int min;
            int max;
            double bin_width;
        };

        Histogram() = delete;

        Histogram(T min, T max, T bin_width): min(min), max(max), bin_width(bin_width) {
            bins = (max - min) / bin_width;
            bin_buffer.resize(bins);
        }

        void update(const T element) noexcept {
            const int i = value_to_index(element);
            bin_buffer[i] += 1 / bin_width;
            update_statistics(element);
        }

        [[nodiscard]] int value_to_index(const T value) const noexcept {
            if (value < min) {
                return 0;
            }
            if (value >= max) {
                return bins - 1;
            }
            double bin = value - min;
            bin /= (max - min);
            bin *= bins;
            return std::floor(bin);
        }

        std::vector<T> get_bins() const noexcept {
            return bin_buffer;
        }

        void update_statistics(T value) noexcept {
            T last_mean = mean;
            moving_average(value);
            if (n < 3) [[unlikely]] {
                last_mean = mean;
            }
            mean_squared += (value - last_mean) * (value - mean);
            std = std::sqrt(mean_squared / n);
        }

        [[nodiscard]] Range range() const noexcept {
            const int mean_index = value_to_index(mean);
            int lower_bound = value_to_index(mean - 2 * std);
            lower_bound = lower_bound >= 0 ? lower_bound : 0;
            int upper_bound = mean_index + value_to_index(mean + 2 * std);
            upper_bound = upper_bound <= bins - 1 ? upper_bound : bins - 1;
            return {lower_bound, upper_bound, bin_width};
        }

        [[nodiscard]] int get_size() const noexcept {
            return n;
        }

    private:
        void moving_average(const T value) noexcept {
            mean = (n * mean + value) / (n + 1);
            n++;
        }

    private:
        std::size_t bins = 1;

        T min = -std::numeric_limits<T>::infinity();

        T max = std::numeric_limits<T>::infinity();

        T bin_width{};

        std::vector<T> bin_buffer;

        T mean{};

        T mean_squared{};

        T std{};

        int n = 1;
    };
}
