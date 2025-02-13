#ifndef HARDWARE_DRIVER_UNIX_UDP_H
#define HARDWARE_DRIVER_UNIX_UDP_H

#include "driver/network.h"

#include <thread>
#include <cstddef>

#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


namespace hardware::driver {
    class UDP: public Network {
    public:
        explicit UDP(std::size_t port);

        void connect() noexcept override;

        void disconnect() noexcept override;

        void listen() noexcept;

        void stop_listen() noexcept;

        State get_state() const noexcept override;

        std::size_t get_port() const noexcept;

    private:
        [[noreturn]] void event_loop() noexcept override;

    private:
        std::size_t port{};

        int sockfd{};

        sockaddr_in addr{};
    };
}

#endif  // HARDWARE_DRIVER_UNIX_UDP_H
