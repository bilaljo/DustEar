#ifndef GUI_VIEW_INTERFACE_GENERAL_PURPOSE_TOOLBAR_H
#define GUI_VIEW_INTERFACE_GENERAL_PURPOSE_TOOLBAR_H

#include "gui/view/interface/general_purpose/state_button.h"

namespace gui::view::interface::tabs {
    class Home {
    public:
        using ButtonState = interface::general_purpose::State;

        virtual void show_settings() const noexcept = 0;

        virtual void set_running() noexcept = 0;

        virtual void set_stopped() noexcept = 0;
    };
}

#endif // GUI_VIEW_INTERFACE_GENERAL_PURPOSE_TOOLBAR_H
