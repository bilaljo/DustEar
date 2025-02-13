#include "gui/model/measurement/configuration.h"

#include <fstream>

#include "nlohmann/json.hpp"

void gui::model::measurement::Configuration::load(std::string_view file_path) noexcept {
    std::ifstream file(file_path.data());
    nlohmann::json data = nlohmann::json::parse(file);
    trigger_detection = data["measurement"]["trigger_detection"].get<bool>();

    ai_detection = data["measurement"]["ai_detection"].get<bool>();

    auto_clean = data["measurement"]["auto_clean"].get<bool>();

    working_directory = data["measurement"]["working_directory"].get<std::string>();
}

void gui::model::measurement::Configuration::save() noexcept {

}

void gui::model::measurement::Configuration::save_as(std::string_view file_path) noexcept {

}
