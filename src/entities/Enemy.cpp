#include "Enemy.h"
#include "Player.h"
#include <QtMath>
#include <QRandomGenerator>
#include <QVector2D>

Enemy::Enemy(Player* target, QObject* parent)
    : Entity(parent), m_target(target), m_directionTimer(0) {
    
    // 设置默认图片或红色方块
    QPixmap p(":/assets/enemy.png");
    if (p.isNull()) {
        p = QPixmap(40, 40);
        p.fill(Qt::red);
    }
    setPixmap(p);
    setTransformOriginPoint(boundingRect().center());

    // 随机大小 (0.5倍 到 2.0倍)
    qreal minScale = 0.5;
    qreal maxScale = 2.0;
    qreal randScale = minScale + QRandomGenerator::global()->generateDouble() * (maxScale - minScale);
    setSizeScale(randScale);
    // 随机速度
    qreal minSpeed = 1.5;
    qreal maxSpeed = 3.5;
    m_speed = minSpeed + QRandomGenerator::global()->generateDouble() * (maxSpeed - minSpeed);
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