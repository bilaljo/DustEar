#ifndef GUI_VIEW_SETTINGS_GENERAL_H
#define GUI_VIEW_SETTINGS_GENERAL_H

#include "gui/view/interface/settings/general.h"

#include <memory>

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

#include "gui/view/widgets/animated_toggle.hpp"
#include "gui/presenter/settings/general.h"

namespace gui::view::settings {
    class General: public interfaces::settings::General, public QGroupBox {
    private:
        using Presenter = presenter::settings::General;

    public:
        General();

        ~General() override = default;

        void set_save_raw_data(bool state) const noexcept override;

        void set_save_pulses(bool state) const noexcept override;

    private:
        void init_labels() const noexcept;

        void init_save_raw_data() const noexcept;

        void init_pulses() const noexcept;

        void init_settings_buttons() const noexcept;

        void init_fonts() noexcept;

        void init_signals() const noexcept;

    private:
        std::unique_ptr<widgets::AnimatedToggle> save_raw_data = nullptr;

        std::unique_ptr<QLabel> save_raw_data_label = nullptr;

        std::unique_ptr<widgets::AnimatedToggle> save_pulses = nullptr;

        std::unique_ptr<QLabel> save_pulses_label = nullptr;

        std::unique_ptr<QPushButton> save_settings = nullptr;

        std::unique_ptr<QPushButton> load_settings = nullptr;

        std::unique_ptr<QGridLayout> m_layout = nullptr;

        Presenter presenter{*this};
    };
}

#endif  // GUI_VIEW_SETTINGS_GENERAL_H
