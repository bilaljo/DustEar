#pragma once

#include "gui/view/interface/tabs/time_series.h"

#include <memory>

#include <QTabWidget>

#include "gui/view/charts/line_chart.h"
#include "gui/presenter/tabs/time_series.h"

namespace gui::view::tabs {
    class TimeSeries final : interfaces::tabs::TimeSeries, public QTabWidget {
    public:
        TimeSeries(std::string&& name, QString x_label, QString y_label);

        void update_chart(const std::vector<double>& x, const std::vector<double>& y) const noexcept override;

        [[nodiscard]] std::string get_name() const noexcept override;

    private:
        std::unique_ptr<charts::LineChart> line_chart;

        static constexpr int BUFFER_SIZE = 10000;

        std::string name;

        presenter::tabs::TimeSeries presenter{*this};
    };
}
