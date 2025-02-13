#include "driver/windows/udp.h"

#include <thread>
#include <span>
#include "helper/thread.h"

#include "helper/logging.h"


#define SOCKET_VERSION MAKEWORD(2, 2)
#define RECEIVE_FLAGS 0

hardware::driver::UDP::UDP(const std::size_t port)
: port(port) {
}

void hardware::driver::UDP::connect() noexcept {
    WSADATA wsaData;
    WSACleanup();
    int error_code = WSAStartup(SOCKET_VERSION, &wsaData);
    if (error_code != NO_ERROR) {  [[unlikely]]
        state = State::startup_failed;
        return;
    }
    receive_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (receive_socket == INVALID_SOCKET) {
        state = State::socket_failed;
        return;
    }
    memset(&receive_address, 0, sizeof(receive_address));
    receive_address.sin_family = AF_INET;
    receive_address.sin_port = htons(port);
    receive_address.sin_addr.s_addr = htonl(INADDR_ANY);
    error_code = bind(receive_socket, reinterpret_cast<sockaddr*>(&receive_address), sizeof(receive_address));
    if (error_code != NO_ERROR) {  [[unlikely]]
        state = State::binding_failed;
        return;
    }
    state = State::connected;
}

void hardware::driver::UDP::listen() noexcept {
    running = true;
    auto event_loop_wrapper = [this]{event_loop();};
    if (state == State::connected) {
        state = State::listening;
        auto thread = std::thread(event_loop_wrapper);
        helper::thread::set_scheduling(thread, THREAD_PRIORITY_TIME_CRITICAL);
        thread.detach();
    }
}

void hardware::driver::UDP::disconnect() noexcept {
    running = false;
    if (const int error_code = closesocket(receive_socket); error_code == SOCKET_ERROR) {
        state = State::dissconnect_failed;
    } else {
        state = State::unconnected;
    }
    WSACleanup();
}

void hardware::driver::UDP::event_loop() noexcept {
    sockaddr_in sender_address{};
    int sender_length = sizeof(sender_address);
    Stream buffer;
    while (running) {
        const int size = recvfrom(receive_socket, buffer.data(), EXPECTED_BYTES,
                RECEIVE_FLAGS, reinterpret_cast<sockaddr*>(&sender_address),
                     &sender_length);
        if (size < EXPECTED_BYTES) [[unlikely]] {
            continue;
        }
        int sequence_number = 0;
        memcpy(&sequence_number, buffer.data(), 4);
        ring_buffer.push(sequence_number);
        if (ring_buffer.get_size() > 1) [[likely]] {
            if (const int difference = ring_buffer[1] - ring_buffer[0]; difference > 1) {
                const auto now = std::chrono::system_clock::now();
                auto local_time = std::chrono::current_zone()->to_local(now);
                std::cerr << "ERROR " << std::format("{:%Y-%m-%d %H:%M:%S}", local_time) << ": Lost " << difference << " packages \n";
                for (int i= 0; i < difference; i++) {
                    package_buffer.push_back({1 << 15});
                }
            }
        }
        convert_endian(std::span{buffer}.subspan(SEQUENCE_NUMBER_SIZE));
        package_buffer.push_back(packages);
    }
}

hardware::driver::Network::State hardware::driver::UDP::get_state() const noexcept {
    return state;
}

std::size_t hardware::driver::UDP::get_port() const noexcept {
    return port;
}

void hardware::driver::UDP::convert_endian(const std::span<char>& numbers) noexcept {
    for (int i = 0; i < PACKAGES; i++) {
        if constexpr (std::endian::native == std::endian::little) {
            packages[i] = (static_cast<std::uint16_t>(numbers[2 * i + 1] & 0xFF) << 8) + static_cast<std::uint16_t>(numbers[2 * i] & 0xFF);
        } else if constexpr (std::endian::native == std::endian::big) {
            packages[i] = (static_cast<std::uint16_t>(numbers[2 * i]) << 8) + static_cast<std::uint16_t>(numbers[2 * i + 1]);
        }
    }
}
