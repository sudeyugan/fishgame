#include "GameEngine.h"
#include "../scenes/LevelManager.h"
#include "../utils/AudioManager.h"

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
    
    if (m_score >= currentData.targetScore) {
        AudioManager::instance().playSound("win");
        // 达到分数，先不升级，而是发送完成信号
        emit levelCompleted(); 
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

    m_globalSpeedBonus = 0.0;
    m_globalSizeBonus = 0.0;
}

void GameEngine::setScore(int score) {
    m_score = score;
    emit scoreChanged(m_score); // 确保通知 HUD 更新
}

