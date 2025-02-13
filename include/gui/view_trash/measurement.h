#ifndef GUI_VIEW_SETTINGS_ALGORITHM
#define GUI_VIEW_SETTINGS_ALGORITHM

#include "gui/view/interface/settings/measurement_old.h"
#include "gui/presenter/settings/measurement.h"

#include <QGroupBox>
#include <QPushButton>


namespace gui::view::settings {
    class Measurement: public QGroupBox, public interfaces::settings::Measurement {
        public:
        Measurement();

        void enable_use_ai_detection() noexcept override;

        void disable_use_ai_detection() noexcept override;

        void enable_use_trigger_detection() noexcept override;

        void disable_use_trigger_detection() noexcept override;

        void enable_save_raw_data() noexcept override;

        void disable_save_raw_data() noexcept override;

        void enable_automatic_clean() noexcept override;

        void disable_automatic_clean() noexcept override;;

        [[nodiscard]] State get_trigger_detection_button_state() const noexcept override;

        [[nodiscard]] State get_ai_button_state() const noexcept override;

        [[nodiscard]] State get_save_raw_data_state() const noexcept override;

        [[nodiscard]] State get_automatic_clean_state() const noexcept override;

        void set_ai_detection_button_state(State state) noexcept override;

        void set_save_raw_data_button_state(State state) noexcept override;

        void set_trigger_detection_button_state(State state) noexcept override;

        void set_automatic_clean_button_state(State state) noexcept override;

    private:
        struct StateButton: public QPushButton{
            State state;
            explicit StateButton(const QString& title);
        };

    private:
        constexpr static std::string_view DARK_BACKGROUND{"#455364"};

        constexpr static std::string_view LIGHT_BACKGROUND{"#C0C4C8"};

        void init_layout() noexcept;

        void init_buttons() noexcept;

        struct Buttons {
            StateButton *trigger_detection;
            StateButton *ai_detection;
            StateButton *save_raw_data;
            StateButton *automatic_clean;
        };

        Buttons buttons;

        gui::presenter::settings::Measurement presenter{*this};

        static void change_color(StateButton *button, std::string_view color) noexcept;
    };
}

#endif  // GUI_VIEW_SETTINGS_ALGORITHM
