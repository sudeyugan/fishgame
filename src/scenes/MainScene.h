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
    ~MainScene();

    void startGame(); // 开始/重启游戏
    void pauseGame(); // 暂停

protected:
    // 键盘事件
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame(); // 游戏主循环 (60FPS)
    void spawnEnemy(); // 生成敌人

private:
    Player* m_player;
    QTimer* m_gameTimer;
    QTimer* m_spawnTimer;
    
    // 记录当前按下的键，实现多键无冲突移动
    QSet<int> m_keysPressed; 
    bool m_isPaused;

    void initLevel();
    void handleInput();     // 处理玩家移动
    void checkCollisions(); // 碰撞检测
    void updateCamera();    // 镜头跟随
    void gameOver();
};

#endif // MAINSCENE_H