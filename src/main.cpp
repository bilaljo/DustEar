#include "gui/view/main_window.h"
#include "helper/logging.h"

#include <QApplication>
#include <QStyleHints>


int main(int argc, char* argv[]) {
    logging::log_level = logging::LogLevel::debug;
    QApplication app{argc, argv};
    logging::info("Application started");
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        logging::debug("Set theme to dark theme");
        QFile file("assets/style_sheets/darkstyle.qss");
        file.open(QFile::ReadOnly);
        const QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    } else {
        logging::debug("Set theme to light theme");
        QFile file("assets/style_sheets/lightstyle.qss");
        file.open(QFile::ReadOnly);
        const QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }
    const auto main_window = new gui::view::MainWindow;
    logging::debug("Main Window started");
    main_window->show();
    const auto end = QGuiApplication::exec();
    logging::info("Application finished");
    return end;
}
