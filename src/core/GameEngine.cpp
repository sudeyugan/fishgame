#include "GameEngine.h"

GameEngine& GameEngine::instance() {
    static GameEngine instance;
    return instance;
}

void GameEngine::startGame() {
    m_score = 0;
    m_currentLevel = 1;
    m_isPaused = false;
    emit scoreChanged(m_score);
    emit levelChanged(m_currentLevel);
}

void GameEngine::pauseGame() {
    m_isPaused = !m_isPaused;
    emit gamePaused(m_isPaused);
}

void GameEngine::addScore(int value) {
    m_score += value;
    emit scoreChanged(m_score);
}

void GameEngine::nextLevel() {
    m_currentLevel++;
    emit levelChanged(m_currentLevel);
}