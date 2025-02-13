#pragma once

namespace gui::view::interfaces::general_purpose {
    class ProgressBar {
    public:
        virtual ~ProgressBar() = default;

        virtual void update(int value) noexcept = 0;

        virtual void reset() noexcept = 0;

    private:

    };
}
