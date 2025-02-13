#ifndef DRIVER_WINDOWS_UDP_H
#define DRIVER_WINDOWS_UDP_H

#include "driver/network.h"
#include <span>

#ifndef NOMINMAX
# define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>

#include <functional>

namespace hardware::driver {
    class UDP: public Network {
    public:
        explicit UDP(std::size_t port);

        void connect() noexcept override;

        void disconnect() noexcept override;

        void listen() noexcept;

        void stop_listen() noexcept;

        [[nodiscard]] State get_state() const noexcept override;

        [[nodiscard]] std::size_t get_port() const noexcept;

    public:
        bool running = false;

        void convert_endian(const std::span<char>& numbers) noexcept;

    private:
        [[noreturn]] void event_loop() noexcept override;

    private:
        std::size_t port;

        SOCKET receive_socket{};

        sockaddr_in receive_address{};

        std::vector<std::function<void(Package)>> callbacks;
    };
}

#endif  // DRIVER_WINDOWS_UDP_H
