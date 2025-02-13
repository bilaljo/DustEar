#ifndef GUI_MODEL_DIRVER_DAQ_H
#define GUI_MODEL_DIRVER_DAQ_H

#ifdef _WIN32
#include "driver/windows/udp.h"
#endif

#include <functional>

namespace gui::model::driver {
    class DAQ {
    public:
        using ReceivedPackage = hardware::driver::Network::Package;

    public:
        DAQ();

        explicit DAQ(std::vector<std::function<void(const ReceivedPackage&)>>&& callbacks);

        DAQ(const DAQ& other) = delete;

        void set_callbacks(std::vector<std::function<void(const ReceivedPackage&)>>&& callbacks) noexcept;

        bool run() noexcept;

        void connect() const noexcept;

        [[nodiscard]] std::string encode_state() const noexcept;

        bool stop() noexcept;

        [[nodiscard]] bool is_running() const noexcept;

        void receive() const noexcept;

        template<typename T>
        static T bit_to_volt(const std::uint16_t sample) noexcept {
            auto sample_T = static_cast<T>(sample);
            return static_cast<T>(sample_T - (1 << 15)) * ADC_VOLTAGE / static_cast<T>(ADC_RESOLUTION);
        }

    public:
        static constexpr uint16_t UDP_PORT{11111};

        static constexpr std::uint8_t ADC_BITS = 16;

        // One bit is for the sign
        static constexpr int ADC_RESOLUTION = (1 << ADC_BITS) - 1;

        static constexpr double ADC_VOLTAGE = 3.3;

        std::vector<std::function<void(const ReceivedPackage&)>> callbacks;

    private:
        std::unique_ptr<hardware::driver::UDP> udp;

        bool running = false;
    };
}

#endif  // GUI_MODEL_DIRVER_DAQ_H
