#ifndef UNTITLED_MAIN_WINDOW_H
#define UNTITLED_MAIN_WINDOW_H

#include <memory>

#include <QMainWindow>
#include <QTabWidget>

#include "gui/view/general_purpose/toolbar.h"
#include "gui/view/general_purpose/logging.h"
#include "gui/view/tabs/distribution.h"
#include "gui/view/tabs/time_series.h"

#include "gui/view/general_purpose/status_bar.h"


namespace gui::view {
    class MainWindow: public QMainWindow {
    public:
        MainWindow();

    private:
        void init_tabs() const;

    public:
        struct Tabs {
            std::unique_ptr<tabs::Distribution> amplitudes;
            std::unique_ptr<tabs::TimeSeries> frequencies;
            std::unique_ptr<tabs::TimeSeries> pulses;
        };

        Tabs tabs;

    private:
        general_purpose::ToolBar *toolbar;

        general_purpose::Logging *logging;

        QTabWidget *tab_bar;

        std::unique_ptr<general_purpose::StatusBar> statusbar;
    };
}


#endif  // UNTITLED_MAIN_WINDOW_H
