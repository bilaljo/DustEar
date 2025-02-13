#ifndef GUI_VIEW_SETTINGS_MEASUREMENT_H
#define GUI_VIEW_SETTINGS_MEASUREMENT_H

#include "gui/view/interface/settings/measurement.h"
#include "gui/presenter/settings/measurement.h"

#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>
#include <QGridLayout>

namespace gui::view::settings {
    class Measurement final : public interfaces::settings::Measurement, public QGroupBox {
    public:
        Measurement();

        ~Measurement() override = default;

        void set_trigger_level(const std::string& trigger_level) const noexcept override;

        void set_dead_time(const std::string& dead_time) const noexcept override;

        void set_trigger_level_unit(const std::string& unit) const noexcept override;

    private:
        void init_labels();

        void init_fonts();

        void init_trigger_level();

        void init_dead_time();

        void init_signals() const noexcept;

    private:
        std::unique_ptr<QLabel> trigger_level_label;

        std::unique_ptr<QTextEdit> trigger_level;

        std::unique_ptr<QLabel> trigger_level_unit;

        std::unique_ptr<QLabel> dead_time_label;

        std::unique_ptr<QTextEdit> dead_time;

        std::unique_ptr<QLabel> dead_time_unit;

        std::unique_ptr<QGridLayout> m_layout;

    private:
        using Presenter = presenter::settings::Measurement;

        Presenter presenter{*this};
    };
}

#endif  // GUI_VIEW_SETTINGS_MEASUREMENT_H
