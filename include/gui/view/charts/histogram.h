#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <vector>

#include "helper/histogram.h"
#include "gui/view/charts/chart.h"

namespace gui::view::charts {
    class Histogram: public Chart {
    public:
        explicit Histogram(QString x_label, QString y_label);

        void draw(const std::vector<double>& yaxis, const helper::Histogram<double>::Range& range) noexcept;

    protected:
        void paintEvent(QPaintEvent *event) override;

        void drawXTicks(QPainter& painter, int i, int offset) const;

        void draw_histogram(QPainter& painter);

        void draw_y_ticks(QPainter& painter, int i, int offset) const;

    private:
        QString x_label;

        std::vector<double> yaxis;

        helper::Histogram<double>::Range range{};

        double y_min;

        double y_max;
    };
}

#endif // HISTOGRAMWIDGET_H
