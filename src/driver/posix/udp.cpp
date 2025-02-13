#include "driver/posix/udp.h"

#include <string_view>

using namespace hardware;


driver::UDP::UDP(const std::size_t port): port(port) {}

void driver::UDP::connect() noexcept {
    sockfd = socket(AF_INET, SOCK_DGRAM, SOCK_DGRAM);
    if (sockfd < 0) {
        state = State::socket_failed;
    } else {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
    }

}

void driver::UDP::listen() noexcept {
    auto event_loop_wrapper = [this](){this->event_loop();};
    state = driver::Network::State::listening;
    std::thread{event_loop_wrapper}.detach();
}

void driver::UDP::disconnect() noexcept {
    int error_code = closesocket(receive_socket);;
    if (error_code == SOCKET_ERROR) {
        state = driver::Network::State::dissconnect_failed;
    } else {
        state = driver::Network::State::unconnected;
    }
    WSACleanup();
}


[[noreturn]] void driver::UDP::event_loop() noexcept {
    logging::debug("UDP event loop started");
    int sender_length = sizeof(sender_address);
    while (true) {
        int bytes_received = recvfrom(receive_socket, buffer.data(), EXPECTED_BYTES,
                                      RECEIVE_FLAGS, (SOCKADDR *) &sender_address,
                                      &sender_length);
        if (bytes_received < EXPECTED_BYTES) {
            logging::warning("Execpted {} bytes but received {}", EXPECTED_BYTES, bytes_received);
        } else {
            int sequence_number = 0;
            memcpy(&sequence_number, buffer.data(), 4);
            ring_buffer.push(sequence_number);
            if (ring_buffer.get_size() > 1) [[likely]] {
                std::size_t package_difference = ring_buffer[1] - ring_buffer[0];
                if (package_difference != 1) {
                    logging::warning("Lost {} packages", package_difference);
                }
            }
            memcpy(packages.data(), buffer.data() + 4, SAMPLE_SIZE);
            {
                std::lock_guard<std::mutex> lock(receive_mutex);
                received_data.push(packages);
            }
        }
    }
}

driver::Network::State driver::UDP::get_state() const noexcept {
    return state;
}

std::size_t driver::UDP::get_port() const noexcept {
    return port;
}
