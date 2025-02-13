#include "gui/view/tabs/distribution.h"

#include <QTabWidget>
#include <QGridLayout>

#include <memory>
#include <utility>

#include "helper/histogram.h"

gui::view::tabs::Distribution::Distribution(QString x_label, QString y_label):
QTabWidget(),
histogram_chart(new charts::Histogram{std::move(x_label), std::move(y_label)}),
presenter(*this)
{
    setLayout(new QGridLayout);
    layout()->addWidget(histogram_chart);
}

void gui::view::tabs::Distribution::draw(const std::vector<double>& bins, const helper::Histogram<double>::Range& range) noexcept {
    histogram_chart->draw(bins, range);
}
