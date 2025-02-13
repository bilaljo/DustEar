#include "gui/view/charts/line_chart.h"

#include <iostream>
#include <QApplication>
#include <QPainter>

#include "gui/view/charts/matplotlib_colours.h"


gui::view::charts::LineChart::LineChart(QString x_label, QString y_label, double x_min, double x_max):
Chart(std::move(x_label), std::move(y_label)) {

}

void gui::view::charts::LineChart::paintEvent(QPaintEvent *event) {
    Chart::paintEvent(event);
    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    draw_chart(painter);
}

void gui::view::charts::LineChart::draw(const std::pair<std::vector<double>, std::vector<double>>& axis) noexcept {
    this->axis = axis;
    update();
}

void gui::view::charts::LineChart::draw_chart(QPainter &painter) {
    auto [x, y] = axis;
    if (x.empty()) {
        return;
    }
    const auto [min_y, max_y] = std::ranges::minmax_element(y);
    auto [x_min, x_max] = std::ranges::minmax_element(x);
    this->y_min = *min_y;
    this->y_max = *max_y;
    this->x_min = *x_min;
    this->x_max = *x_max;
    const int x_start = 1.5 * margin;
    const int x_end = width() - 2 * margin;
    const int y_start = height() - margin;
    const int y_end = margin;

    set_theme(painter);
    auto range = 0;
    if (this->x_max < x.size()) {
        range = static_cast<int>(this->x_max);
    } else {
        range = x.size();
    }
    const int step_size = (range - 1) / 5.0;
    double y_range = *max_y - *min_y;
    for (int i = 0; i < range; i++) {
        const int offset = static_cast<int>(std::round(x_start + x_end * i / x.size()));
        double scaled_value = (y[i] - *min_y) / y_range;
        const int y_pos = static_cast<int>(std::round(y_start - scaled_value * (y_start - y_end)));
        if (i % step_size == 0) {
            drawXTicks(painter, i, offset);
            double y_offset_double = y_start - ((y_start - y_end) * i) / (x.size() - 1);
            const int y_offset = static_cast<int>(y_offset_double + 0.5);
            draw_y_ticks(painter, i, y_offset);
        }
        painter.setBrush(Qt::blue);
        painter.drawPoint(offset, y_pos);
    }
}

void gui::view::charts::LineChart::drawXTicks(QPainter& painter, int i, int offset) const {
    painter.drawLine(offset, height() - margin, offset, height() - 0.9 * margin);
    const auto axis = std::format("{:1.2}", static_cast<double>(i) / this->axis.first.size() * x_max + x_min);
    painter.drawText(offset - 0.08 * margin, height() - 0.6 * margin, axis.c_str());
}

void gui::view::charts::LineChart::draw_y_ticks(QPainter& painter, int i, int offset) const {
    painter.drawLine(1.3 * margin, offset, 1.5 * margin, offset);
    double range = y_max - y_min;
    const auto axis = std::format("{:.1f}", y_min + static_cast<double>(i) / this->axis.first.size() * range);
    painter.drawText(0.8 * margin, offset + 0.1 * margin, axis.c_str());
}
