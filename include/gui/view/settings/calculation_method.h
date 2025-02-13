#ifndef GUI_VIEW_SETTINGS_CALCULATION_METHOD_H
#define GUI_VIEW_SETTINGS_CALCULATION_METHOD_H

#include "gui/view/interface/settings/calculation_method.h"
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QTextEdit>

#include "gui/presenter/settings/calculation_method.h"

namespace gui::view::settings {
    class CalculationMethod final: public interfaces::settings::CalculationMethod, public QGroupBox {
    public:
        CalculationMethod();

        ~CalculationMethod() override = default;

        void set_calculation_method(const std::string& method) const noexcept override;

        void set_calculation_methods(const std::vector<std::string>& methods) const noexcept override;

        void set_detection_method(const std::string& method) const noexcept override;

        void set_detection_methods(const std::vector<std::string>& methods) const noexcept override;

        void set_frequency(const std::string& frequency) const noexcept override;

        void set_time_constant(const std::string& time_constant) const noexcept override;

    private:
        void init_labels() const noexcept;

        void init_fonts() noexcept;

        void init_change_method() const noexcept;

        void init_initial_guesses() const noexcept;

        void init_signals() const noexcept;

    private:
        std::unique_ptr<QLabel> change_method_label;

        std::unique_ptr<QComboBox> change_method;

        std::unique_ptr<QLabel> change_detection_method_label;

        std::unique_ptr<QComboBox> change_detection_method;

        std::unique_ptr<QLabel> resonance_frequency_label;

        std::unique_ptr<QTextEdit> resonance_frequency;

        std::unique_ptr<QLabel> resonance_frequency_unit;

        std::unique_ptr<QLabel> time_constant_label;

        std::unique_ptr<QTextEdit> time_constant;

        std::unique_ptr<QLabel> time_constant_unit;

        std::unique_ptr<QGridLayout> m_layout;

        gui::presenter::settings::CalculationMethod presenter{*this};
    };
}

#endif  // GUI_VIEW_SETTINGS_CALCULATION_METHOD_H
