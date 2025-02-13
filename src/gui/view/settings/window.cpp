#include "gui/view/settings/window.h"
#include "helper/logging.h"

#include <QGridLayout>
#include <QScreen>


gui::view::settings::Window::Window():
QDialog(),
filter(new Filter),
calculation_method(new CalculationMethod),
measurement(new Measurement),
general(new General),
grid(new QGridLayout)
{
    setWindowTitle("Settings");
    setWindowIcon(QIcon("assets/images/Settings.png"));
    grid->addWidget(filter.get(), 0, 0);
    grid->addWidget(calculation_method.get(), 1, 0);
    grid->addWidget(measurement.get(), 0, 1);
    grid->addWidget(general.get(), 1, 1);
    setLayout(grid.get());
    setFixedSize(1100, 375);
    logging::debug("Settings Window initialised");
}
