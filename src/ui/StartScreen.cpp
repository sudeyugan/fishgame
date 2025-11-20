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

// 重写 paintEvent 画一个漂亮的海洋背景
void StartScreen::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 线性渐变：深蓝到浅蓝
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(0, 10, 50));   // 深海
    gradient.setColorAt(1.0, QColor(0, 100, 150)); // 浅海
    painter.fillRect(rect(), gradient);
}