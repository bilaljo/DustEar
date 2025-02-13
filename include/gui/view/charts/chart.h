#pragma once

#include <QOpenGLWidget>

namespace gui::view::charts {
    class Chart: public QOpenGLWidget {
    public:
        Chart(QString x_label, QString y_label);

        ~Chart() override;

    protected:
        void paintEvent(QPaintEvent *event) override;

        static void set_theme(QPainter& painter);

    protected:
        static constexpr int margin = 70;

        static constexpr int DARK = 0x19232D;

        static constexpr int LIGHT = 0xFAFAFA;

    protected:
        QString x_label;

        QString y_label;
    };
}
