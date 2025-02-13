#ifndef DRIVER_INTERFACE_NETWORK_H
#define DRIVER_INTERFACE_NETWORK_H

#include <array>
#include <semaphore>

#include "helper/ring_buffer.h"
#include "helper/double_buffer.hpp"

namespace hardware::driver {
    class Network {
    public:
        Network() = default;

        virtual ~Network() = default;

        [[noreturn]] virtual void event_loop() noexcept = 0;

        virtual void connect() noexcept = 0;

        virtual void disconnect() noexcept = 0;

        Network(const Network& other) = delete;

        Network& operator=(const Network& other) = delete;

        Network(Network&& other) = delete;

        Network& operator=(Network&& other) = delete;

    public:
        enum class State {
            startup_failed,
            socket_failed,
            binding_failed,
            unconnected,
            connected,
            listening,
            dissconnect_failed,
        };

        [[nodiscard]] virtual State get_state() const noexcept = 0;

    private:
        static constexpr std::size_t QUEUE_SIZE{16};

    protected:
        static constexpr int EXPECTED_BYTES = 1458;

        static constexpr int SEQUENCE_NUMBER_SIZE = 4;

        static constexpr int SAMPLE_SIZE = 2;

    public:
        static constexpr int PACKAGES = (EXPECTED_BYTES - SEQUENCE_NUMBER_SIZE) / SAMPLE_SIZE;

    protected:
        std::array<std::uint16_t, PACKAGES> packages{};

        helper::RingBuffer<int, 2> ring_buffer;

        State state;
    public:
        static constexpr std::size_t BUFFER_SIZE = 50000;

        using Package = std::array<std::uint16_t, PACKAGES>;

        helper::DoubleBuffer<Package, BUFFER_SIZE> package_buffer{};

    protected:
        using Stream = std::array<char, EXPECTED_BYTES>;
    };
}

#endif  // DRIVER_INTERFACE_NETWORK_H
