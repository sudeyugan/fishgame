#include "StartScreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 1. 游戏标题
    QLabel *titleLabel = new QLabel("深海大作战\nFish Game 2025", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: #2c3e50;");
    
    // 2. 按钮组
    QPushButton *btnStart = new QPushButton("开始游戏", this);
    QPushButton *btnHelp = new QPushButton("游戏帮助", this);
    QPushButton *btnQuit = new QPushButton("退出游戏", this);

    // 设置按钮固定大小
    QSize btnSize(200, 50);
    btnStart->setFixedSize(btnSize);
    btnHelp->setFixedSize(btnSize);
    btnQuit->setFixedSize(btnSize);

    // 3. 添加到布局
    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(btnStart);
    layout->addWidget(btnHelp);
    layout->addWidget(btnQuit);

    // 4. 连接信号槽
    connect(btnStart, &QPushButton::clicked, this, &StartScreen::startClicked);
    connect(btnQuit, &QPushButton::clicked, this, &StartScreen::quitClicked);
    
    // 帮助弹窗直接在这里处理
    connect(btnHelp, &QPushButton::clicked, [this](){
        QMessageBox::information(this, "游戏玩法", 
            "1. 使用 WASD 或 方向键 控制主角移动。\n"
            "2. 吃掉比你小的鱼变大，躲避比你大的鱼。\n"
            "3. 达到目标分数即可通关！\n"
            "4. 按 ESC 键暂停游戏。");
    });
}