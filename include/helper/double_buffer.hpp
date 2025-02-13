#pragma once

#include <chrono>
#include <iostream>
#include <format>

namespace helper {
    template<typename T, std::size_t buffer_size>
    class DoubleBuffer {
    public:
        DoubleBuffer() {
            buffer.resize(2);
            for (auto & inner_buffer: buffer) {
                inner_buffer.resize(buffer_size);
            }
        }

        void push_back(const T& element) {
            if (current_index < buffer_size) {
                insert(element);
            } else {
                current_index = 0;
                buffer_index.fetch_xor(1);
                insert(element);
            }
        }

        void push_back(const T&& element) {
            if (current_index < buffer_size) {
                insert(std::move(element));
            } else {
                current_index = 0;
                buffer_index.fetch_xor(1);
                insert(std::move(element));
            }
        }

        const std::vector<T>& get_buffer() noexcept {
            semaphore.acquire();
            used_buffer.fetch_sub(1);
            return buffer[buffer_index.load() ^ 1];
        }

    private:
        void insert(const T& element) {
            if (used_buffer.load() < 2) {
                buffer[buffer_index.load()][current_index] = element;
                current_index++;
                if (current_index == buffer_size) {
                    used_buffer.fetch_add(1);
                    semaphore.release();
                }
            } else {
                const auto now = std::chrono::system_clock::now();
                auto local_time = std::chrono::current_zone()->to_local(now);
                std::cerr << "ERROR " << std::format("{:%Y-%m-%d %H:%M:%S}", local_time) << ": Buffer Overflow\n";
                semaphore.acquire();
                used_buffer.fetch_sub(1);
            }
        }

        void insert(T&& element) {
            if (used_buffer.load() < 2) {
                buffer[buffer_index.load()][current_index] = std::move(element);
                current_index++;
                if (current_index == buffer_size) {
                    used_buffer.fetch_add(1);
                    semaphore.release();
                }
            } else {
                const auto now = std::chrono::system_clock::now();
                auto local_time = std::chrono::current_zone()->to_local(now);
                std::cerr << "ERROR "  << std::format("{:%Y-%m-%d %H:%M:%S}", local_time) << ": Buffer Overflow\n";
                semaphore.acquire();
                used_buffer.fetch_sub(1);
            }
        }

    public:
        std::atomic<int> buffer_index = 0;

    private:
        std::vector<std::vector<T>> buffer;

        int current_index = 0;

        std::counting_semaphore<2> semaphore{0};

        std::atomic<int> used_buffer = 0;
    };
}
