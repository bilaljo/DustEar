#include "gui/presenter/general_purpose/logging.h"

#include <string>

#include "helper/logging.h"


gui::presenter::general_purpose::Logging::Logging(viewType& view)
: view(view),
  model(modelType{})
{
    auto update = [this](const std::string& message){ update_messages(message);};
    model.add_observer(update);
    logging::debug("Presenter of logging initialised");
}

void gui::presenter::general_purpose::Logging::update_messages(const std::string& message) noexcept {
    if (message.find("ERROR") != std::string::npos) [[unlikely]] {
        view.set_color("red");
    } else if (message.find("INFO") != std::string::npos) [[likely]] {
        view.set_color("green");
    } else if (message.find("WARNING") != std::string::npos) [[unlikely]] {
        view.set_color("orange");
    } else if (message.find("DEBUG") != std::string::npos) [[unlikely]] {
        view.set_color("blue");
    } else if (message.find("CRITICAL") != std::string::npos) [[unlikely]] {
        view.set_color("darkred");
    }
    view.update_logging(message);
    if (model.logging_entries > MAX_LOG_HISTORY_LENGTH) {
        // FIXME: Open discussion. Should we limit the log?
    }
}
