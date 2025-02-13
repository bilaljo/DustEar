#include "gui/view/charts/heatmap.h"

#include <QColor>
#include <QPainter>


QColor gui::view::charts::HeatMap::get_colour(const double value) const noexcept  {
    // See https://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
    const double colour_value = (value - z_range.min) / (z_range.max - z_range.min);
    const int colour = static_cast<int>(std::round(colour_value * 255));
    return {colour, colour, colour};
}

void gui::view::charts::HeatMap::paintEvent(QPaintEvent *event) {
    Chart::paintEvent(event);
    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    draw_chart(painter);
}

void gui::view::charts::HeatMap::draw_chart(QPainter &painter) const {
    // The algorithm behind is that we draw a grid which we fill with coloured rectangles
    // based on the z-value.
    const int x_start = 1.5 * margin;
    const int x_end = width() - 2 * margin;
    const int y_start = height() - margin;
    const int y_end = margin;
    for (int i = 0; i < resolution.x; i++) {
        for (int j = 0; j < resolution.y; j++) {
            const auto [min_z, max_z] = std::ranges::minmax_element(values.z[i]);
            auto colour = get_colour(values.z[i][j]);
            painter.setBrush(colour);
            double scaled_value = (values.z[i][j] - *min_z) / (*max_z - *min_z);
            const int x_pos = static_cast<int>(x_start + x_end * static_cast<double>(i) / resolution.x);
            const int y_pos = static_cast<int>(std::round(y_start - scaled_value * (y_start - y_end)));
            painter.drawRect(QRect(x_pos, y_pos, map_resolution, map_resolution));
        }
    }
}
