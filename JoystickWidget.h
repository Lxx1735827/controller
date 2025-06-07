#ifndef JOYSTICKWIDGET_H
#define JOYSTICKWIDGET_H

#include <QWidget>
#include <QPointF>

class JoystickWidget : public QWidget {
    Q_OBJECT

public:
    explicit JoystickWidget(QWidget *parent = nullptr);

signals:
    void directionChanged(double dx, double dy); // [-1, 1] 范围内的方向

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    QPointF center;      // 摇杆底座中心
    QPointF knobPos;     // 小圆位置（摇杆头）
    bool dragging;       // 是否正在拖拽
    void updateDirection();
};

#endif // JOYSTICKWIDGET_H
