#include "gui/view/charts/histogram.h"
#include <QFontMetrics>
#include <QApplication>
#include <QStyleHints>
#include <QTextItem>

#include <utility>
#include <algorithm>

#include "helper/histogram.h"
#include "gui/view/charts/matplotlib_colours.h"

gui::view::charts::Histogram::Histogram(QString x_label):
QWidget(),
x_label(std::move(x_label))
{
}

#include <iostream>

void gui::view::charts::Histogram::paintEvent(QPaintEvent *event) {
    QPainter painter{this};
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    painter.drawRect(margin, margin, width() - 2 * margin, height() - 2 * margin);
    //painter.rotate(90);
    painter.drawText(margin * 0.1, (height() - margin) / 2, "Counts");
    //painter.rotate(-90);
    painter.drawText((width() - margin) / 2, height() - 0.1 * margin, x_label);
    QString legend;
    int maximum = 0;
    double min_value = -std::numeric_limits<double>::infinity();
    double max_value = std::numeric_limits<double>::infinity();
    for (const auto& [label, histogram_map]: histograms) {
        auto [index, histogram] = histogram_map;
        auto counts = histogram.get_counts();
        if (*std::ranges::max_element(counts) > maximum) {
            maximum = *std::ranges::max_element(counts);
        }
        auto values = histogram.get_values();
        auto temp_min = *std::ranges::min_element(values);
        if (min_value == -std::numeric_limits<double>::infinity() || temp_min < min_value) {
            min_value = temp_min;
        }
        auto temp_max = *std::ranges::max_element(values);
        if (max_value == std::numeric_limits<double>::infinity() || temp_max > max_value) {
            max_value = temp_max;
        }
    }
    double bin_width;
    std::size_t bins;
    for (const auto& [label, histogram_map]: histograms) {
        auto [index, histogram] = histogram_map;
        bins = histogram.get_bins();
        double offset = 0;
        for (int i = 0; i < bins; i++) {
            auto bar_height = static_cast<double>(height() - 2.1 * margin) * histogram.get_counts()[i] / static_cast<double>(maximum);
            bin_width = static_cast<double>(width() - 2.1 * margin) / static_cast<double>(bins);
            painter.setBrush(QColor(MatplotlibColourArray[index]));
            painter.setOpacity(0.5);
            painter.setPen(Qt::black);
            painter.drawRect(1.1 * margin + bin_width * i, height() - margin - bar_height, bin_width, bar_height);
            QFontMetrics fm = painter.fontMetrics();
            QRect textRect = fm.boundingRect(label.c_str());
            textRect.moveTo(0.8 * width(), 1.1 * margin + offset);
            if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
                painter.setPen(Qt::white);
            } else {
                painter.setPen(Qt::black);
            }
            painter.setOpacity(1);
            painter.drawRect(textRect);
            //painter.drawText(textRect, label.c_str());
            offset += 0.05;
        }
    }
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }
    for (int i = 1; i < 6; i++) {
        double value = min_value +  i * (max_value - min_value) / 5;
        int offset = margin + (width() - margin) * i / 5;
        painter.drawLine(offset, height() - margin, offset, height() - 0.9 * margin);
        if (std::abs(value) < 1e-2) {
            value = std::round(value * 1e4) / 1e4;
        } else if (std::abs(value) < 1e-1) {
            value = std::round(value * 1e3) / 1e3;
        } else {
            value = std::round(value);
        }
        auto axis = std::format("{:1}", value);
        painter.drawText(offset, height() - 0.7 * margin, axis.c_str());
    }
    for (int i = 0; i < 6; i++) {
        int offset = (height() - margin) * i / 6;
        auto yaxis = std::format("{:1.1e}", 0.0 + (i + 1) * maximum / 6);
        painter.drawLine(margin, height() - margin - offset, margin * 0.9, height() - margin - offset);
        painter.drawText(0, height() - 0.8 * margin - offset, yaxis.c_str());
    }
}

void gui::view::charts::Histogram::add_histogram(const QString &label, helper::Histogram<double> histogram) noexcept {
    histograms[label.toStdString()] = {plots, histogram};
    plots++;
}
