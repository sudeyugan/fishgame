#include "StartScreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    // 1. 使用布局管理器，并将边距设为0，确保按钮能铺满整个窗口
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 2. 创建一个透明的大按钮
    QPushButton *fullScreenBtn = new QPushButton(this);
    // 设置样式：背景透明，无边框
    fullScreenBtn->setStyleSheet("background-color: transparent; border: none;");
    // 鼠标移上去变成手型，提示可点击
    fullScreenBtn->setCursor(Qt::PointingHandCursor);
    
    // 3. 连接点击信号
    // 当点击这个透明按钮时，直接调用原本的 startGameClicked 函数
    connect(fullScreenBtn, &QPushButton::clicked, this, &StartScreen::startGameClicked);
    
    layout->addWidget(fullScreenBtn);
}

// 重写 paintEvent 使用图片背景
void StartScreen::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bg(":/assets/images/start.jpg");

    if (!bg.isNull()) {
        // 背景图拉伸铺满
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        painter.fillRect(rect(), QColor(0, 20, 60));
    } 
}