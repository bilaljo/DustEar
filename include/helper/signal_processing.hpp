#pragma once

/* Signal Processing is a high-performance, header only signal processing toolbox in which
 * all operations are multithreaded parallezised and make use of SIMD when ever possible. Just
 * copy the file into your project.
 *
 * Currently implemented:
 * - Correlation
 * - Convolution
 * - rfft (1D)
 * - irfft (1D)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software. THE SOFTWARE IS PROVIDED “AS IS”,
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <concepts>
#include <execution>
#include <ranges>

namespace signal_processing {
    template <typename T>
    concept NumericType= std::integral<T> || std::floating_point<T>;

    enum class Status {
        SUCCESS,
        RESULT_BUFFER_TOO_SMALL,
    };

    template<NumericType T>
    Status correlate(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& result) noexcept {
        const auto n = static_cast<int>(x.size());
        const auto m = static_cast<int>(y.size());
        const int corr_length = n + m - 1;
        std::ranges::iota_view inidices(0, corr_length);
        if (static_cast<int>(result.size()) != corr_length) {
            return Status::RESULT_BUFFER_TOO_SMALL;
        }
        std::for_each(std::execution::par_unseq, inidices.begin(), inidices.end(),
                      [&x, m, &y, n, &result](const int k) {
                          double res = 0;
                          for (int i = 0; i < n; i++) {
                            if (const int j = k - (n - 1) + i; j >= 0 && j < m) {
                                res += x[i] * y[j];
                            }
                          }
                          result[k] = res;
                      });
        return Status::SUCCESS;
    }

    template<NumericType T>
    Status convolve(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& result) noexcept {
        return correlate(x, std::reverse(y), result);
    }
}
