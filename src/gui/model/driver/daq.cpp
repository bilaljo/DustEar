#include <fstream>
#include "gui/model/driver/daq.h"

#ifdef _WIN32
#include "driver/windows/udp.h"
#else
#include "driver/linux/udp.h"
#endif

#include "helper/logging.h"
#include "helper/thread.h"

gui::model::driver::DAQ::DAQ(): udp(new hardware::driver::UDP{UDP_PORT}) {
}

gui::model::driver::DAQ::DAQ(std::vector<std::function<void(const ReceivedPackage&)>> &&callbacks):
callbacks(callbacks),
udp(new hardware::driver::UDP{UDP_PORT})
{
}

bool gui::model::driver::DAQ::run() noexcept {
    connect();
    bool success = false;
    std::function<void()> receive_handler;
    std::thread receive_thread;
    switch (udp->get_state()) {
        case hardware::driver::Network::State::connected:
            udp->listen();
            receive_handler = [this]{receive();};
            receive_thread = std::thread{receive_handler};
        #ifdef _WIN32
            helper::thread::set_scheduling(receive_thread, THREAD_PRIORITY_TIME_CRITICAL);
        #endif
            receive_thread.detach();
            logging::info("Start listening to port");
            success = true;
            break;
        default:
            logging::error("Could start listening because the port is not connected");
            logging::debug("It is currently in the state {}", encode_state());
            break;
    }
    return success;
}

void gui::model::driver::DAQ::set_callbacks(std::vector<std::function<void(const ReceivedPackage&)>>&& callbacks) noexcept {
    this->callbacks = std::move(callbacks);
}

void gui::model::driver::DAQ::connect() const noexcept {
    udp->connect();
    switch (udp->get_state()) {
        case hardware::driver::Network::State::startup_failed:
            logging::error("Startup of Win Socket failed");
            break;
        case hardware::driver::Network::State::socket_failed:
            logging::error("socket failed with error {}", WSAGetLastError());
            break;
        case hardware::driver::Network::State::binding_failed:
            logging::error("Bind failed with error {}", WSAGetLastError());
            break;
        default:
            logging::debug("Successfully started the Win Socket API");
            logging::debug("Successfully created socket");
            logging::info("Successfully binded to {}", udp->get_port());
    }
}

std::string gui::model::driver::DAQ::encode_state() const noexcept {
    switch (udp->get_state()) {
        case hardware::driver::Network::State::startup_failed:
            return "Startup Failed";
        case hardware::driver::Network::State::socket_failed:
            return "Socket Failed";
        case hardware::driver::Network::State::binding_failed:
            logging::error("Bind failed with error {}", WSAGetLastError());
            return "Bind Failed";
        case hardware::driver::Network::State::unconnected:
            return "Not Connected";
        case hardware::driver::Network::State::connected:
            return "Connected";
        case hardware::driver::Network::State::listening:
            return "Listening";
        case hardware::driver::Network::State::dissconnect_failed:
            return "Disconnect Failed";
        default:
            return "";
    }
}

bool gui::model::driver::DAQ::stop() noexcept {
    bool success = false;
    switch (udp->get_state()) {
        case hardware::driver::Network::State::listening:
            udp->disconnect();
            logging::info("Stoped listening to port");
            udp->running = false;
            success = true;
            break;
        default:
            logging::error("Could stop listening because the port was not listening");
            logging::debug("It is currently in the state \"{}\"", encode_state());
            break;
    }
    return success;
}

void gui::model::driver::DAQ::receive() const noexcept {
    while (udp->running) {
        for (const auto& buffer = udp->package_buffer.get_buffer(); const auto& package: buffer) {
            for (const auto& callback: callbacks) {
                callback(package);
            }
        }
    }
}

bool gui::model::driver::DAQ::is_running() const noexcept {
    return udp->running;
}
