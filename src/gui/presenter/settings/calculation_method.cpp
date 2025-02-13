#include "gui/presenter/settings/calculation_method.h"

#include "gui/model/event_bus.h"

#include <format>

gui::presenter::settings::CalculationMethod::CalculationMethod(ViewType &view): view(view) {
    view.set_calculation_methods({"Peek to Peek",
                                 "LSQ",
                                 });
    view.set_detection_methods({"Trigger Level",
                                 "Crosscorrelation",
                                 });
    model::event_bus.register_topic("calculation_method_changed");
    model::event_bus.register_topic("detection_method_update");
    model::event_bus.register_topic("correlation_enabled");
    auto update_calculation_method = [this](const std::any& m){
        update_calculation_method_view(std::any_cast<model::measurement::Processing::CalculationMethod>(m));
    };
    model::event_bus.register_subscriber(update_calculation_method, "calculation_method_changed");
    auto update_detection_method = [this](const std::any& m){
        update_detection_method_view(std::any_cast<model::measurement::Processing::DetectionMethod>(m));
    };
    model::event_bus.register_subscriber(update_detection_method, "detection_method_changed");
    auto update_resonance_frequency = [this](const std::any& m){update_resonance_frequency_view(std::any_cast<double>(m));};
    model::event_bus.register_subscriber(update_resonance_frequency, "resonance_frequency_changed");
    auto update_time_constant = [this](const std::any& m){update_time_constant_view(std::any_cast<double>(m));};
    model::event_bus.register_subscriber(update_time_constant, "time_constant_changed");
}

void gui::presenter::settings::CalculationMethod::update_calculation_method_view(const model::measurement::Processing::CalculationMethod method) const noexcept {
    switch (method) {
        case model::measurement::Processing::CalculationMethod::peek_to_peek:
            view.set_calculation_method("Peek to Peek");
            break;
        case model::measurement::Processing::CalculationMethod::LSQ:
            view.set_calculation_method("LSQ");
            break;
    }
}

void gui::presenter::settings::CalculationMethod::update_calculation_method_model(const std::string &method) noexcept {
    model::measurement::Processing::CalculationMethod new_method{};
    if (method == "Peek to Peek") {
        new_method = model::measurement::Processing::CalculationMethod::peek_to_peek;
    } else if (method == "LSQ") {
        new_method = model::measurement::Processing::CalculationMethod::LSQ;
    }
    model::event_bus.publish("calculation_method_update", new_method);
}

void gui::presenter::settings::CalculationMethod::update_detection_method_view(const model::measurement::Processing::DetectionMethod method) const noexcept {
    switch (method) {
        case model::measurement::Processing::DetectionMethod::trigger_level:
            view.set_calculation_method("Trigger Level");
            model::event_bus.publish("correlation_enabled", false);
            break;
        case model::measurement::Processing::DetectionMethod::crosscorrelation:
            view.set_detection_method("Crosscorrelation");
            model::event_bus.publish("correlation_enabled", true);
            break;
    }
}

void gui::presenter::settings::CalculationMethod::update_detection_method_model(const std::string &method) noexcept {
    model::measurement::Processing::DetectionMethod new_method{};
    if (method == "Trigger Level") {
        new_method = model::measurement::Processing::DetectionMethod::trigger_level;
    } else if (method == "Crosscorrelation") {
        new_method = model::measurement::Processing::DetectionMethod::crosscorrelation;
    }
    model::event_bus.publish("detection_method_update", new_method);
}

void gui::presenter::settings::CalculationMethod::update_resonance_frequency_view(const double frequency) const noexcept {
    view.set_frequency(std::format("{:.2f}", frequency));
}

void gui::presenter::settings::CalculationMethod::update_resonance_frequency_model(const std::string &frequency) noexcept {
    model::event_bus.publish("resonance_frequency_update", std::stod(frequency));
}

void gui::presenter::settings::CalculationMethod::update_time_constant_view(const double time_constant) const noexcept {
    view.set_time_constant(std::format("{:.2f}", time_constant));
}

void gui::presenter::settings::CalculationMethod::update_time_constant_model(const std::string &time_constant) noexcept {
    model::event_bus.publish("time_constant_update", std::stod(time_constant));
}
