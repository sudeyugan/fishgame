#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QSet>
#include <QKeyEvent>

class Player;

class MainScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit MainScene(QObject* parent = nullptr);
    
    void startGame();

protected:
    // 键盘事件
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // 【新增】绘制背景，用于画海底渐变和网格
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private slots:
    void updateGame();
    void spawnEnemy();

private:
    Player* m_player;
    QTimer* m_gameTimer;
    QTimer* m_spawnTimer;
    
    QSet<int> m_keysPressed; 
    bool m_isPaused;

    void initLevel();
    void handleInput();
    void checkCollisions();
    void gameOver();
};

#endif // MAINSCENE_H