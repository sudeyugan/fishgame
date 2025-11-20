#include "PauseDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

PauseDialog::PauseDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("暂停");
    setModal(true); // 模态对话框，阻止背景操作
    setFixedSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QLabel *label = new QLabel("游戏暂停", this);
    label->setAlignment(Qt::AlignCenter);
    
    QPushButton *btnResume = new QPushButton("继续游戏", this);
    connect(btnResume, &QPushButton::clicked, [this](){
        emit resumeGame();
        accept(); // 关闭弹窗
    });

    QPushButton *btnQuit = new QPushButton("返回标题", this);
    connect(btnQuit, &QPushButton::clicked, [this](){
        emit quitToTitle();
        accept();
    });

    layout->addWidget(label);
    layout->addWidget(btnResume);
    layout->addWidget(btnQuit);
}