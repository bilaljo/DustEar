#pragma once

// C++ Port of the Python library python-qtwidgets of Martin Fitzpatrick, Eman and Sacha Schutz
// https://github.com/pythonguis/python-qtwidgets/tree/master

#include <QCheckBox>
#include <QPainter>
#include <QColor>

namespace gui::view::widgets {
    class Toggle : public QCheckBox {
        Q_OBJECT

        Q_PROPERTY(qreal handlePosition READ handlePosition WRITE setHandlePosition)

        Q_PROPERTY(qreal pulseRadius READ pulseRadius WRITE setPulseRadius)

    public:
        explicit Toggle(QColor bar_color=Qt::gray, QColor checked_color=0x00B0FF, QColor handle_color=Qt::white):
        bar_brush(new QBrush(bar_color)), bar_checked_brush(new QBrush(QColor(checked_color).lighter())),
        handle_checked_brush(new QBrush(checked_color)), handle_brush(new QBrush(handle_color))
        {
            setContentsMargins(8, 0, 8, 0);
            connect(this, &QCheckBox::stateChanged, [this](int value){return handle_state_change(value);});
        }

        [[nodiscard]] QSize sizeHint() const override {
            return {58, 45};
        }

    protected:
        [[nodiscard]] bool hitButton(const QPoint &pos) const override {
            return contentsRect().contains(pos);
        }

        void paintEvent([[maybe_unused]] QPaintEvent* event) override {
            auto contRect = contentsRect();
            auto handleRadius = round(0.24 * contRect.height());
            auto p = QPainter(this);
            p.setRenderHint(QPainter::Antialiasing);
            p.setPen(Qt::transparent);
            barRect = QRectF(0, 0, contRect.width() - handleRadius, 0.40 * contRect.height());
            barRect.moveCenter(contRect.center());
            auto rounding = barRect.height() / 2;
            auto trailLength = contRect.width() - 2 * handleRadius;
            xPos = contRect.x() + handleRadius + trailLength * handle_position;

            if (isChecked()) {
                p.setBrush(*bar_checked_brush);
                p.drawRoundedRect(barRect, rounding, rounding);
                p.setBrush(*handle_checked_brush);
            } else {
                p.setBrush(*bar_brush);
                p.drawRoundedRect(barRect, rounding, rounding);
                p.setPen(Qt::lightGray);
                p.setBrush(*handle_brush);
            }
            p.drawEllipse(QPointF(xPos, barRect.center().y()), handleRadius, handleRadius);
            p.end();
         }

         [[nodiscard]] qreal handlePosition() const noexcept {
            return handle_position;
        }

        void setHandlePosition(const qreal position) noexcept {
            handle_position = position;
            update();
        }

        [[nodiscard]] qreal pulseRadius() const noexcept {
            return pulse_radius;
        }

        void setPulseRadius(const qreal position) noexcept {
            pulse_radius = position;
            update();
        }

    protected:
        virtual void handle_state_change(const int value) {
            handle_position = value ? 1 : 0;
        }

    protected:
        std::unique_ptr<QBrush> bar_brush;

        std::unique_ptr<QBrush> bar_checked_brush;

        std::unique_ptr<QBrush> handle_checked_brush;

        std::unique_ptr<QBrush> handle_brush;

        qreal handle_position{};

        qreal pulse_radius{};

        QRectF barRect;

        double xPos{};
    };
}
