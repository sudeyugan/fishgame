#include "StartScreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 1. 游戏标题
    QLabel *title = new QLabel("Deep Sea Evolution");
    title->setStyleSheet(
        "QLabel { "
        "   font-family: 'Arial Black'; "
        "   font-size: 64px; "
        "   color: #E0FFFF; "
        "   background: transparent; "
        "   padding: 20px; "
        "}"
    );
    layout->addWidget(title);

    // 2. 通用按钮样式
    QString btnStyle = R"(
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4facfe, stop:1 #00f2fe);
            border: 2px solid #ffffff;
            border-radius: 25px;
            color: white;
            font-size: 24px;
            padding: 15px 50px;
            min-width: 200px;
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #66a6ff, stop:1 #89f7fe);
            margin-top: 2px; 
        }
        QPushButton:pressed {
            background-color: #00c6fb;
            border-color: #dddddd;
        }
    )";

    // 3. 开始按钮
    m_startBtn = new QPushButton("Start Game");
    m_startBtn->setStyleSheet(btnStyle);
    m_startBtn->setCursor(Qt::PointingHandCursor);
    connect(m_startBtn, &QPushButton::clicked, this, &StartScreen::startGameClicked);
    layout->addWidget(m_startBtn);

    // 4. 退出按钮
    m_quitBtn = new QPushButton("Exit");
    m_quitBtn->setStyleSheet(btnStyle);
    m_quitBtn->setCursor(Qt::PointingHandCursor);
    connect(m_quitBtn, &QPushButton::clicked, this, &StartScreen::quitGameClicked); // 需在头文件加信号或直接关窗
    layout->addWidget(m_quitBtn);
}

// 重写 paintEvent 使用图片背景
void StartScreen::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bg(":/assets/images/start.jpg");

    if (!bg.isNull()) {
        // 【关键修复】使用 scaled() 将图片缩放到当前窗口大小
        // Qt::KeepAspectRatioByExpanding 保证填满窗口且不变形（可能会裁剪）
        // Qt::IgnoreAspectRatio 如果你想强制拉伸填满（可能会变形）
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        painter.fillRect(rect(), QColor(0, 20, 60));
    }
}