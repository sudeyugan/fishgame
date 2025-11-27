#include "CharacterSelectDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

CharacterSelectDialog::CharacterSelectDialog(QWidget *parent)
    : QDialog(parent), m_selectedType(CharacterType::Normal)
{
    setWindowTitle("选择你的英雄");
    setFixedSize(800, 500); // 调整窗口大小
    setupUi();
    
    // 设置背景样式 (渐变色)
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1e3c72, stop:1 #2a5298);
        }
        QLabel {
            color: white;
            font-family: "Microsoft YaHei";
        }
    )");
}

CharacterType CharacterSelectDialog::getSelectedCharacter() const {
    return m_selectedType;
}

void CharacterSelectDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 标题
    QLabel *titleLabel = new QLabel("请选择出战角色", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; margin-bottom: 20px;");
    mainLayout->addWidget(titleLabel);

    // 角色卡片容器
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(40);
    cardsLayout->setContentsMargins(50, 0, 50, 20);

    // 卡片 1: 普通鱼
    QWidget *card1 = createCharacterCard(
        "小丑鱼 (Classic)", 
        "均衡的属性，适合新手。\n\n• 速度: 中等\n• 特技: 无", 
        ":/assets/images/fish1.png", 
        CharacterType::Normal
    );

    // 卡片 2: 敏捷鱼 (Fish2)
    QWidget *card2 = createCharacterCard(
        "深海刺客 (Agile)", 
        "牺牲体型换取爆发力。\n\n• 速度: 较快\n• 特技: 按空格键缩小并冲刺", 
        ":/assets/images/fish2.png", 
        CharacterType::Agile
    );

    cardsLayout->addWidget(card1);
    cardsLayout->addWidget(card2);
    
    mainLayout->addLayout(cardsLayout);
}

QWidget* CharacterSelectDialog::createCharacterCard(const QString &name, const QString &desc, const QString &imagePath, CharacterType type) {
    QFrame *frame = new QFrame(this);
    frame->setStyleSheet(R"(
        QFrame {
            background-color: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }
        QFrame:hover {
            background-color: rgba(255, 255, 255, 0.2);
            border: 2px solid #00d2ff;
        }
    )");
    
    QVBoxLayout *layout = new QVBoxLayout(frame);
    
    // 图片
    QLabel *imgLabel = new QLabel(frame);
    QPixmap pix(imagePath);
    imgLabel->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setStyleSheet("border: none; background: transparent;");
    
    // 名字
    QLabel *nameLabel = new QLabel(name, frame);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-size: 20px; font-weight: bold; border: none; background: transparent; color: #ffdb4d;");
    
    // 描述
    QLabel *descLabel = new QLabel(desc, frame);
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    descLabel->setStyleSheet("font-size: 14px; border: none; background: transparent; color: #e0e0e0;");
    
    // 选择按钮
    QPushButton *btn = new QPushButton("选择", frame);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: #00d2ff;
            color: black;
            border-radius: 20px;
            padding: 10px 30px;
            font-weight: bold;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #33d9ff;
        }
        QPushButton:pressed {
            background-color: #00a3cc;
        }
    )");

    connect(btn, &QPushButton::clicked, this, [this, type]() {
        m_selectedType = type;
        accept(); // 关闭对话框并返回 QDialog::Accepted
    });

    layout->addStretch();
    layout->addWidget(imgLabel);
    layout->addWidget(nameLabel);
    layout->addSpacing(10);
    layout->addWidget(descLabel);
    layout->addSpacing(20);
    layout->addWidget(btn);
    layout->addStretch();

    return frame;
}