#include "MainScene.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../core/GameEngine.h"
#include <QGraphicsView>
#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>

MainScene::MainScene(QObject* parent)
    : QGraphicsScene(parent), m_player(nullptr), m_isPaused(false) {

    // 设置超大地图 (2400x1800)
    setSceneRect(0, 0, 2400, 1800);
    
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &MainScene::updateGame);

    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &MainScene::spawnEnemy);
}

// 绘制背景改用成员变量 m_bgPixmap
void MainScene::drawBackground(QPainter *painter, const QRectF &rect) {
    if (m_bgPixmap.isNull()) {
        painter->fillRect(rect, QColor(0, 20, 40)); // 默认深海色
        return;
    }
    // 简单绘制：平铺或者拉伸，这里使用平铺覆盖可视区域
    painter->drawPixmap(sceneRect(), m_bgPixmap, m_bgPixmap.rect());
}

void MainScene::loadLevel(const LevelData& data) {
    // 1. 设置参数
    m_currentSpawnRate = data.enemySpawnRate;
    
    // 2. 加载背景图
    if (!data.bgImage.isEmpty()) {
        m_bgPixmap.load(data.bgImage);
    }
    update(); // 触发重绘背景

    // 3. 清理场景中的所有“敌人” (保留主角)
    QList<QGraphicsItem*> allItems = items();
    for (auto item : allItems) {
        // 假设 Enemy 继承自 Entity，且 type 是 ENEMY
        // 这里做一个简单的 dynamic_cast 检查
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            removeItem(enemy);
            delete enemy;
        }
    }

    // 4. 重置主角位置到中心 (可选)
    if (!m_player) {
        initLevel(); // 如果没有主角则创建
    } else {
        m_player->setPos(sceneRect().width()/2, sceneRect().height()/2);
    }
    
    // 5. 确保定时器更新
    if (m_spawnTimer->isActive()) {
        m_spawnTimer->start(m_currentSpawnRate);
    }
}

void MainScene::startGame() {
    if (!m_player) initLevel();

    // 清除按键状态
    GameEngine::instance().keys().clear();
    
    m_isPaused = false;     
    m_gameTimer->start(16); 
    m_spawnTimer->start(m_currentSpawnRate); // 使用当前关卡的速度
}

void MainScene::initLevel() {
    m_player = new Player();
    m_player->setPos(sceneRect().width()/2, sceneRect().height()/2);
    addItem(m_player);
    setFocusItem(m_player);
}


void MainScene::updateGame() {
    if (m_isPaused || !m_player) return;
    
    advance(); // 触发所有物体的移动逻辑 (包括 Player 和 Enemy)
    checkCollisions();
    
    // 镜头跟随
    if (!views().isEmpty()) {
        views().first()->centerOn(m_player);
    }
}

void MainScene::spawnEnemy() {
    if (!m_player) return;
    if (items().size() > 30) return;

    Enemy* enemy = new Enemy(m_player);
    
    int w = static_cast<int>(sceneRect().width());
    int h = static_cast<int>(sceneRect().height());
    
    int x = QRandomGenerator::global()->bounded(0, w);
    int y = QRandomGenerator::global()->bounded(0, h);
    
    enemy->setPos(x, y);
    addItem(enemy);
}

void MainScene::checkCollisions() {
    if (!m_player) return;
    
    // 获取当前碰撞的所有物体
    auto items = m_player->collidingItems();
    
    for (auto item : items) {
        Entity* entity = dynamic_cast<Entity*>(item);
        if (entity && entity->getEntityType() == Entity::TYPE_ENEMY) {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            if (m_player->getSizeScale() > enemy->getSizeScale()) {
                // --- 玩家吃掉鱼 ---
                
                // 1. 先变大
                m_player->grow(0.05);

                // 2. 从场景移除并删除对象
                // 确保即便后面触发了 loadLevel，这个 enemy 也已经不在场景列表中了，不会被重复删除
                removeItem(enemy);
                delete enemy; 

                // 3. 加分 (可能会触发 nextLevel -> loadLevel)
                // 此时就算 loadLevel 清空场景，也不会影响已经删除的 enemy
                GameEngine::instance().addScore(10);
                
                // 4. 立即返回
                return; 

            } else {
                // 被大鱼吃掉
                gameOver();
                return;
            }
        }
    }
}

// 将按键事件同步到 GameEngine
void MainScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        pauseGame();
    } else {
        // 更新全局按键状态，供 Player 使用
        GameEngine::instance().keys()[event->key()] = true;
    }
}

void MainScene::keyReleaseEvent(QKeyEvent *event) {
    // 按键释放
    GameEngine::instance().keys()[event->key()] = false;
}

void MainScene::gameOver() {
    m_gameTimer->stop();
    m_spawnTimer->stop();
    emit GameEngine::instance().gameOver(false);
}

void MainScene::pauseGame() {
    setPaused(!m_isPaused);
}

void MainScene::setPaused(bool paused) {
    m_isPaused = paused;
    if (m_isPaused) {
        m_gameTimer->stop();
        m_spawnTimer->stop();
    } else {
        m_gameTimer->start(16);
        m_spawnTimer->start(m_currentSpawnRate);
    }
    emit gamePaused(m_isPaused);
}