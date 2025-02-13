#include "gui/presenter/settings/measurement.h"
#include "gui/model/event_bus.h"

#include <format>
#include <any>
#include <helper/logging.h>


gui::presenter::settings::Measurement::Measurement(viewType &view): view(view) {
    auto update_trigger_level = [this](const std::any& t){
        update_trigger_level_view(std::any_cast<const double>(t));};
    model::event_bus.register_topic("trigger_level_changed");
    model::event_bus.register_subscriber(update_trigger_level, "trigger_level_changed");
    auto update_dead_time = [this](const std::any& d){
        update_dead_time_view(std::any_cast<const std::size_t>(d));};
    model::event_bus.register_topic("dead_time_changed");
    model::event_bus.register_subscriber(update_dead_time, "dead_time_changed");
    auto trigger_level_unit_update = [this](const std::any& d){
        update_trigger_level_unit(std::any_cast<const bool>(d));};
    model::event_bus.register_subscriber(trigger_level_unit_update, "correlation_enabled");
}

void gui::presenter::settings::Measurement::update_trigger_level_view(const double trigger_level) const noexcept {
    if (correlation) {
        view.set_trigger_level(std::format("{:1.2f}", trigger_level * 100));
    } else {
        view.set_trigger_level(std::format("{:1.3f}", trigger_level));
    }
}

void gui::presenter::settings::Measurement::update_dead_time_view(const std::size_t dead_time) const noexcept {
    view.set_dead_time(std::to_string(dead_time));
}

void gui::presenter::settings::Measurement::update_trigger_level(const std::string &number) const noexcept {
    if (number.empty()) {
        logging::error("No number given");
    } else {
        if (std::stod(number) <= 0) {
            return;
        }
        if (correlation) {
            model::event_bus.publish("trigger_level_update", std::stod(number) / 100);
        } else {
            model::event_bus.publish("trigger_level_update", std::stod(number));
        }
    }
}

void gui::presenter::settings::Measurement::update_dead_time(const std::string &number) noexcept {
    if (number.empty()) {
        logging::error("No number given");
    } else {
        model::event_bus.publish("dead_time_update", static_cast<std::size_t>(std::stoi(number)));
    }
}

void gui::presenter::settings::Measurement::save_config() noexcept {
    model::event_bus.emit("save_config");
}

void gui::presenter::settings::Measurement::update_trigger_level_unit(const bool correlation) noexcept {
    if (correlation) {
        view.set_trigger_level_unit("%");
    } else {
        view.set_trigger_level_unit("V");
    }
    this->correlation = correlation;
}
