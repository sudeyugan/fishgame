#include "MainScene.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../core/GameEngine.h"
#include <QGraphicsView>
#include <QDebug>
#include <QRandomGenerator>

MainScene::MainScene(QObject* parent)
    : QGraphicsScene(parent), m_player(nullptr), m_isPaused(false) {
    
    // 1. 设置一个很大的场景 (实现地图扩展/滑动窗口的基础)
    setSceneRect(0, 0, 3000, 2000); 
    
    // 2. 初始化定时器
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &MainScene::updateGame);

    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &MainScene::spawnEnemy);
}

MainScene::~MainScene() {
    // QObject 树会自动清理 children，但为了保险可以手动清理
}

void MainScene::startGame() {
    clear(); // 清空场景中所有物体
    m_keysPressed.clear();
    
    initLevel();
    
    m_isPaused = false;
    m_gameTimer->start(16); // ~60 FPS
    m_spawnTimer->start(1500); // 每1.5秒生成一个敌人
}

void MainScene::pauseGame() {
    m_isPaused = !m_isPaused;
    if (m_isPaused) {
        m_gameTimer->stop();
        m_spawnTimer->stop();
    } else {
        m_gameTimer->start();
        m_spawnTimer->start();
    }
    emit GameEngine::instance().gamePaused(m_isPaused);
}

void MainScene::initLevel() {
    // 创建玩家并居中
    m_player = new Player();
    m_player->setPos(sceneRect().width()/2, sceneRect().height()/2);
    addItem(m_player);
    
    // 必须设置焦点，否则收不到键盘事件
    setFocusItem(m_player); 
}

void MainScene::updateGame() {
    if (m_isPaused || !m_player) return;

    // 1. 处理输入
    handleInput();

    // 2. 物理移动 (调用所有 Entity 的 advance)
    advance(); 

    // 3. 碰撞检测
    checkCollisions();

    // 4. 镜头跟随 (高级功能)
    updateCamera();
}

void MainScene::handleInput() {
    if (!m_player) return;

    qreal vx = 0;
    qreal vy = 0;
    qreal speed = m_player->getSpeed();

    if (m_keysPressed.contains(Qt::Key_W) || m_keysPressed.contains(Qt::Key_Up)) {
        vy = -speed;
    }
    if (m_keysPressed.contains(Qt::Key_S) || m_keysPressed.contains(Qt::Key_Down)) {
        vy = speed;
    }
    if (m_keysPressed.contains(Qt::Key_A) || m_keysPressed.contains(Qt::Key_Left)) {
        vx = -speed;
    }
    if (m_keysPressed.contains(Qt::Key_D) || m_keysPressed.contains(Qt::Key_Right)) {
        vx = speed;
    }

    // 更新玩家向量
    m_player->updateMoveDirection(vx, vy);
}

void MainScene::checkCollisions() {
    if (!m_player) return;

    // 获取玩家碰到的所有东西
    QList<QGraphicsItem*> collisions = m_player->collidingItems();
    
    for (QGraphicsItem* item : collisions) {
        Entity* entity = dynamic_cast<Entity*>(item);
        if (!entity) continue;

        // 处理与敌人的碰撞
        if (entity->getEntityType() == Entity::TYPE_ENEMY) {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            // 比较大小
            if (m_player->getSizeScale() > enemy->getSizeScale()) {
                // 吃掉敌人：加分、变大、移除敌人
                GameEngine::instance().addScore(10);
                m_player->grow(0.05); 
                
                // 移除并删除对象
                removeItem(enemy);
                delete enemy;
                
                // 可以在这里加播放音效的代码
            } else {
                // 被吃掉：游戏结束
                gameOver();
                return; // 结束循环
            }
        }
    }
}

void MainScene::spawnEnemy() {
    if (!m_player) return;

    // 在玩家周围一定范围外生成，避免直接生成在脸上
    Enemy* enemy = new Enemy(m_player);
    
    int x, y;
    // 简单逻辑：随机全图
    x = QRandomGenerator::global()->bounded(0, (int)width());
    y = QRandomGenerator::global()->bounded(0, (int)height());
    
    enemy->setPos(x, y);
    addItem(enemy);
}

void MainScene::updateCamera() {
    // 获取视图列表
    QList<QGraphicsView*> viewsList = views();
    if (!viewsList.isEmpty()) {
        // 让第一个视图中心始终对准玩家
        viewsList.first()->centerOn(m_player);
    }
}

void MainScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        pauseGame();
    } else {
        m_keysPressed.insert(event->key());
    }
}

void MainScene::keyReleaseEvent(QKeyEvent *event) {
    m_keysPressed.remove(event->key());
}

void MainScene::gameOver() {
    m_gameTimer->stop();
    m_spawnTimer->stop();
    emit GameEngine::instance().gameOver(false); // 发送失败信号
}