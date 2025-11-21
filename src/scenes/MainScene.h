#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene> // 必须包含这个，不能只用前向声明
#include <QTimer>
#include <QSet>
#include <QKeyEvent>

class Player;

class MainScene : public QGraphicsScene { // 【关键】确保这里有 public
    Q_OBJECT
public:
    explicit MainScene(QObject* parent = nullptr);
    
    void startGame();

protected:
    // 键盘事件
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // 绘制背景 (这是 protected 函数，必须 public 继承才能重写)
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
    void updateCamera();
    void gameOver();
    void pauseGame();
};

#endif // MAINSCENE_H