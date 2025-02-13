#include "gui/presenter/general_purpose/status_bar.h"

#include "gui/model/event_bus.h"
#include "helper/logging.h"

#include <filesystem>

gui::presenter::general_purpose::StatusBar::StatusBar(ViewType& view): view(view) {
    auto update = [this](const std::any& path){return update_destination_folder(std::any_cast<const std::string&>(path));};
    model::event_bus.register_topic("destination_folder");
    model::event_bus.register_subscriber(update, "destination_folder");
    const auto path = std::filesystem::current_path().string();
    update_destination_folder(path);
    auto update_particle_concentration_event = [this](const std::any& concentration){return update_particle_concentration(std::any_cast<const double>(concentration));};
    model::event_bus.register_subscriber(update_particle_concentration_event, "particle_concentration");

}

void gui::presenter::general_purpose::StatusBar::update_destination_folder(const std::string& path) const {
    logging::debug("Set destination directory to {}", path);
    view.update_directory(path);
}

void gui::presenter::general_purpose::StatusBar::update_particle_concentration(const double concentration) const {
    view.update_particle_concentration(std::format("{:.2e}", concentration));
}
