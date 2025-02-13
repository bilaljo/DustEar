#ifndef UNTITLED_MAIN_WINDOW_H
#define UNTITLED_MAIN_WINDOW_H


#include <gtkmm-3.0/gtkmm.h>

#include "gui/view/tabs/home.h"
#include "gui/view/general_purpose/logging.h"
#include "gui/view/tabs/distribution.h"



namespace gui::view {
    class MainWindow: public Gtk::Window {
    public:
        MainWindow();

    private:
        void init_tabs();

    public:
        struct Tabs {
            tabs::Home* home;
            tabs::Distribution* amplitudes;
            tabs::Distribution* frequencies;
            tabs::Distribution* spectrum;
            tabs::Distribution* pulses;

        };

        Tabs tabs;

    private:

        general_purpose::Logging *logging;

        Gtk::Notebook *tab_bar;
    };
}


#endif  // UNTITLED_MAIN_WINDOW_H
