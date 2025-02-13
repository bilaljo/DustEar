#ifndef GUI_VIEW_INTERFACE_GENERAL_PURPOSE_LOGGING_H
#define GUI_VIEW_INTERFACE_GENERAL_PURPOSE_LOGGING_H

#include <string>

namespace gui::view::interface::general_purpose {
    class Logging {
    public:
        virtual void update_logging(const std::string& log) noexcept = 0;

        virtual void set_color(std::string_view color) noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACE_GENERAL_PURPOSE_LOGGING_H
