#include "StartScreen.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    // 1. 设置鼠标手型，暗示屏幕可点击
    setCursor(Qt::PointingHandCursor);

    // 2. 使用布局管理器将“帮助按钮”放置在右下角
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30); // 设置边距
    mainLayout->addStretch(); // 顶部的弹簧，把东西往下推

    // 创建底部水平布局
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(); // 左侧弹簧，把按钮推到右边

    // --- 帮助按钮 ---
    QPushButton *btnHelp = new QPushButton("游戏指南", this);
    btnHelp->setFixedSize(160, 50);
    btnHelp->setCursor(Qt::PointingHandCursor); // 按钮上也显示手型
    
    // 按钮样式：半透明黑底+青色边框，看着比较高级
    btnHelp->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(0, 0, 0, 180);"
        "    color: #00ffff;"
        "    border: 2px solid #00ffff;"
        "    border-radius: 25px;"
        "    font-family: 'Microsoft YaHei';"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 255, 255, 50);"
        "    color: white;"
        "    border-color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 255, 255, 100);"
        "}"
    );

    // 点击按钮 -> 发送 helpClicked 信号
    connect(btnHelp, &QPushButton::clicked, this, &StartScreen::helpClicked);

    bottomLayout->addWidget(btnHelp);
    mainLayout->addLayout(bottomLayout);
}

// 点击屏幕空白处 -> 开始游戏
void StartScreen::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit startGameClicked();
    }
    // 调用基类处理（虽然这里不需要，但好习惯）
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

    // 可选：在屏幕中间画一行提示文字
    painter.setPen(QColor(255, 255, 255, 150)); // 半透明白色
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    // 让文字在屏幕底部上方一点闪烁或静止
    painter.drawText(rect().adjusted(0, 0, 0, -100), Qt::AlignCenter | Qt::AlignBottom, "- 点击屏幕任意位置开始 -");
}