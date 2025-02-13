#ifndef GUI_MODEL_GENERAL_PURPOSE_LOGGING_H
#define GUI_MODEL_GENERAL_PURPOSE_LOGGING_H

#include "helper/ring_buffer.h"
#include <string_view>
#include <functional>

namespace gui::model::general_purpose {
    class Logging {
    public:
        Logging();

        void add_observer(const std::function<void(const std::string&)>& observer) noexcept;

    public:
        std::size_t logging_entries = 0;

    private:
        void handler(const std::string& message);


    private:
        std::vector<std::function<void(const std::string&)>> observers;
    };
}

#endif  // GUI_MODEL_GENERAL_PURPOSE_LOGGING_H
