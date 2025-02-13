#ifndef GUI_VIEW_INTERFACES_SETTINGS_GENERAL_H
#define GUI_VIEW_INTERFACES_SETTINGS_GENERAL_H

namespace gui::view::interfaces::settings {
    class General {
    public:
        virtual void set_save_raw_data(bool state) const noexcept = 0;

        virtual void set_save_pulses(bool state) const noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACES_SETTINGS_GENERAL_H
