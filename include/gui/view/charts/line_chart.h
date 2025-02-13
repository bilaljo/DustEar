#pragma once

#include "gui/view/charts/chart.h"

namespace gui::view::charts {
    class LineChart final: public Chart {
    public:
        explicit LineChart(QString x_label, QString y_label, double x_min, double x_max);

        void draw(const std::pair<std::vector<double>, std::vector<double>>& axis) noexcept;

    protected:
        void paintEvent(QPaintEvent *event) override;

        void drawXTicks(QPainter& painter, int i, int offset) const;

        void draw_y_ticks(QPainter& painter, int i, int offset) const;

        void draw_chart(QPainter& painter);

    protected:
        std::pair<std::vector<double>, std::vector<double>> axis;

        double x_min;

        double x_max;

        double y_min;

        double y_max;
    };
}
