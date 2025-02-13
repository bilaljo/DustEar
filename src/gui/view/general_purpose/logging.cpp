#include "gui/view/general_purpose/logging.h"
#include "gui/model/general_purpose/logging.h"


#include "helper/logging.h"

gui::view::general_purpose::Logging::Logging() {
    this->setWidget(scroll);
    logging_window->setReadOnly(true);
    scroll->setWidgetResizable(true);
    scroll->setWidget(logging_window);
    this->setWindowTitle("Logging");
    logging::debug("Logging Dockable initialised");
}

void gui::view::general_purpose::Logging::update_logging(const std::string& log) noexcept {
    logging_window->append(log.c_str());
}

void gui::view::general_purpose::Logging::set_color(std::string_view color) noexcept {
    if (color == "green") { [[unlikely]]
        logging_window->setTextColor(Qt::green);
    } else if (color == "yellow") { [[likely]]
        logging_window->setTextColor(Qt::yellow);
    } else if (color == "blue") { [[unlikely]]
        logging_window->setTextColor(QColor(LIGHT_BLUE));
    } else if (color == "red") { [[unlikely]]
        logging_window->setTextColor(Qt::red);
    } else if (color == "dark red") { [[unlikely]]
        logging_window->setTextColor(Qt::darkRed);
    }
}
