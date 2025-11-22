#include "StartScreen.h"
#include <QPainter>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    
    // 2. 设置鼠标手型，告诉用户这里可以点击
    setCursor(Qt::PointingHandCursor);

}

//鼠标点击任意位置都会触发
void StartScreen::mousePressEvent(QMouseEvent *event) {
    // 判断是否是左键点击
    if (event->button() == Qt::LeftButton) {
        emit startGameClicked();
    }
    
    QWidget::mousePressEvent(event);
}

void StartScreen::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bg(":/assets/images/start.jpg");

    if (!bg.isNull()) {
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        painter.fillRect(rect(), QColor(0, 20, 60));
    }

}