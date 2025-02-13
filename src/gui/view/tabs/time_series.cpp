#include "gui/view/tabs/time_series.h"

#include <QGridLayout>

gui::view::tabs::TimeSeries::TimeSeries(std::string&& name, QString x_label, QString y_label):
QTabWidget(),
line_chart(new charts::LineChart{std::move(x_label), std::move(y_label), 0, BUFFER_SIZE}),
name(name)
{
    setLayout(new QGridLayout);
    layout()->addWidget(line_chart.get());
}

void gui::view::tabs::TimeSeries::update_chart(const std::vector<double>& x, const std::vector<double>& y) const noexcept {
    line_chart->draw({x, y});
}

std::string gui::view::tabs::TimeSeries::get_name() const noexcept {
    return name;
}
