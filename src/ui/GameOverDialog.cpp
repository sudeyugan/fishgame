#include "GameOverDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

GameOverDialog::GameOverDialog(bool win, int score, QWidget *parent) : QDialog(parent) {
    // 1. 窗口属性：无边框、背景透明、模态
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(400, 350); 

    // 2. 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(50, 40, 50, 40);

    // 3. 标题 (VICTORY / GAME OVER)
    QLabel *titleLabel = new QLabel(win ? "VICTORY!" : "GAME OVER", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // 根据胜负设置不同颜色 (金色 vs 红色)
    QString titleColor = win ? "#FFD700" : "#FF4444"; 
    titleLabel->setStyleSheet(QString(
        "QLabel { color: %1; font-family: 'Arial Black'; font-size: 38px; font-weight: bold; }"
    ).arg(titleColor));

    // 给标题添加文字阴影，增加立体感
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 200));
    shadow->setOffset(3, 3);
    titleLabel->setGraphicsEffect(shadow);

    // 4. 分数显示
    QLabel *scoreLabel = new QLabel(QString("Final Score: %1").arg(score), this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("QLabel { color: white; font-size: 22px; font-weight: bold; font-family: 'Verdana'; margin-bottom: 10px; }");

    // 5. 按钮通用样式 (渐变色按钮)
    QString btnStyle = R"(
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4facfe, stop:1 #00f2fe);
            color: white;
            border: 2px solid #ffffff;
            border-radius: 15px;
            font-size: 18px;
            font-weight: bold;
            padding: 8px;
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #66b3ff, stop:1 #33ccff);
            border-color: #e0e0e0;
        }
        QPushButton:pressed {
            background-color: #0078d7;
        }
    )";

    // 重新开始按钮
    QPushButton *btnRestart = new QPushButton("再玩一次", this);
    btnRestart->setCursor(Qt::PointingHandCursor);
    btnRestart->setStyleSheet(btnStyle);
    connect(btnRestart, &QPushButton::clicked, [this](){
        emit restartGame();
        accept();
    });

    // 返回标题按钮 (覆盖样式为橙色系，区分操作)
    QPushButton *btnQuit = new QPushButton("返回标题", this);
    btnQuit->setCursor(Qt::PointingHandCursor);
    btnQuit->setStyleSheet(btnStyle); 
    // 如果想让退出按钮颜色不同，可以在这里覆盖 setStyleSheet，例如使用红色系渐变

    connect(btnQuit, &QPushButton::clicked, [this](){
        emit quitToTitle();
        accept();
    });

    // 添加到布局
    layout->addWidget(titleLabel);
    layout->addWidget(scoreLabel);
    layout->addStretch(); 
    layout->addWidget(btnRestart);
    layout->addWidget(btnQuit);
}

void GameOverDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景：深海蓝渐变 + 半透明
    QRectF rect = this->rect().adjusted(5, 5, -5, -5); // 留出边距画边框
    
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(0, 35, 70, 240));  // 上部较亮
    gradient.setColorAt(1.0, QColor(0, 10, 20, 250));  // 下部接近黑色

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(80, 200, 255, 180), 3)); // 亮蓝色边框
    painter.drawRoundedRect(rect, 25, 25);
}