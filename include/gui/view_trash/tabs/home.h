#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "gui/view/settings/window.h"
#include "gui/view/interface/tabs/home.h"
#include "gui/view/general_purpose/state_button.h"
#include "gui/presenter/tabs/home.h"

#include <gtkmm-3.0/gtkmm.h>

#include <thread>


namespace gui::view::tabs {
    class Home: public Gtk::Box, public view::interface::tabs::Home {
    public:
        Home();

        void show_settings() const noexcept override;

        void set_running() noexcept override;

        void set_stopped() noexcept override;


    private:
        void init_frames() noexcept;

        void init_buttons() noexcept;

    private:
        using presenterType = presenter::tabs::Home;

        struct Frames {
            Gtk::Frame* measurement;
            Gtk::Frame* configuration;
            Gtk::Frame* processing;
        };

        Frames frames{};

        general_purpose::StateButton run_button;

        general_purpose::StateButton connect_button;

        Gtk::Button settings_button;

        Gtk::Button calculate_button;

        settings::Window* settings;

        presenterType presenter = presenterType{*this};
    };
}

#endif // TOOLBAR_H
