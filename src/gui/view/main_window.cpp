#include "gui/view/main_window.h"
#include "helper/logging.h"
#include "gui/view/tabs/distribution.h"

#include <QScreen>
#include <QTabBar>
#include <memory>
#include <QGuiApplication>
#undef slots

gui::view::MainWindow::MainWindow():
QMainWindow(),
tabs(Tabs{.amplitudes = std::make_unique<tabs::Distribution>("Mass [pg]", "dN/dm [1/pgcm³]"),
          .frequencies = std::make_unique<tabs::TimeSeries>("Frequency", "Time [s]", "Frequency [kHz]"),
          .pulses = std::make_unique<tabs::TimeSeries>("Pulses", "Time [s]", "Signal [V]"),
          }),
statusbar(new general_purpose::StatusBar())
{
    setWindowTitle("DustEar");
    setWindowIcon(QIcon("assets/images/logo.ico"));
    this->toolbar = new general_purpose::ToolBar;
    this->logging = new general_purpose::Logging;
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, logging);
    this->tab_bar = new QTabWidget{};
    auto font = this->tab_bar->font();
    font.setPointSize(12);
    this->tab_bar->setFont(font);
    // See https://stackoverflow.com/questions/42252403/how-to-expand-tabs-in-qtabwidget
    this->tab_bar->tabBar()->setExpanding(true);
    this->tab_bar->tabBar()->setDocumentMode(true);
    this->addToolBar(Qt::ToolBarArea::BottomToolBarArea, toolbar);
    this->setCentralWidget(this->tab_bar);
    this->setStatusBar(statusbar.get());
    init_tabs();
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QSize screenSize = screen->size();
    resize(screenSize.width() * 0.7, screenSize.height() * 0.8);
    logging::debug("Main Window initialised");
}

void gui::view::MainWindow::init_tabs() const {
    tab_bar->addTab(tabs.amplitudes.get(), "Masses");
    tab_bar->addTab(tabs.frequencies.get(), "Frequencies");
    //tab_bar->addTab(tabs.pulses.get(), "Pulses");
}
