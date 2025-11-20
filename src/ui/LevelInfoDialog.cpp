#include "LevelInfoDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

LevelInfoDialog::LevelInfoDialog(int level, const QString& description, QWidget *parent) 
    : QDialog(parent) 
{
    setWindowTitle("关卡目标");
    setModal(true);
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // 标题
    QLabel *title = new QLabel("第 " + QString::number(level) + " 关", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    // 说明文字
    QLabel *desc = new QLabel(description, this);
    desc->setAlignment(Qt::AlignCenter);
    desc->setWordWrap(true);
    desc->setStyleSheet("font-size: 16px; padding: 20px;");

    // 确认按钮
    QPushButton *btnOk = new QPushButton("开始挑战", this);
    btnOk->setFixedSize(120, 40);
    connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(title);
    layout->addWidget(desc);
    layout->addWidget(btnOk, 0, Qt::AlignCenter);
}