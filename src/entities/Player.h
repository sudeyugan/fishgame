#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
    Q_OBJECT
public:
    explicit Player(QObject* parent = nullptr);
    
    EntityType getEntityType() const override { return TYPE_PLAYER; }
    
    void grow(qreal amount);
    void updateMoveDirection(qreal dx, qreal dy);

    void applyEffect(int type);

    bool isInvincible() const { return m_invincibleTimer > 0; }

protected:
    void advance(int phase) override; 

private:
    int m_speedBoostTimer = 0; // 加速剩余时间
    int m_invincibleTimer = 0; // 无敌剩余时间
    
    qreal m_normalSpeed = 5.0; // 记录原始速度    
};

#endif // PLAYER_H