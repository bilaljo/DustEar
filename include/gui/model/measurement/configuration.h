#ifndef GUI_MODEL_MEASUREMENT_CONFIGURATION_H
#define GUI_MODEL_MEASUREMENT_CONFIGURATION_H

#include <string_view>

#include "nlohmann/json.hpp"

namespace gui::model::measurement {
    class Configuration {
    public:
        void load(std::string_view file_path) noexcept;

        void save() noexcept;

        void save_as(std::string_view file_path) noexcept;

    public:
        bool trigger_detection;

        bool ai_detection;

        bool auto_clean;

        bool save_raw_data;

        std::string working_directory;
    };
}


#endif  // GUI_MODEL_MEASUREMENT_CONFIGURATION_H
