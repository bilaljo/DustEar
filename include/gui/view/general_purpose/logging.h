#ifndef GUI_VIEW_GENERAL_PURPOSE_LOGGING_H
#define GUI_VIEW_GENERAL_PURPOSE_LOGGING_H

#include <QDockWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QString>

#include "gui/view/interface/general_purpose/logging.h"

#include "gui/presenter/general_purpose/logging.h"

#include <memory>

namespace gui::view::general_purpose {
    class Logging: public QDockWidget, interfaces::general_purpose::Logging {
    public:
        Logging();

        void set_color(std::string_view color) noexcept override;

    private:
        void update_logging(const std::string& log) noexcept override;

        static constexpr int LIGHT_BLUE = 0x14aaff;

    private:
        using presenterType = presenter::general_purpose::Logging;

        QScrollArea *scroll = new QScrollArea;

        QTextEdit * logging_window = new QTextEdit;

        presenterType presenter = presenterType{*this};

    };
}

#endif  // GUI_VIEW_GENERAL_PURPOSE_LOGGING_H
