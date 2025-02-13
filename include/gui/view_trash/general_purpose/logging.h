#ifndef GUI_VIEW_GENERAL_PURPOSE_LOGGING_H
#define GUI_VIEW_GENERAL_PURPOSE_LOGGING_H

#include <gtkmm/textview.h>
#include <gtkmm/frame.h>

#include "gui/view/interface/general_purpose/logging.h"

#include "gui/presenter/general_purpose/logging.h"


namespace gui::view::general_purpose {
    class Logging: public Gtk::Frame, interface::general_purpose::Logging {
    public:
        Logging();

        void set_color(std::string_view color) noexcept override;

    private:
        void update_logging(const std::string& log) noexcept override;

        static constexpr int LIGHT_BLUE = 0x14aaff;

    private:
        Gtk::TextView logging_text;

        using presenterType = presenter::general_purpose::Logging;

        presenterType presenter = presenterType{*this};

    };
}

#endif  // GUI_VIEW_GENERAL_PURPOSE_LOGGING_H
