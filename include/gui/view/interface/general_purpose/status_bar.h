#pragma once

#include <string>

namespace gui::view::interfaces::general_purpose {
    class StatusBar {
    public:
        virtual ~StatusBar() = default;

        virtual void update_directory(const std::string& path) const noexcept = 0;

        virtual void update_particle_concentration(const std::string& number) const noexcept = 0;
    };
}
