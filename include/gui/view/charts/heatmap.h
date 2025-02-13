#pragma once

#include "gui/view/charts/chart.h"

namespace gui::view::charts {
    class HeatMap final: public Chart {
    public:
        struct Resolution {
            int x;
            int y;
            int z;
        };

        struct Values {
            std::vector<double> x;
            std::vector<double> y;
            std::vector<std::vector<double>> z;
        };

    public:
        explicit HeatMap(QString x_label, QString y_label, QString colorbar_label,
                         Resolution&& resolution);

        void draw(const std::vector<double>& values) noexcept;

    protected:
        void paintEvent(QPaintEvent *event) override;

        void drawXTicks(QPainter& painter, int i, int offset) const;

        void draw_y_ticks(QPainter& painter, int i, int offset) const;

        void draw_chart(QPainter& painter) const;

        [[nodiscard]] QColor get_colour(double value) const noexcept;

    protected:
        struct ZRange {
            double min;
            double max;
        };
        ZRange z_range;

        Resolution& resolution;

        int map_resolution;

        Values values;
    };
}
