#pragma once

namespace gui::view::interfaces::general_purpose {
    class ToolBar {
    public:
        virtual ~ToolBar() = default;

        virtual void show_settings() const noexcept = 0;

        virtual void set_running() noexcept = 0;

        virtual void set_stopped() noexcept = 0;

        virtual void process() noexcept = 0;
    };
}
