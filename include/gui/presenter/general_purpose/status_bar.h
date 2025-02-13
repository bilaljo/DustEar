#ifndef GUI_PRESENTER_GENERAL_PURPOSE_STATUS_BAR_H
#define GUI_PRESENTER_GENERAL_PURPOSE_STATUS_BAR_H

#include "gui/view/interface/general_purpose/status_bar.h"

#include <string>

namespace gui::presenter::general_purpose {
    class StatusBar {
    public:
        using ViewType = view::interfaces::general_purpose::StatusBar;

    public:
        explicit StatusBar(ViewType& view);

    private:
        void update_destination_folder(const std::string& path) const;

        void update_particle_concentration(double concentration) const;

    private:
        ViewType& view;
    };
}


#endif  // GUI_PRESENTER_GENERAL_PURPOSE_STATUS_BAR_H
