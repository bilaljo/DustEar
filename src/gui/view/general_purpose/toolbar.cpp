#include "gui/view/general_purpose/toolbar.h"

#include <QIcon>
#include <QFileDialog>
#include <QString>

#include "helper/logging.h"

gui::view::general_purpose::ToolBar::ToolBar():
    actions{.run = new QAction("Run"),
            .process =  new QAction("Process"),
            .plot =  new QAction("Plot"),
            .settings = new QAction("Settings"),
            .clean = new QAction("Clean"),
            .directory = new QAction("Directory"),
    },
    settings(new gui::view::settings::Window)
{
    init_icons();
    init_actions();
    this->setIconSize(QSize(30, 30));
    this->setStyleSheet("QToolBar{spacing:12px;}");
    spacerWidgets.first->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    //spacerWidgets.first->setVisible(true);
    spacerWidgets.second->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    //spacerWidgets.second->setVisible(true);
    logging::debug("Toolbar initialised");
}

void gui::view::general_purpose::ToolBar::init_icons() const noexcept  {
    //this->addWidget(spacerWidgets.first);
    actions.run->setIcon(QIcon("assets/images/run.png"));
    actions.process->setIcon(QIcon("assets/images/processing.svg"));
    actions.plot->setIcon(QIcon("assets/images/plot.png"));
    actions.settings->setIcon(QIcon("assets/images/settings.png"));
    actions.directory->setIcon(QIcon("assets/images/directory.svg"));
    actions.clean->setIcon(QIcon("assets/images/clean.png"));
    //this->addWidget(spacerWidgets.second);
}

void gui::view::general_purpose::ToolBar::init_actions() noexcept {
    connect(actions.run, &QAction::triggered, [this](){return presenter.on_run();});
    this->addAction(actions.run);
    connect(actions.process, &QAction::triggered, [this](){return presenter.process(open_binary_file());});
    this->addAction(actions.process);
    connect(actions.plot, &QAction::triggered, [this](){return presenter.on_plot(open_csv_file());});
    this->addAction(actions.plot);
    connect(actions.settings, &QAction::triggered, [this](){return presenter.on_settings();});
    this->addAction(actions.settings);
    this->addAction(actions.directory);
    connect(actions.directory, &QAction::triggered, [this](){return presenter.on_directory(open_directory());});
    // this->addAction(actions.clean);
}

void gui::view::general_purpose::ToolBar::show_settings() const noexcept {
    settings->show();
}

void gui::view::general_purpose::ToolBar::set_running() noexcept {
    this->actions.run->setIcon(QIcon("assets/images/stop.svg"));
}

void gui::view::general_purpose::ToolBar::set_stopped() noexcept {
    this->actions.run->setIcon(QIcon("assets/images/run.png"));
}

void gui::view::general_purpose::ToolBar::process() noexcept {
}

std::string gui::view::general_purpose::ToolBar::open_binary_file() const noexcept {
    return QFileDialog::getOpenFileName(this->parentWidget(),
                                        "Open Raw Data", ".",
                                        "All Files (*.*);;Binary Files (*.bin)").toStdString();
}

std::string gui::view::general_purpose::ToolBar::open_csv_file() const noexcept {
    return QFileDialog::getOpenFileName(this->parentWidget(),
                                        "Open Raw Data", ".",
                                        "All Files (*.*);;CSV Files (*.csv)").toStdString();
}

std::string gui::view::general_purpose::ToolBar::open_directory() const noexcept {
    return QFileDialog::getExistingDirectory(this->parentWidget(), "Set Directory", ".").toStdString();
}
