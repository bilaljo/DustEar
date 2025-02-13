#include "processing/detection/trigger_level.h"

#include <algorithm>

void dustear::processing::detection::TriggerLevel::set_frame_size(const std::size_t frame_size) noexcept {
    this->frame_size = frame_size;
    buffer.resize(frame_size);
}

std::vector<std::vector<double>> dustear::processing::detection::TriggerLevel::detect_pulses(const std::vector<double>& data) noexcept {
    std::vector<std::vector<double>> pulses;
    int start = 0;
    if (last_index) {
        std::ranges::copy_n(data.begin(), frame_size - last_index, buffer.begin() + last_index);
        start = frame_size - last_index;
        pulses.emplace_back(buffer);
        last_index = 0;
    }
    for (std::size_t i = start; i < data.size(); i++) {
        if (data[i] > threshold) {
            if (data.size() - i < frame_size) {
                last_index = data.size() - i;
                std::ranges::copy_n(data.begin() + i, last_index, buffer.begin());
                break;
            }
            pulses.emplace_back(data.begin() + i, data.begin() + i + frame_size);
            i += frame_size - 1;
        }
    }
    return pulses;
}
