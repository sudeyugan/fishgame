#include "GameEngine.h"
#include "../scenes/LevelManager.h"

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
    LevelData currentData = LevelManager::getLevelData(m_currentLevel);
    
    // 只有当目标分数不是无限模式(如9999)时才检查
    if (m_score >= currentData.targetScore) {
        nextLevel(); // 触发升级
    }
}

void GameEngine::nextLevel() {
    m_currentLevel++;
    emit levelChanged(m_currentLevel);
}

void GameEngine::resetGame() {
    m_score = 0;
    m_currentLevel = 1;
    m_isPaused = false;

    // Update the UI with the reset values
    emit scoreChanged(m_score);
    emit levelChanged(m_currentLevel);
    // Ensure the paused state is updated if it was paused
    emit gamePaused(m_isPaused); 
}