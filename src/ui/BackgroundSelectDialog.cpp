#include "BackgroundSelectDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

BackgroundSelectDialog::BackgroundSelectDialog(QWidget *parent) 
    : QDialog(parent), m_selectedIndex(1) 
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // 标题
    QLabel *title = new QLabel(">>> 请选择作战区域 <<<", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #00ffff; font-size: 24px; font-weight: bold; font-family: 'Arial Black';");
    mainLayout->addWidget(title);

    // 图片选择区
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);

    // 循环创建3个选项按钮
    for (int i = 1; i <= 3; ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(160, 200);
        btn->setCursor(Qt::PointingHandCursor);
        
        // 使用样式表显示缩略图和边框
        // 假设图片路径是 :/assets/images/background1.jpg 等
        QString style = QString(
            "QPushButton { "
            "   border: 3px solid #444; "
            "   border-radius: 10px; "
            "   border-image: url(:/assets/images/background%1.jpg); "
            "} "
            "QPushButton:hover { "
            "   border: 3px solid #00ffff; "
            "}"
        ).arg(i);
        btn->setStyleSheet(style);

        connect(btn, &QPushButton::clicked, [=](){
            m_selectedIndex = i;
            accept(); // 点击即选中并关闭
        });

        // 下方加个标签说明
        QVBoxLayout *itemLayout = new QVBoxLayout();
        itemLayout->addWidget(btn);
        
        QLabel *lbl = new QLabel(QString("区域 0%1").arg(i), this);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("color: #aaa; margin-top: 5px; font-weight: bold;");
        itemLayout->addWidget(lbl);

        btnLayout->addLayout(itemLayout);
    }

    mainLayout->addLayout(btnLayout);

    // 底部提示
    QLabel *tip = new QLabel("点击图片直接开始", this);
    tip->setAlignment(Qt::AlignCenter);
    tip->setStyleSheet("color: #666; font-size: 12px;");
    mainLayout->addWidget(tip);
}

void BackgroundSelectDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 深色半透明背景
    painter.setBrush(QColor(0, 15, 30, 240));
    painter.setPen(QPen(QColor(0, 100, 200), 2));
    painter.drawRoundedRect(rect().adjusted(2,2,-2,-2), 15, 15);
}