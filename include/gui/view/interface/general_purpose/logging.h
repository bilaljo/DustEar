#pragma once

#include <string>

namespace gui::view::interfaces::general_purpose {
    class Logging {
    public:
        virtual ~Logging() = default;

        virtual void update_logging(const std::string& log) noexcept = 0;

        virtual void set_color(std::string_view color) noexcept = 0;
    };
}
