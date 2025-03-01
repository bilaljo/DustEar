#include "gui/view/charts/chart.h"

#include <QApplication>
#include <QPainter>
#include <QStyleHints>
#include <utility>

gui::view::charts::Chart::Chart(QString x_label, QString  y_label):
QOpenGLWidget(),
x_label(std::move(x_label)),
y_label(std::move(y_label))
{
    setAutoFillBackground(true);
}

void gui::view::charts::Chart::paintEvent(QPaintEvent *event) {
    QOpenGLWidget::paintEvent(event);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QPainter painter{this};
    set_theme_reversed(painter);
    set_theme(painter);
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    painter.save();
    painter.translate(margin / 4, height() / 2);
    painter.rotate(-90);
    painter.drawText(0, 0, y_label);
    painter.restore();
    painter.drawText((width() - margin) / 2, height() - 0.1 * margin, x_label);
    painter.drawRect(1.5 * margin, margin , width() - 2 * margin, height() - 2 * margin);
}

gui::view::charts::Chart::~Chart() {
    QOpenGLWidget::~QOpenGLWidget();
}

void gui::view::charts::Chart::set_theme(QPainter &painter) {
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }
}

void gui::view::charts::Chart::set_theme_reversed(QPainter &painter) {
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        painter.setPen(Qt::black);
        painter.fillRect(rect(), Qt::black);
    } else {
        painter.setPen(Qt::white);
        painter.fillRect(rect(), Qt::white);

    }
}
