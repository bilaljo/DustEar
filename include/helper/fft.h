#pragma once

#include <ranges>
#include <complex>
#include <vector>
#include <execution>
#include <thread>
#include <numbers>

#include <pocketfft/pocketfft.h>

#undef POCKETFFT_CACHE_SIZE
#define POCKETFFT_CACHE_SIZE 3
#undef POCKETFFT_NO_VECTORS

namespace signal_processing {
    template<typename T>
    class FFT {
    public:
        using ComplexArray = std::vector<std::complex<T>>;

        using RealArray = std::vector<T>;

        struct Window {
            static void hann(std::vector<T>& result) {
                const int m = static_cast<int>(result.size());
                std::ranges::iota_view corr_indicies(0, m);
                std::for_each(std::execution::par_unseq, corr_indicies.begin(), corr_indicies.end(),
                              [&result, &m](const int n) {
                                  result[n] = 1.0 / 2.0 * (1 - std::cos(2 * std::numbers::pi_v<double> * n / (m - 1)));
                              });
            }
        };

        enum class WindowType {
            HANN,
            NONE
        };

        explicit FFT(int size, const WindowType type = WindowType::HANN): size(size), window_type(type) {
            window.resize(size);
            signal_buffer.resize(size);
            fft_buffer.resize(size / 2 + 1);
            absolute_spectrum.resize(size / 2 + 1);
            inidices = std::ranges::iota_view(0, size);
            switch (type) {
                case WindowType::HANN:
                    Window::hann(window);
                break;
                default:
                    std::ranges::fill(window, 1.0);
            }
        }

        void apply_window(const RealArray& signal) {
            std::for_each(std::execution::par_unseq, inidices.begin(), inidices.end(),
                         [&signal, this](const int i){signal_buffer[i] = window[i] * signal[i];});
        }

        ComplexArray& rfft(const RealArray& signal) {
            if (window_type != WindowType::NONE) {  [[likely]]
                apply_window(signal);
            }
            pocketfft::r2c({size},
                           {sizeof(T)},
                           {sizeof(std::complex<T>)},
                           0,
                           pocketfft::FORWARD,
                           signal.data(),
                           fft_buffer.data(),
                           static_cast<T>(1.0),
                           threads);
            return fft_buffer;
        }

        RealArray& irfft(ComplexArray& spectrum) {
            pocketfft::c2r({size},
                           {sizeof(std::complex<T>)},
                           {sizeof(T)},
                           0,
                           pocketfft::BACKWARD,
                           spectrum.data(),
                           signal_buffer.data(),
                           static_cast<T>(1.0) / size,
                           threads);
            return signal_buffer;
        }

        RealArray& get_abs_spectrum(int f0, int f1) noexcept {
            std::transform(std::execution::par_unseq, fft_buffer.begin() + f0, fft_buffer.begin() + f0 + f1,
                           absolute_spectrum.begin(), [](const std::complex<double> element){return std::abs(element);});
            return absolute_spectrum;
        }

        static unsigned int threads;

    private:
        alignas(32) RealArray window;

        alignas(32) RealArray signal_buffer;

        alignas(32) RealArray absolute_spectrum;

        alignas(32) ComplexArray fft_buffer;

        std::ranges::iota_view<int, int> inidices;

        std::size_t size;

        WindowType window_type;
    };

    template<typename T>
    unsigned int FFT<T>::threads = static_cast<int>(3.0 / 4.0 * std::thread::hardware_concurrency());
}
