#ifndef GUI_PRESENTER_GENERAL_PURPOSE_TOOLBAR_H
#define GUI_PRESENTER_GENERAL_PURPOSE_TOOLBAR_H

#include "gui/view/interface/general_purpose/toolbar.h"
#include "gui/model/measurement/processing.h"
#include "gui/model/driver/daq.h"

namespace gui::presenter::general_purpose {
    class Toolbar {
    private:
        using viewType = gui::view::interfaces::general_purpose::ToolBar;

    public:
        explicit Toolbar(viewType &view);

        void on_run() noexcept;

        void on_settings() const noexcept;

        void process(const std::string& file_path) noexcept;

        void on_directory(const std::string& directory) const noexcept;

        void on_plot(const std::string& file_path) noexcept;

    private:
        viewType& view;

        model::measurement::Processing processing;

        model::driver::DAQ daq;

    };
}

#endif  // GUI_PRESENTER_GENERAL_PURPOSE_TOOLBAR_H
