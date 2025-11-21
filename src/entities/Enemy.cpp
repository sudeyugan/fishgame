#include "Enemy.h"
#include "Player.h"
#include <QtMath>
#include <QRandomGenerator>
#include <QVector2D>

Enemy::Enemy(Player* target, QObject* parent)
    : Entity(parent), m_target(target), m_directionTimer(0) {
    
    // 1. 先随机生成大小 (0.5倍 到 2.5倍)
    qreal minScale = 0.5;
    qreal maxScale = 2.5;
    qreal randScale = minScale + QRandomGenerator::global()->generateDouble() * (maxScale - minScale);

    // 2. 根据大小决定图片 (体积越大 -> 图片编号越大)
    QString imagePath;
    if (randScale < 1.0) {
        imagePath = ":/assets/enemyfish1.png"; // 小鱼
    } else if (randScale < 1.8) {
        imagePath = ":/assets/enemyfish2.png"; // 中鱼
    } else {
        imagePath = ":/assets/enemyfish3.png"; // 大鱼
    }

    // 3. 加载图片
    QPixmap p(imagePath);
    if (p.isNull()) {
        // 如果找不到图，用红色方块代替
        p = QPixmap(40, 40);
        p.fill(Qt::red);
    }
    setPixmap(p);
    setTransformOriginPoint(boundingRect().center());

    // 4. 应用大小
    setSizeScale(randScale);

    // 5. 随机速度 (通常大鱼游得慢，小鱼游得快，这里做一个简单的反比设定)
    // 基础速度 2.0，体积越大速度越慢
    m_speed = 4.0 / randScale; 
    // 限制一下最小速度，防止大鱼不动
    if (m_speed < 1.0) m_speed = 1.0;
}

void Enemy::advance(int phase) {
    if (!phase) return;

    performAI(); // 每帧思考
    Entity::advance(phase); // 执行移动
}

void Enemy::performAI() {
    if (!m_target) return;

    // 计算与玩家的向量和距离
    QLineF line(pos(), m_target->pos());
    qreal dist = line.length();

    if (dist < 350) { // 感知范围
        qreal angle = -line.angle(); // QLineF 的角度是 0-360
        qreal rad = qDegreesToRadians(angle);

        if (this->getSizeScale() > m_target->getSizeScale()) {
            // 我比玩家大 -> 追逐 (Chase)
            m_dx = std::cos(rad) * m_speed;
            m_dy = std::sin(rad) * m_speed;
        } else {
            // 我比玩家小 -> 逃跑 (Flee)
            // 反向移动
            m_dx = -std::cos(rad) * m_speed * 1.2; 
            m_dy = -std::sin(rad) * m_speed * 1.2;
        }
    } else {
        // 巡逻模式：每隔一段时间改变随机方向
        m_directionTimer++;
        if (m_directionTimer > 120) { // 约2秒换一次
            int angle = QRandomGenerator::global()->bounded(0, 360);
            qreal rad = qDegreesToRadians((double)angle);
            m_dx = std::cos(rad) * m_speed * 0.5; // 巡逻慢一点
            m_dy = std::sin(rad) * m_speed * 0.5;
            m_directionTimer = 0;
        }
    }

    // 简单的面向调整
    if (m_dx > 0) setTransform(QTransform()); 
    else setTransform(QTransform().scale(-1, 1).translate(-boundingRect().width(), 0));
}