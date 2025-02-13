#ifndef HELPER_RINGBUFFER_H
#define HELPER_RINGBUFFER_H

#include <vector>
#include <optional>
#include <algorithm>
#include <semaphore>
#include <mutex>

namespace helper {
    template<typename RingBuffer>
    class RingBufferIterator {
    public:
        using ValueType = typename RingBuffer::ValueType;
        using PointerType = typename RingBuffer::ValueType*;
        using ReferenceType = typename RingBuffer::ValueType&;
    };

    template<typename T, std::size_t size, bool blocking = false>
    class RingBuffer {
    public:
        using ValueType = T;
        using Iterator = RingBufferIterator<RingBuffer>;

        RingBuffer() {
            buffer.resize(size);
        };

        ~RingBuffer() = default;

        Iterator begin() {
            return Iterator{buffer.data()};
        }

        Iterator end() const {
            return  Iterator{buffer.data() + size};
        }

        std::vector<T> data() const noexcept {
            return buffer;
        }

        std::optional<T> pop() noexcept {
            if (end_index < 0) {
                return {};
            }
            return buffer[end_index--];
        }

        void push(T&& element) noexcept {
            if (end_index == size - 1) {
                std::ranges::rotate(buffer, buffer.begin() + 1);
            } else {
                end_index++;
            }
            buffer[end_index] = element;
        }

        void push(const T& element) noexcept {
            if (end_index == size - 1) {
                std::ranges::rotate(buffer, buffer.begin() + 1);
            } else {
                end_index++;
            }
            buffer[end_index] = element;
        }

        [[nodiscard]] std::size_t get_size() const noexcept {
            return end_index + 1;
        }

        void clear() noexcept {
            end_index = -1;
            std::ranges::fill(buffer, 0);
        }

        const T& operator[](const std::size_t index) const noexcept {
            return buffer[index];
        }

        [[nodiscard]] bool is_empty() const noexcept {
            return end_index < 0;
        }

        int end_index = -1;
    private:
        std::vector<T> buffer{};
    };
}

#endif  // HELPER_RINGBUFFER_H
