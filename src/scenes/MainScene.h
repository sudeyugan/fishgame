#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QSet>
#include "../entities/Player.h"

class MainScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit MainScene(QObject* parent = nullptr);
    
    void startGame();
    void pauseGame();
    bool isPaused() const { return m_isPaused; }

protected:
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Player *m_player;
    QTimer *m_gameTimer;
    QTimer *m_spawnTimer;
    QSet<int> m_pressedKeys;
    bool m_isPaused;

    void spawnEnemy();
    void checkCollisions();
    void checkLevelClear(); // 检查是否通关
    void gameOver();
    void updateGame();      // 游戏主循环
};

#endif // MAINSCENE_H