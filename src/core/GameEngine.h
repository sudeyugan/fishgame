#include "Constants.h"

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QMap>

class GameEngine : public QObject {
    Q_OBJECT
public:
    static GameEngine& instance(); // 单例获取

    void setSelectedCharacter(CharacterType type) { m_selectedCharacter = type; }
    CharacterType getSelectedCharacter() const { return m_selectedCharacter; }

    void startGame();
    void pauseGame();
    void resetGame();

    int getScore() const { return m_score; }
    void addScore(int value);
    
    int getCurrentLevel() const { return m_currentLevel; }
    void nextLevel();

    void setBackgroundIndex(int index) { m_bgIndex = index; }
    int getBackgroundIndex() const { return m_bgIndex; }

    QMap<int, bool>& keys() { return m_keys; }

    void setScore(int score);

    qreal getGlobalSpeedBonus() const { return m_globalSpeedBonus; }
    void addGlobalSpeedBonus(qreal value) { m_globalSpeedBonus += value; }

    qreal getGlobalSizeBonus() const { return m_globalSizeBonus; }
    void addGlobalSizeBonus(qreal value) { m_globalSizeBonus += value; }
    
    qreal getGlobalGrowthRate() const { return m_globalGrowthRate; }
    void addGlobalGrowthRate(qreal value) { m_globalGrowthRate += value; }
signals:
    void scoreChanged(int newScore);
    void levelChanged(int newLevel);
    void gamePaused(bool paused);
    void gameOver(bool win);
    void levelCompleted();

private:
    CharacterType m_selectedCharacter = CharacterType::Normal;
    GameEngine() : m_score(0), m_currentLevel(1), m_isPaused(false) {}
    int m_score;
    int m_currentLevel;
    bool m_isPaused;
    QMap<int, bool> m_keys;
    int m_bgIndex = 1;
    qreal m_globalGrowthRate = 1.0;
    qreal m_globalSpeedBonus = 0.0; // 速度加成 (例如 1.0 代表 +100%)
    qreal m_globalSizeBonus = 0.0;  // 初始体型加成
};

#endif