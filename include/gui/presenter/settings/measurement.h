#ifndef GUI_PRESENTER_SETTINGS_MEASUREMENT_H
#define GUI_PRESENTER_SETTINGS_MEASUREMENT_H

#include "gui/view/interface/settings/measurement.h"

namespace gui::presenter::settings {
    class Measurement {
    private:
        using viewType = view::interfaces::settings::Measurement;

    public:
        explicit Measurement(viewType& view);

        void update_trigger_level_view(double trigger_level) const noexcept;

        void update_dead_time_view(std::size_t dead_time) const noexcept;

        void update_trigger_level(const std::string& number) const noexcept;

        static void update_dead_time(const std::string& number) noexcept;

        static void save_config() noexcept;

        void update_trigger_level_unit(bool correlation) noexcept;

    private:
        viewType& view;

        bool correlation;
    };
}


#endif  // GUI_PRESENTER_SETTINGS_MEASUREMENT_H
