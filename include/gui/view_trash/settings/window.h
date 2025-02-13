#ifndef GUI_VIEW_SETTINGS_WINDOW
#define GUI_VIEW_SETTINGS_WINDOW

#include <gtkmm-3.0/gtkmm.h>


#include "gui/view/settings/measurement.h"


namespace gui::view::settings {
    class Window: public Gtk::Window {
        public:
        Window();

        private:
        settings::Measurement measurement;
    };
}


#endif  // GUI_VIEW_SETTINGS_WINDOW