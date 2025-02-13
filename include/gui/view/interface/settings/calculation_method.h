#ifndef GUI_VIEW_INTERFACES_SETTINGS_CALCULATION_METHOD_H
#define GUI_VIEW_INTERFACES_SETTINGS_CALCULATION_METHOD_H

#include <string>
#include <vector>

namespace gui::view::interfaces::settings {
    class CalculationMethod {
    public:
        virtual void set_calculation_method(const std::string& method) const noexcept = 0;

        virtual void set_calculation_methods(const std::vector<std::string>& methods) const noexcept = 0;

        virtual void set_detection_method(const std::string& method) const noexcept = 0;

        virtual void set_detection_methods(const std::vector<std::string>& methods) const noexcept = 0;

        virtual void set_frequency(const std::string& frequency) const noexcept = 0;

        virtual void set_time_constant(const std::string& time_constant) const noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACES_SETTINGS_CALCULATION_METHOD_H
