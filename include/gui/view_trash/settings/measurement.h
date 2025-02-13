#ifndef GUI_VIEW_SETTINGS_ALGORITHM
#define GUI_VIEW_SETTINGS_ALGORITHM

#include "gui/view/interface/settings/measurement_old.h"
#include "gui/presenter/settings/measurement.h"
#include "gui/view/general_purpose/state_button.h"

#include <gtkmm.h>
#include <gtkmm/button.h>



namespace gui::view::settings {
    class Measurement: public Gtk::Frame, public interface::settings::Measurement {
        public:
        Measurement();

        void enable_use_ai_detection() noexcept override;

        void disable_use_ai_detection() noexcept override;

        void enable_use_trigger_detection() noexcept override;

        void disable_use_trigger_detection() noexcept override;

        void enable_save_raw_data() noexcept override;

        void disable_save_raw_data() noexcept override;

        void enable_automatic_clean() noexcept override;

        void disable_automatic_clean() noexcept override;

        [[nodiscard]] general_purpose::ButtonState get_ai_button_state() const noexcept override;

        [[nodiscard]] general_purpose::ButtonState get_trigger_detection_button_state() const noexcept override;

        [[nodiscard]] general_purpose::ButtonState get_save_raw_data_state() const noexcept override;

        [[nodiscard]] general_purpose::ButtonState get_automatic_clean_state() const noexcept override;

        void set_ai_detection_button_state(general_purpose::ButtonState state) noexcept override;

        void set_trigger_detection_button_state(general_purpose::ButtonState state) noexcept override;

        void set_save_raw_data_button_state(general_purpose::ButtonState state) noexcept override;

        void set_automatic_clean_button_state(general_purpose::ButtonState state) noexcept override;

    private:
        void init_layout() noexcept;

        void init_buttons() noexcept;

        static void enable(Gtk::Button& button) noexcept;

        static void disable(Gtk::Button& button) noexcept;


        struct Buttons {
            general_purpose::StateButton trigger_detection;
            general_purpose::StateButton ai_detection;
            general_purpose::StateButton save_raw_data;
            general_purpose::StateButton automatic_clean;
        };

        Buttons buttons;

        using presenterType = presenter::settings::Measurement;

        presenterType presenter{*this};
    };
}

#endif  // GUI_VIEW_SETTINGS_ALGORITHM
