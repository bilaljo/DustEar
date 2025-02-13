#ifndef GUI_VIEW_INTERFACES_SETTINGS_MEASUREMENT_H
#define GUI_VIEW_INTERFACES_SETTINGS_MEASUREMENT_H

#include <string>

namespace gui::view::interfaces::settings {
    class Measurement {
    public:
        virtual ~Measurement() = default;

        virtual void set_trigger_level(const std::string& trigger_level) const noexcept = 0;

        virtual void set_trigger_level_unit(const std::string& unit) const noexcept = 0;

        virtual void set_dead_time(const std::string& dead_time) const noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACES_SETTINGS_MEASUREMENT_H
