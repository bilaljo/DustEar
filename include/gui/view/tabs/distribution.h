#pragma once

#include <QTabWidget>

#include "gui/view/interface/tabs/distribution.h"
#include "gui/presenter/tabs/distribution.h"

#include "gui/view/charts/histogram.h"



namespace gui::view::tabs {
class Distribution final : public interfaces::tabs::Distribution, public QTabWidget {
    public:
        explicit Distribution(QString x_label, QString y_label);

        void draw(const std::vector<double>& bins, const helper::Histogram<double>::Range& range) noexcept override;

    private:
        charts::Histogram* histogram_chart;

        presenter::tabs::Distribution presenter;
 };
}
