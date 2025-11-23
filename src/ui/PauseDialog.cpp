#include "PauseDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

PauseDialog::PauseDialog(QWidget *parent) : QDialog(parent) {
    // 1. 窗口属性设置
    // 去掉标题栏，设置背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
    setFixedSize(360, 300); // 稍微调大一点

    // 2. 布局管理
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20); // 组件间距
    layout->setContentsMargins(40, 30, 40, 30); // 整体边距

    // 3. 标题
    QLabel *label = new QLabel("PAUSED", this);
    label->setAlignment(Qt::AlignCenter);
    // 设置标题样式：白色粗体，带一点阴影
    label->setStyleSheet("QLabel { color: white; font-family: 'Arial Black'; font-size: 24px; font-weight: bold; }");
    
    // 4. 通用按钮样式
    QString btnStyle = R"(
        QPushButton {
            background-color: rgba(0, 100, 200, 180); 
            color: white;
            border: 2px solid #4facfe;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
            padding: 8px;
        }
        QPushButton:hover {
            background-color: rgba(0, 150, 255, 220);
            border-color: white;
        }
        QPushButton:pressed {
            background-color: rgba(0, 80, 150, 255);
        }
    )";

    // 5. 继续按钮
    QPushButton *btnResume = new QPushButton("继续游戏", this);
    btnResume->setCursor(Qt::PointingHandCursor);
    btnResume->setStyleSheet(btnStyle);
    connect(btnResume, &QPushButton::clicked, [this](){
        emit resumeGame();
        accept();
    });

    // 保存按钮
    QPushButton *btnSave = new QPushButton("保存进度", this);
    btnSave->setCursor(Qt::PointingHandCursor);
    btnSave->setStyleSheet(btnStyle);
    connect(btnSave, &QPushButton::clicked, [this](){
        emit saveGame(); // 发送信号给 MainScene/MainWindow 处理保存逻辑
        // 可选：弹出简单的提示框，或者直接关闭
        QMessageBox::information(this, "提示", "游戏保存成功！");
    });
    
    // 6. 退出按钮
    QPushButton *btnQuit = new QPushButton("返回标题", this);
    btnQuit->setCursor(Qt::PointingHandCursor);
    btnQuit->setStyleSheet(btnStyle);
    connect(btnQuit, &QPushButton::clicked, [this](){
        emit quitToTitle();
        accept();
    });

    // 添加到布局
    layout->addWidget(label);
    layout->addStretch(); // 弹簧，把标题顶上去，按钮压下来
    layout->addWidget(btnResume);
    layout->addWidget(btnQuit);
    layout->addWidget(btnSave);
}

// 自定义绘制背景
void PauseDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制半透明黑色圆角背景
    painter.setBrush(QColor(0, 20, 40, 230)); // 深海蓝黑，90%不透明度
    painter.setPen(Qt::NoPen);
    
    // 绘制圆角矩形
    painter.drawRoundedRect(rect(), 15, 15);

    // 可选：画一个边框
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(79, 172, 254), 2)); // 亮蓝色边框
    painter.drawRoundedRect(rect().adjusted(1,1,-1,-1), 15, 15);
}