#include "Enemy.h"
#include "Player.h" 
#include "../core/GameEngine.h"
#include <QtMath>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QDebug>

// 构造函数实现：没有 void，参数匹配
Enemy::Enemy(Player* target, QObject* parent)
    : Entity(parent), m_target(target), m_directionTimer(0) {
    
    // 1. 随机大小 (修改为加权随机，大幅增加小鱼概率)
    int chance = QRandomGenerator::global()->bounded(100);
    qreal minScale, maxScale;

    if (chance < 70) {
        // 70% 概率生成小鱼 (0.5 - 0.99)
        minScale = 0.5;
        maxScale = 1.0;
    } else if (chance < 90) {
        // 20% 概率生成中等鱼 (1.0 - 1.79)
        minScale = 1.0;
        maxScale = 1.8;
    } else {
        // 10% 概率生成大鱼 (1.8 - 2.5)
        minScale = 1.8;
        maxScale = 2.5;
    }

    // 使用基类 Entity 的 m_scale 成员
    m_scale = minScale + QRandomGenerator::global()->generateDouble() * (maxScale - minScale);

    // 2. 加载对应图片
    QString imagePath;
    if (m_scale < 1.0)      imagePath = ":/assets/images/enemyfish1.png"; 
    else if (m_scale < 1.8) imagePath = ":/assets/images/enemyfish2.png"; 
    else                    imagePath = ":/assets/images/enemyfish3.png"; 

    QPixmap p(imagePath);
    if (p.isNull()) {
        p = QPixmap(40, 40);
        p.fill(Qt::red); // 兜底红色方块
    } else {
        // 统一基准宽度，方便后续缩放
        p = p.scaledToWidth(80, Qt::SmoothTransformation);
    }

    setPixmap(p);
    
    // 设置变换中心
    setTransformOriginPoint(p.width()/2, p.height()/2);
    setOffset(-p.width()/2, -p.height()/2);

    // 应用初始大小
    setSizeScale(m_scale); 

    // 速度计算
    // 1. 获取当前等级
    int currentLevel = GameEngine::instance().getCurrentLevel();

    // 2. 计算等级加成系数：每级提升 10% (0.1)，最高提升到 3.0倍 (即20级封顶)
    double levelFactor = 1.0 + (currentLevel - 1) * 0.1;
    if (levelFactor > 3.0) levelFactor = 3.0; // 速度上限倍率

    // 3. 基础速度 (体积越大越慢) * 等级系数
    double baseSpeed = 2.0 / m_scale; 
    m_speed = baseSpeed * levelFactor;

    // 4. 再次限制绝对速度范围，防止游戏崩溃或不可玩
    // 比如：最小速度随等级提升一点，最大速度也提升
    double maxLimit = 4.9; // 绝对速度上限
    if (m_speed > maxLimit) m_speed = maxLimit;
    if (m_speed < 1.5) m_speed = 1.5; // 绝对保底速度
}

void Enemy::advance(int phase) {
    if (!phase) return;

    performAI(); // 计算 m_dx, m_dy 和翻转

    // --- 正弦波游动效果 ---
    // 在原有 AI 计算的 m_dy 基础上，叠加一个波动
    // 只有在水平巡逻时才明显波动，追逐时减弱
    qreal wave = std::sin(x() * 0.05) * 2.0; 
    
    qreal newX = x() + m_dx;
    qreal newY = y() + m_dy + wave;

    setPos(newX, newY);
    
    // 销毁逻辑：如果跑出太远（比如左边屏幕外）
    if (newX < -200) {
        if (scene()) scene()->removeItem(this);
        delete this;
    }
}

void Enemy::performAI() {
    if (!m_target) return; 

    QLineF line(pos(), m_target->pos());
    qreal dist = line.length();

    // --- [修改点] 动态计算感知范围 ---
    
    int currentLevel = GameEngine::instance().getCurrentLevel();

    // 基础范围 200，每级增加 20 像素
    qreal detectionRange = 200 + (currentLevel - 1) * 20;

    // 设定感知范围上限（比如 400，大约是屏幕宽度的 1/3 到 1/2）
    if (detectionRange > 400) detectionRange = 400;

    // --------------------------------

    // 使用计算出的 detectionRange 替代原来的 200
    if (dist < detectionRange) { 
        qreal angle = -line.angle(); 
        qreal rad = qDegreesToRadians(angle);

        bool chase = (this->getSizeScale() > m_target->getSizeScale());
        int direction = chase ? 1 : -1;

        m_dx = std::cos(rad) * m_speed * direction;
        m_dy = std::sin(rad) * m_speed * direction;
    } else {
        // 巡逻模式 (保持不变)
        m_directionTimer++;
        if (m_directionTimer > 100) {
            int angle = QRandomGenerator::global()->bounded(0, 360);
            qreal rad = qDegreesToRadians((double)angle);
            m_dx = std::cos(rad) * m_speed * 0.5;
            m_dy = std::sin(rad) * m_speed * 0.5;
            m_directionTimer = 0;
        }
    }

    // (翻转逻辑保持不变)
    QTransform t;
    if (m_dx < 0) t.scale(-m_scale, m_scale); 
    else t.scale(m_scale, m_scale);
    setTransform(t);
}