#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QSet>
#include <QKeyEvent>

#include "../scenes/LevelManager.h"

class Player; // 前向声明

class MainScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit MainScene(QObject* parent = nullptr);
    
    void startGame();

    void loadLevel(const LevelData& data);
    
    Player* getPlayer() const { return m_player; }

    void setLevelData(const LevelData& data);

    void setPaused(bool paused);

public slots:
    void pauseGame();

signals:
    void gamePaused(bool isPaused);
    
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private slots:
    void updateGame();
    void spawnEnemy();

private:
    Player* m_player;
    QTimer* m_gameTimer;
    QTimer* m_spawnTimer;

    bool m_isPaused;
    QPixmap m_bgPixmap; 
    int m_currentSpawnRate;

    void initLevel();
    void checkCollisions();
    void updateCamera(); 
    void gameOver();
};

#endif // MAINSCENE_H