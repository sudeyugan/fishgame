#include "GameHud.h"
#include <QHBoxLayout>
#include <QFont>

GameHud::GameHud(QWidget *parent) : QWidget(parent) {
    // 设置 HUD 覆盖在窗口顶部
    QHBoxLayout *layout = new QHBoxLayout(this);
    
    m_scoreLabel = new QLabel("Score: 0", this);
    m_levelLabel = new QLabel("Level: 1", this);

    // 美化字体
    QFont font("Arial", 16, QFont::Bold);
    m_scoreLabel->setFont(font);
    m_levelLabel->setFont(font);
    m_scoreLabel->setStyleSheet("color: yellow; text-shadow: 1px 1px 2px black;");
    m_levelLabel->setStyleSheet("color: white; text-shadow: 1px 1px 2px black;");

    layout->addWidget(m_levelLabel);
    layout->addStretch(); // 弹簧，把两个标签顶在两边
    layout->addWidget(m_scoreLabel);
    
    setLayout(layout);
}

void GameHud::updateScore(int score) {
    m_scoreLabel->setText(QString("Score: %1").arg(score));
}

void GameHud::updateLevel(int level) {
    m_levelLabel->setText(QString("Level: %1").arg(level));
}