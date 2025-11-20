#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>

class GameEngine : public QObject {
    Q_OBJECT
public:
    static GameEngine& instance(); // 单例获取

    void startGame();
    void pauseGame();
    void resetGame();

    int getScore() const { return m_score; }
    void addScore(int value);
    
    int getCurrentLevel() const { return m_currentLevel; }
    void nextLevel();

signals:
    void scoreChanged(int newScore);
    void levelChanged(int newLevel);
    void gamePaused(bool paused);
    void gameOver(bool win);

private:
    GameEngine() : m_score(0), m_currentLevel(1), m_isPaused(false) {}
    int m_score;
    int m_currentLevel;
    bool m_isPaused;
};

#endif