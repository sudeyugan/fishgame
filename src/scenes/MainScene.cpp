#include "MainScene.h"
#include "../entities/Item.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../core/GameEngine.h"
#include "../utils/AudioManager.h"
#include "../entities/FloatingText.h"
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
    
    m_itemTimer = new QTimer(this);
    connect(m_itemTimer, &QTimer::timeout, this, &MainScene::spawnItem);
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

    GameEngine::instance().keys().clear();
    
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
    m_itemTimer->start(5000);
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
        // 将碰撞物体转换为 Entity 基类
        Entity* entity = dynamic_cast<Entity*>(item);
        if (!entity) continue;

        // 情况 A: 碰到敌人 (Enemy)
        if (entity->getEntityType() == Entity::TYPE_ENEMY) {
            Enemy* enemy = static_cast<Enemy*>(entity);
            
            // 1. 无敌状态判断
            // 如果处于无敌状态（吃了蓝色气泡），可以直接撞死任何敌人（类似马里奥的星星）
            if (m_player->isInvincible()) {
                AudioManager::instance().playSound("eat");
                
                m_player->grow(0.05);                // 依然可以获得成长
                GameEngine::instance().addScore(10); // 依然加分
                
                removeItem(enemy);
                delete enemy;
                
                return; // 处理完一次碰撞后立即返回，防止逻辑冲突
            }

            // 2. 正常状态：比较大小
            if (m_player->getSizeScale() > enemy->getSizeScale()) {
                // --- 玩家吃掉鱼 ---
                AudioManager::instance().playSound("eat");
                
                // 变大
                m_player->grow(0.05);

                // 移除并删除敌人
                removeItem(enemy);
                delete enemy; 

                // 加分 (可能会触发升级逻辑)
                GameEngine::instance().addScore(10);
                
                return; 

            } else {
                // --- 被大鱼吃掉 (游戏结束) ---
                gameOver();
                return;
            }
        }
        
        // 情况 B: 碰到道具 (Item)
        else if (entity->getEntityType() == Entity::TYPE_ITEM) {
            Item* gameItem = static_cast<Item*>(entity);
            
            // 播放音效
            AudioManager::instance().playSound("eat");

            // 获取道具类型
            Item::ItemType type = gameItem->getItemType();


            QString floatStr;
            QColor floatColor;

            // 根据类型分发逻辑
            if (type == Item::ITEM_GOLD) {
                // 金色气泡：直接加分 (50分)
                GameEngine::instance().addScore(50); 

                // [设置] 金色气泡文字
                floatStr = "+50 Score";
                floatColor = QColor(255, 215, 0);
            } else {
                // 其他气泡：红色(变大)、绿色(加速)、蓝色(无敌)
                // 调用 Player 的 applyEffect 进行处理
                m_player->applyEffect(type);
                switch(type) {
                case Item::ITEM_RED:
                    floatStr = "GROW UP!";
                    floatColor = Qt::red;
                    break;
                case Item::ITEM_GREEN:
                    floatStr = "SPEED UP!";
                    floatColor = Qt::green;
                    break;
                case Item::ITEM_BLUE:
                    floatStr = "INVINCIBLE!";
                    floatColor = QColor(0, 191, 255); // 深天蓝
                    break;
                default:
                    floatStr = "Effect!";
                    floatColor = Qt::white;
                    break;
            }
            }

            FloatingText* textEffect = new FloatingText(floatStr, floatColor, m_player->pos());
            addItem(textEffect);

            // 从场景移除并销毁道具
            removeItem(gameItem);
            delete gameItem;
            
            return; // 吃到道具后也立即返回
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
    m_itemTimer->stop();
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
        m_itemTimer->stop();
    } else {
        m_gameTimer->start(16);
        m_spawnTimer->start(m_currentSpawnRate);
        m_itemTimer->start(5000);
    }
    emit gamePaused(m_isPaused);
}

void MainScene::spawnItem() {
    if (!m_player) return;
    // 限制场上道具数量，防止太多
    int itemCount = 0;
    for(auto item : items()) {
        if(dynamic_cast<Item*>(item)) itemCount++;
    }
    if (itemCount > 5) return;

    Item* item = new Item();
    
    // 随机位置
    int w = static_cast<int>(sceneRect().width());
    int h = static_cast<int>(sceneRect().height());
    int x = QRandomGenerator::global()->bounded(0, w);
    int y = QRandomGenerator::global()->bounded(0, h);
    
    item->setPos(x, y);
    addItem(item);
}