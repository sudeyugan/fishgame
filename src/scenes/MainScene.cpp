#include "MainScene.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../core/GameEngine.h"
#include "../core/Constants.h"
#include "LevelManager.h"
#include "../ui/LevelInfoDialog.h" // 稍后定义

#include <QGraphicsView>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>

MainScene::MainScene(QObject* parent) 
    : QGraphicsScene(parent), m_player(nullptr)
{
    setSceneRect(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    
    // 1. 初始化计时器
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &MainScene::updateGame);

    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &MainScene::spawnEnemy);
}

void MainScene::startGame() {
    clear(); // 清空上一局的残留

    // 1. 创建主角
    m_player = new Player();
    m_player->setPos(Config::WINDOW_WIDTH / 2, Config::WINDOW_HEIGHT / 2);
    addItem(m_player);

    // 2. 获取当前关卡数据
    int levelIndex = GameEngine::instance().getCurrentLevel();
    LevelData data = LevelManager::getLevelData(levelIndex);

    // 3. 显示关卡说明 (自评表必做)
    LevelInfoDialog dlg(levelIndex, data.description);
    dlg.exec(); // 模态显示，玩家点击确定后才继续

    // 4. 启动计时器
    m_gameTimer->start(Config::REFRESH_RATE);
    m_spawnTimer->start(data.enemySpawnRate);
}

void MainScene::pauseGame() {
    // 只是停止/启动计时器，逻辑状态由 GameEngine 管理
    if (m_gameTimer->isActive()) {
        m_gameTimer->stop();
        m_spawnTimer->stop();
    } else {
        m_gameTimer->start();
        m_spawnTimer->start();
    }
}

void MainScene::updateGame() {
    if (!m_player) return;

    // 1. 处理玩家输入
    m_player->handleInput(m_pressedKeys);

    // 2. 所有的 Entity 移动 (调用 advance)
    advance();

    // 3. 碰撞检测
    checkCollisions();
}

void MainScene::spawnEnemy() {
    // 在屏幕右侧外生成
    int startY = QRandomGenerator::global()->bounded(50, Config::WINDOW_HEIGHT - 50);
    qreal scale = QRandomGenerator::global()->bounded(0.5, 2.5); 
    qreal speed = QRandomGenerator::global()->bounded(2.0, 5.0);

    Enemy* enemy = new Enemy(Config::WINDOW_WIDTH + 50, startY, scale, speed);
    addItem(enemy);
}

void MainScene::checkCollisions() {
    if (!m_player) return;

    QList<QGraphicsItem*> items = collidingItems(m_player);
    for (auto item : items) {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            // 判定大小
            if (m_player->scale() > enemy->scale()) {
                // 吃掉敌人
                GameEngine::instance().addScore(10);
                m_player->grow();
                
                removeItem(enemy);
                delete enemy;

                // 检查是否过关
                checkLevelClear();
            } else {
                // 被吃掉 - 游戏结束
                gameOver();
                return; // 玩家已死，停止检测
            }
        }
    }
}

void MainScene::checkLevelClear() {
    int currentScore = GameEngine::instance().getScore();
    int level = GameEngine::instance().getCurrentLevel();
    LevelData data = LevelManager::getLevelData(level);

    if (currentScore >= data.targetScore) {
        m_gameTimer->stop();
        m_spawnTimer->stop();
        
        // 进入下一关
        GameEngine::instance().nextLevel();
        startGame(); // 重新开始（会加载新关卡数据）
    }
}

void MainScene::gameOver() {
    m_gameTimer->stop();
    m_spawnTimer->stop();
    // 发送信号给 GameEngine 或 MainWindow 弹出失败窗口
    emit GameEngine::instance().gameOver(false);
}

void MainScene::keyPressEvent(QKeyEvent *event) {
    m_pressedKeys.insert(event->key());
}

void MainScene::keyReleaseEvent(QKeyEvent *event) {
    m_pressedKeys.remove(event->key());
}