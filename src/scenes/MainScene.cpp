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

void MainScene::drawBackground(QPainter *painter, const QRectF &rect) {
    static QPixmap bg(":/assets/images/background.jpg");

    if (bg.isNull()) {
        painter->fillRect(rect, Qt::blue); // 图片加载失败时的兜底
        return;
    }
    // 优化：只绘制当前视野区域的背景，提高性能（可选）
    painter->drawPixmap(sceneRect(), bg, bg.rect());
}

void MainScene::startGame() {
    clear(); 
    // 清除 GameEngine 中的旧按键状态
    GameEngine::instance().keys().clear();
    
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

    // 注意：这里不需要调用 handleInput() 了，
    // 因为 Player::advance() 内部会直接读取 GameEngine::keys()
    
    advance(); // 触发所有物体的移动逻辑 (包括 Player 和 Enemy)
    checkCollisions();
    
    // 镜头跟随
    if (!views().isEmpty()) {
        views().first()->centerOn(m_player);
    }
}

void MainScene::spawnEnemy() {
    if (!m_player) return;
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
    // 此函数已废弃，逻辑移至 Player::advance
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

// 【关键修改】将按键事件同步到 GameEngine
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
    m_isPaused = !m_isPaused;
    if (m_isPaused) {
        m_gameTimer->stop();
        m_spawnTimer->stop();
    } else {
        m_gameTimer->start();
        m_spawnTimer->start();
    }
    emit gamePaused(m_isPaused);
}