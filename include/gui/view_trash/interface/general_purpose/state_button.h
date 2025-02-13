#ifndef GUI_VIEW_INTERFACE_GENERAL_PURPOSE_STATE_BUTTON_H
#define GUI_VIEW_INTERFACE_GENERAL_PURPOSE_STATE_BUTTON_H


namespace gui::view::interface::general_purpose {
    enum class State {
        enabled,
        disabled
    };

    struct StateButton {
        State state = State::disabled;

        virtual void enable() noexcept = 0;

        virtual void disable() noexcept = 0;
    };
}

#endif  // GUI_VIEW_INTERFACE_GENERAL_PURPOSE_STATE_BUTTON_H
