#ifndef GUI_VIEW_GENERAL_PURPOSE_STATE_BUTTON_H
#define GUI_VIEW_GENERAL_PURPOSE_STATE_BUTTON_H

#include <string>

#include <gtkmm/button.h>

#include "gui/view/interface/general_purpose/state_button.h"


namespace gui::view::general_purpose {
    using ButtonState = interface::general_purpose::State;

    struct StateButton: public interface::general_purpose::StateButton, Gtk::Button {
        explicit StateButton(const std::string& title);

        void enable() noexcept override;

        void disable() noexcept override;
    };
}


#endif  // GUI_VIEW_GENERAL_PURPOSE_STATE_BUTTON_H
