#include "gui/view/charts/histogram.h"

#include <QtOpenGL/QtOpenGL>

#include <utility>
#include <algorithm>

#include "helper/histogram.h"
#include "gui/view/charts/matplotlib_colours.h"

gui::view::charts::Histogram::Histogram(QString x_label, QString y_label):
Chart(std::move(x_label), std::move(y_label))
{
}

void gui::view::charts::Histogram::drawXTicks(QPainter& painter, int i, int offset) const {
    painter.drawLine(offset, height() - margin, offset, height() - 0.9 * margin);
    auto axis = std::format("{:1.2}", i * range.bin_width);
    painter.drawText(offset - 0.08 * margin, height() - 0.6 * margin, axis.c_str());
}

void gui::view::charts::Histogram::draw(const std::vector<double>& yaxis, const helper::Histogram<double>::Range& range) noexcept {
    this->yaxis = yaxis;
    this->range = range;
    update();
}


void gui::view::charts::Histogram::draw_histogram(QPainter &painter) {
    if (yaxis.empty()) {
        return;
    }
    const auto [temp_y_min, temp_y_max] = std::minmax_element(yaxis.begin() + range.min, yaxis.begin() + range.min  + range.max);
    this->y_min = *temp_y_min;
    this->y_max = *temp_y_max;
    const int x_start = 1.5 * margin;
    const int x_end = width() - 2 * margin - x_start;
    const int step_size = (range.max - range.min) / 5;
    if (!step_size) {
        // Too small range to plot anything useful.
        return;
    }
    const int y_start = height() - margin +  range.min;
    const int y_end = margin;
    set_theme(painter);
    for (int i = range.min; i < range.max; i++) {
        const int offset = x_start + x_end * i / (range.max - 1) + 0.1 * i / (range.max - 1);
        const auto bar_height = (height() - 2.1 * margin) * yaxis[i] / this->y_max;
        if (i % step_size == 0) {
            double y_offset_double = y_start - ((y_start - y_end) * i) / (range.max - 1);
            const int y_offset = static_cast<int>(y_offset_double + 0.5);
            drawXTicks(painter, i, offset);
            draw_y_ticks(painter, i, y_offset);
        }
        const double bin_width = (width() - 2.1 * margin) / static_cast<double>(range.max - 1);
        painter.setBrush(QColor(static_cast<int>(MatplotlibColour::blue)));
        painter.drawRect(offset, height() - margin - bar_height, bin_width, bar_height);
    }
}

void gui::view::charts::Histogram::paintEvent(QPaintEvent *event) {
    Chart::paintEvent(event);
    QPainter painter{this};
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    draw_histogram(painter);
}

void gui::view::charts::Histogram::draw_y_ticks(QPainter& painter, int i, int offset) const {
    painter.drawLine(1.3 * margin, offset, 1.5 * margin, offset);
    double range = y_max - y_min;
    const auto axis = std::format("{:.1e}", y_min + static_cast<double>(i) / this->yaxis.size() * range);
    painter.drawText(0.5 * margin, offset + 0.1 * margin, axis.c_str());
}
