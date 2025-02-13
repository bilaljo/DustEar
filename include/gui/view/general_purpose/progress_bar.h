#ifndef GUI_VIEW_GENERAL_PURPOSE_PROGRESSBAR_H
#define GUI_VIEW_GENERAL_PURPOSE_PROGRESSBAR_H

#include <QProgressBar>

#include "gui/view/interface/general_purpose/progress_bar.h"


namespace gui::view::general_purpose {
    class ProgressBar: interface::general_purpose::ProgressBar, QProgressBar {
    public:
        ProgressBar(int min_value, int max_value);

        void update(int value) noexcept override;

        void reset() noexcept override;

    private:

    };
}


#endif // GUI_VIEW_GENERAL_PURPOSE_PROGRESSBAR_H
