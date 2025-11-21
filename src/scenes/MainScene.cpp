#include "MainScene.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../core/GameEngine.h"
#include <QGraphicsView>
#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>

// 注意：这里没有 class MainScene 定义，只有函数实现

MainScene::MainScene(QObject* parent)
    : QGraphicsScene(parent), m_player(nullptr), m_isPaused(false) {
    
    // 1. 设置超大地图 (3000x3000)
    setSceneRect(0, 0, 3000, 3000);
    
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &MainScene::updateGame);

    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &MainScene::spawnEnemy);
}

// 【美化】绘制背景 - 海底网格效果
void MainScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // 1. 填充深海渐变
    QLinearGradient gradient(sceneRect().topLeft(), sceneRect().bottomLeft());
    gradient.setColorAt(0, QColor(0, 105, 148));   // 上层浅蓝
    gradient.setColorAt(1, QColor(0, 20, 40));     // 深海黑蓝
    painter->fillRect(rect, gradient);

    // 2. 绘制装饰性网格
    painter->setPen(QColor(255, 255, 255, 20)); // 极淡的白色
    int gridSize = 200;
    
    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    for (qreal x = left; x < rect.right(); x += gridSize)
        painter->drawLine(x, rect.top(), x, rect.bottom());
    
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        painter->drawLine(rect.left(), y, rect.right(), y);
}

void MainScene::startGame() {
    clear(); 
    m_keysPressed.clear();
    
    initLevel();
    
    m_isPaused = false;
    m_gameTimer->start(16); 
    m_spawnTimer->start(1500); 
}

void MainScene::initLevel() {
    m_player = new Player();
    m_player->setPos(sceneRect().width()/2, sceneRect().height()/2);
    addItem(m_player);
    setFocusItem(m_player);
}

void MainScene::updateGame() {
    if (m_isPaused || !m_player) return;

    handleInput();
    advance();       // 所有物体移动
    checkCollisions();
    
    // 【核心功能】镜头跟随
    if (!views().isEmpty()) {
        views().first()->centerOn(m_player);
    }
}

void MainScene::spawnEnemy() {
    if (!m_player) return;

    // 限制全图最大敌人数
    if (items().size() > 50) return;

    Enemy* enemy = new Enemy(m_player);
    
    int w = static_cast<int>(sceneRect().width());
    int h = static_cast<int>(sceneRect().height());
    
    int x = QRandomGenerator::global()->bounded(0, w);
    int y = QRandomGenerator::global()->bounded(0, h);
    
    enemy->setPos(x, y);
    addItem(enemy);
}

void MainScene::handleInput() {
    if (!m_player) return;
    qreal vx = 0, vy = 0;
    qreal speed = m_player->getSpeed();

    if (m_keysPressed.contains(Qt::Key_W)) vy = -speed;
    if (m_keysPressed.contains(Qt::Key_S)) vy = speed;
    if (m_keysPressed.contains(Qt::Key_A)) vx = -speed;
    if (m_keysPressed.contains(Qt::Key_D)) vx = speed;

    m_player->updateMoveDirection(vx, vy);
}

void MainScene::checkCollisions() {
    if (!m_player) return;
    
    auto items = m_player->collidingItems();
    for (auto item : items) {
        Entity* entity = dynamic_cast<Entity*>(item);
        if (entity && entity->getEntityType() == Entity::TYPE_ENEMY) {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            if (m_player->getSizeScale() > enemy->getSizeScale()) {
                m_player->grow(0.05);
                GameEngine::instance().addScore(10);
                removeItem(enemy);
                delete enemy;
            } else {
                gameOver();
                return;
            }
        }
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
    emit GameEngine::instance().gameOver(false);
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
    // 可以添加暂停信号发射
}