#ifndef GUI_PRESENTER_SETTINGS_GENERAL_H
#define GUI_PRESENTER_SETTINGS_GENERAL_H

#include "gui/view/interface/settings/general.h"

namespace gui::presenter::settings {
    class General {
    public:
        using View = view::interfaces::settings::General;

    public:
        explicit General(View& view);

        void update_save_raw_data_view(bool state) noexcept;

        static void update_save_raw_data_model(bool state) noexcept;

        void update_save_pulses_view(bool state) noexcept;

        static void update_save_pulses_model(bool state) noexcept;

        static void save_settings_model() noexcept;

    private:
        View& view;
    };
}

#endif  // GUI_PRESENTER_SETTINGS_GENERAL_H
