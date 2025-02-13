#ifndef GUI_PRESENTER_SETINGS_CALCULATION_METHOD_H
#define GUI_PRESENTER_SETINGS_CALCULATION_METHOD_H

#include "gui/view/interface/settings/calculation_method.h"
#include "gui/model/measurement/processing.h"

namespace gui::presenter::settings {
    class CalculationMethod {
    public:
        using ViewType = view::interfaces::settings::CalculationMethod;

    public:
        explicit CalculationMethod(ViewType& view);

        void update_calculation_method_view(model::measurement::Processing::CalculationMethod method) const noexcept;

        static void update_calculation_method_model(const std::string& method) noexcept;

        void update_detection_method_view(model::measurement::Processing::DetectionMethod method) const noexcept;

        static void update_detection_method_model(const std::string& method) noexcept;

        void update_resonance_frequency_view(double frequency) const noexcept;

        static void update_resonance_frequency_model(const std::string& frequency) noexcept;

        void update_time_constant_view(double time_constant) const noexcept;

        static void update_time_constant_model(const std::string& time_constant) noexcept;

    private:
        ViewType& view;
    };
}

#endif  // GUI_PRESENTER_SETINGS_CALCULATION_METHOD_H
