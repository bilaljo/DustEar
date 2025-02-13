#ifndef DUSTEAR_PROCESSING_TRIGGER_ALGORITHM_H
#define DUSTEAR_PROCESSING_TRIGGER_ALGORITHM_H


#include <vector>


namespace dustear::processing::detection {
    class TriggerLevel {
    public:
        TriggerLevel() noexcept = default;

        std::vector<std::vector<double>> detect_pulses(const std::vector<double>& data) noexcept;

        void set_frame_size(std::size_t frame_size) noexcept;

    public:
        double threshold{};

    private:
        std::vector<double> buffer;

        std::size_t last_index = 0;

        std::size_t frame_size{};
    };
}


#endif  // DUSTEAR_PROCESSING_TRIGGER_ALGORITHM_H
