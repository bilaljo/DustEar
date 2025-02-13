#ifndef GUI_VIEW_SETTINGS_WINDOW
#define GUI_VIEW_SETTINGS_WINDOW

#include <QDialog>

#include "gui/view/settings/filter.h"
#include "gui/view/settings/calculation_method.h"
#include "gui/view/settings/measurement.h"
#include "gui/view/settings/general.h"

#include <memory>

namespace gui::view::settings {
    class Window final : public QDialog {
        public:
        Window();

        ~Window() override = default;

        private:
        std::unique_ptr<Filter> filter;

        std::unique_ptr<CalculationMethod> calculation_method;

        std::unique_ptr<Measurement> measurement;

        std::unique_ptr<General> general;

        std::unique_ptr<QGridLayout> grid;
    };
}


#endif  // GUI_VIEW_SETTINGS_WINDOW