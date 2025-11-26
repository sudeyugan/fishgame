#include "LevelInfoDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

LevelInfoDialog::LevelInfoDialog(int level, const QString& description, QWidget *parent) 
    : QDialog(parent) 
{
    // 1. 窗口基础设置：无边框、透明背景、模态
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(450, 320); // 稍微加宽一点

    // 2. 主布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 40, 30, 30);
    layout->setSpacing(15);

    // 3. 标题：LEVEL X
    QLabel *titleLabel = new QLabel(QString("LEVEL %1").arg(level), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #00ffff;"          // 亮青色
        "font-family: 'Arial Black';"
        "font-size: 32px;"
        "font-weight: bold;"
        "letter-spacing: 2px;"     // 字间距
    );
    // 给标题加一点发光特效
    QGraphicsDropShadowEffect *glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(15);
    glow->setColor(QColor(0, 255, 255, 150));
    glow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(glow);

    // 4. 分割线 (用一个高度为1的 QWidget 模拟)
    QWidget *line = new QWidget(this);
    line->setFixedHeight(2);
    line->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,0,0,0), stop:0.5 #00aaaa, stop:1 rgba(0,0,0,0));");

    // 5. 说明文字内容
    // 我们可以对 description 稍微处理一下，或者直接显示
    QLabel *descLabel = new QLabel(description, this);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(
        "color: #ffffff;"
        "font-size: 16px;"
        "line-height: 150%;"       // 行高
        "font-weight: 500;"
        "margin: 10px;"
    );

    // 6. 确认按钮
    QPushButton *btnOk = new QPushButton("开始挑战", this);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setFixedSize(160, 45);
    
    // 按钮样式表：深蓝底、青色边框、悬停变亮
    btnOk->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(0, 85, 128, 200);"
        "   color: white;"
        "   border: 2px solid #00aaaa;"
        "   border-radius: 22px;" // 圆角胶囊形状
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 120, 180, 220);"
        "   border-color: #00ffff;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 60, 90, 255);"
        "   border-color: #008888;"
        "}"
    );
    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);

    // 添加到布局
    layout->addWidget(titleLabel);
    layout->addWidget(line);
    layout->addWidget(descLabel);
    layout->addStretch(); // 弹簧，把按钮顶到底部
    layout->addWidget(btnOk, 0, Qt::AlignCenter);
}

// 绘制背景
void LevelInfoDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 绘制半透明深色背景
    // 使用渐变色让背景更有质感
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0.0, QColor(0, 20, 40, 240));   // 顶部深蓝
    bgGradient.setColorAt(1.0, QColor(0, 10, 20, 250));   // 底部近黑

    painter.setBrush(bgGradient);
    
    // 2. 绘制边框 (亮青色)
    QPen borderPen(QColor(0, 170, 170));
    borderPen.setWidth(2);
    painter.setPen(borderPen);

    // 绘制圆角矩形
    QRect drawRect = rect().adjusted(2, 2, -2, -2); // 稍微内缩一点防止切边
    painter.drawRoundedRect(drawRect, 15, 15);

    // 3. 可选：画一些装饰线条增加科技感
    painter.setPen(QPen(QColor(0, 255, 255, 100), 1));
    // 左上角装饰
    painter.drawLine(20, 20, 50, 20);
    painter.drawLine(20, 20, 20, 50);
    // 右下角装饰
    painter.drawLine(width()-20, height()-20, width()-50, height()-20);
    painter.drawLine(width()-20, height()-20, width()-20, height()-50);
}