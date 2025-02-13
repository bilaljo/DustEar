#ifndef GUI_PRESENTER_PROGESS_BAR_H
#define GUI_PRESENTER_PROGESS_BAR_H

#include "gui/view/interface/general_purpose/progress_bar.h"

namespace gui::presenter::general_purpose {
    class ProgressBar {
    private:
        using viewType = gui::view::interfaces::general_purpose::ProgressBar;

    public:
        explicit ProgressBar(viewType& view);

    private:
        viewType& view;

    };
}

#endif  // GUI_PRESENTER_PROGESS_BAR_H
