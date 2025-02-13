#include "gui/model/general_purpose/logging.h"
#include "helper/logging.h"

gui::model::general_purpose::Logging::Logging() {
    const auto handler = [this](const std::string& s){ return this->handler(s); };
    logging::add_handler(handler);
}

void gui::model::general_purpose::Logging::handler(const std::string& message) {
    logging_entries++;
    for (const auto& observer: observers) {
        observer(message);
    }
}

void gui::model::general_purpose::Logging::add_observer(const std::function<void(const std::string&)>& observer) noexcept {
    observers.push_back(observer);
}
