#include "JoystickWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

JoystickWidget::JoystickWidget(QWidget *parent)
    : QWidget(parent), dragging(false)
{
    setMinimumSize(100, 100);
    // knobPos 的初始值将在 resizeEvent 中设置
}

void JoystickWidget::resizeEvent(QResizeEvent *) {
    center = QPointF(width() / 2, height() / 2);
    knobPos = center;
    update(); // 请求重绘以显示居中圆点
}

void JoystickWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 重新确认 center（防止窗口动态调整）
    center = QPointF(width() / 2, height() / 2);

    // 底部大圆
    painter.setBrush(Qt::lightGray);
    painter.drawEllipse(center, 40, 40);

    // 摇杆小圆
    painter.setBrush(Qt::darkGray);
    painter.drawEllipse(knobPos, 15, 15);
}

void JoystickWidget::mousePressEvent(QMouseEvent *event) {
    if ((event->pos() - knobPos).manhattanLength() < 20)
        dragging = true;
}

void JoystickWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        QPointF delta = event->pos() - center;
        double len = qMin(40.0, qSqrt(delta.x() * delta.x() + delta.y() * delta.y()));
        double angle = qAtan2(delta.y(), delta.x());
        knobPos = center + QPointF(qCos(angle) * len, qSin(angle) * len);
        updateDirection();
        update();
    }
}

void JoystickWidget::mouseReleaseEvent(QMouseEvent *) {
    dragging = false;
    knobPos = center;
    emit directionChanged(0, 0);
    update();
}

void JoystickWidget::updateDirection() {
    double dx = (knobPos.x() - center.x()) / 40.0;
    double dy = (knobPos.y() - center.y()) / 40.0;
    emit directionChanged(dx, dy);
}
