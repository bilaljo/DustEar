#ifndef GUI_VIEW_INTERFACE_SETTINGS_MEASUREMENT_H
#define GUI_VIEW_INTERFACE_SETTINGS_MEASUREMENT_H


namespace gui::view::interfaces::settings {
    class Measurement {
    public:
        enum class State {
            enabled,
            disabled
        };

        virtual void enable_use_ai_detection() noexcept = 0;

        virtual void disable_use_ai_detection() noexcept = 0;

        virtual void enable_use_trigger_detection() noexcept = 0;

        virtual void disable_use_trigger_detection() noexcept = 0;

        virtual void enable_save_raw_data() noexcept = 0;

        virtual void disable_save_raw_data() noexcept = 0;

        virtual void enable_automatic_clean() noexcept = 0;

        virtual void disable_automatic_clean() noexcept = 0;

        [[nodiscard]] virtual State get_ai_button_state() const noexcept = 0;

        [[nodiscard]] virtual State get_trigger_detection_button_state() const noexcept = 0;

        [[nodiscard]] virtual State get_save_raw_data_state() const noexcept = 0;

        [[nodiscard]] virtual State get_automatic_clean_state() const noexcept = 0;

        virtual void set_ai_detection_button_state(State state) noexcept = 0;

        virtual void set_save_raw_data_button_state(State state) noexcept = 0;

        virtual void set_trigger_detection_button_state(State state) noexcept = 0;

        virtual void set_automatic_clean_button_state(State state) noexcept = 0;
    };
}


#endif  // GUI_VIEW_INTERFACE_SETTINGS_MEASUREMENT_H
