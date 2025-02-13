#ifndef GUI_VIW_TOOLBAR_GENERAL_PURPOSE_H
#define GUI_VIW_TOOLBAR_GENERAL_PURPOSE_H

#include <QToolBar>
#include <QAction>
#include <QFileDialog>

#include "gui/view/settings/window.h"
#include "gui/view/interface/general_purpose/toolbar.h"
#include "gui/presenter/general_purpose/toolbar.h"


namespace gui::view::general_purpose {
    class ToolBar: public QToolBar, public view::interfaces::general_purpose::ToolBar {
    public:
        ToolBar();

        void show_settings() const noexcept override;

        void set_running() noexcept override;

        void set_stopped() noexcept override;

        void process() noexcept override;


    private:
        void init_icons() const noexcept;

        void init_actions() noexcept;

        [[nodiscard]] std::string open_binary_file() const noexcept;

        [[nodiscard]] std::string open_csv_file() const noexcept;

        [[nodiscard]] std::string open_directory() const noexcept;

    private:
        using presenterType = presenter::general_purpose::Toolbar;

        struct Actions {
            QAction* run;
            QAction* process;
            QAction* plot;
            QAction* settings;
            QAction* clean;
            QAction* directory;
        };

        Actions actions;

        settings::Window* settings;

        std::pair<QWidget*, QWidget*> spacerWidgets{new QWidget, new QWidget};

        QFileDialog file_dialog;

        presenterType presenter = presenterType{*this};
    };
}

#endif // GUI_VIW_TOOLBAR_GENERAL_PURPOSE_H
