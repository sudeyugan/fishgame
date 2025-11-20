#include "Enemy.h"
#include "Constants.h"

Enemy::Enemy(qreal startX, qreal startY, qreal scale, qreal speed) {
    // 敌人显示为红色
    QPixmap pix(50, 50);
    pix.fill(Qt::red);
    setPixmap(pix);

    setPos(startX, startY);
    setSizeScale(scale);
    setSpeed(speed);
}

void Enemy::advance(int phase) {
    if (!phase) return;
    
    // 简单的AI：一直向左游
    moveBy(-m_speed, 0);

    // 游出屏幕后删除自己（内存管理）
    if (x() < -100) {
        this->scene()->removeItem(this);
        delete this;
    }
}