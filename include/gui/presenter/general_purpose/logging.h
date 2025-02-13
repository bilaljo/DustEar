#ifndef GUI_PRESENTER_GENERAL_PURPOSE_LOGGING_H
#define GUI_PRESENTER_GENERAL_PURPOSE_LOGGING_H

#include "gui/model/general_purpose/logging.h"
#include "gui/view/interface/general_purpose/logging.h"
#include "helper/ring_buffer.h"


namespace gui::presenter::general_purpose {
    class Logging {
    private:
        using viewType = view::interfaces::general_purpose::Logging;
        using modelType = model::general_purpose::Logging;

    public:
        explicit Logging(viewType& view);

    private:
        void update_messages(const std::string& messages) noexcept;

    private:
        viewType& view;
        modelType model;

        static constexpr std::size_t MAX_LOG_HISTORY_LENGTH = 50;
    };
}

#endif  // GUI_PRESENTER_GENERAL_PURPOSE_LOGGING_H
