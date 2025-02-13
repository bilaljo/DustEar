#pragma once

// C++ Port of the Python library python-qtwidgets of Martin Fitzpatrick, Eman and Sacha Schutz
// https://github.com/pythonguis/python-qtwidgets/tree/master

#include <memory>

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QSequentialAnimationGroup>

#include "toggle.hpp"

namespace gui::view::widgets {
    class AnimatedToggle: public Toggle {
        Q_OBJECT

    public:
        AnimatedToggle(QColor pulse_unchecked_color=0x44999999, QColor pulse_checked_color=0x4400B0EE):
        animation(new QPropertyAnimation(this, "handlePosition", this)),
        pulse_animation(new QPropertyAnimation(this, "pulseRadius", this)),
        animations_group(new QSequentialAnimationGroup()),
        pulse_unchecked_animation_brush(new QBrush(pulse_unchecked_color)),
        pulse_checked_animation_brush(new QBrush(pulse_checked_color))
        {
            pulse_radius = 0;
            animation->setEasingCurve(QEasingCurve::InOutCubic);
            animation->setDuration(ANIMATION_DURATION_MS);
            pulse_animation->setDuration(PULSE_ANIMATION_DURATION_MS);
            pulse_animation->setStartValue(PULSE_ANIMATION_START_VALUE);
            pulse_animation->setEndValue(PULSE_ANIMATION_END_VALUE);
            animations_group->addAnimation(animation.get());
            animations_group->addAnimation(pulse_animation.get());
        }

        void paintEvent(QPaintEvent* event) override {
            Toggle::paintEvent(event);
            auto painter = QPainter(this);
            if (pulse_animation->state() == QPropertyAnimation::Running) {
                if (isChecked()) {
                    painter.setBrush(*pulse_checked_animation_brush);
                } else {
                    painter.setBrush(*pulse_unchecked_animation_brush);
                }
                painter.drawEllipse(QPointF(xPos, barRect.center().y()), pulse_radius, pulse_radius);
            }
        }

        void handle_state_change(const int value) override {
            animations_group->stop();
            if (value) {
                animation->setEndValue(1);
            } else {
                animation->setEndValue(0);
            }
            animations_group->start();
        }

    private:
        std::unique_ptr<QPropertyAnimation> animation;

        std::unique_ptr<QPropertyAnimation> pulse_animation;

        std::unique_ptr<QSequentialAnimationGroup> animations_group;

        std::unique_ptr<QBrush> pulse_unchecked_animation_brush;

        std::unique_ptr<QBrush> pulse_checked_animation_brush;

    private:
        static constexpr std::size_t ANIMATION_DURATION_MS = 200;

        static constexpr std::size_t PULSE_ANIMATION_DURATION_MS = 350;

        static constexpr std::size_t PULSE_ANIMATION_START_VALUE = 10;

        static constexpr std::size_t PULSE_ANIMATION_END_VALUE = 10;

    };
}
