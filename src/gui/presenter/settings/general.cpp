#include "gui/presenter/settings/general.h"
#include "gui/model/event_bus.h"

gui::presenter::settings::General::General(view::interfaces::settings::General &view): view(view) {
    auto update_save_raw_data = [this](const std::any& r){update_save_raw_data_view(std::any_cast<bool>(r));};
    model::event_bus.register_subscriber(update_save_raw_data, "save_raw_data_changed");
    auto update_save_pulses = [this](const std::any& r){update_save_pulses_view(std::any_cast<bool>(r));};
    model::event_bus.register_subscriber(update_save_pulses, "save_pulses_changed");
    model::event_bus.register_topic("save_pulses_update");
    model::event_bus.register_topic("save_raw_data_update");
}

void gui::presenter::settings::General::update_save_raw_data_view(const bool state) noexcept {
    view.set_save_raw_data(state);
}

void gui::presenter::settings::General::update_save_pulses_view(const bool state) noexcept {
    view.set_save_pulses(state);
}

void gui::presenter::settings::General::update_save_raw_data_model(bool state) noexcept {
    model::event_bus.publish("save_raw_data_update", state);
}

void gui::presenter::settings::General::update_save_pulses_model(bool state) noexcept {
    model::event_bus.publish("save_pulses_update", state);
}

void gui::presenter::settings::General::save_settings_model() noexcept {
    model::event_bus.emit("save_settings");
}
