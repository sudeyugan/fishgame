#include "GameHud.h"
#include <QHBoxLayout>
#include "../core/GameEngine.h"

GameHud::GameHud(QWidget *parent) : QWidget(parent) {
    // 放在窗口顶部
    QHBoxLayout *layout = new QHBoxLayout(this);
    scoreLabel = new QLabel("Score: 0");
    levelLabel = new QLabel("Level: 1");
    
    // 设置样式（自评表：美观度）
    QString style = "QLabel { color: white; font-size: 20px; font-weight: bold; }";
    scoreLabel->setStyleSheet(style);
    levelLabel->setStyleSheet(style);

    layout->addWidget(levelLabel);
    layout->addStretch();
    layout->addWidget(scoreLabel);

    // 连接信号
    connect(&GameEngine::instance(), &GameEngine::scoreChanged, this, &GameHud::updateScore);
    connect(&GameEngine::instance(), &GameEngine::levelChanged, this, &GameHud::updateLevel);
}

void GameHud::updateScore(int score) {
    scoreLabel->setText("Score: " + QString::number(score));
}

void GameHud::updateLevel(int level) {
    levelLabel->setText("Level: " + QString::number(level));
}