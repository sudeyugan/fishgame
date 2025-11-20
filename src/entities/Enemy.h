#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Player; // 前向声明

class Enemy : public Entity {
    Q_OBJECT
public:
    enum Behavior { PATROL, CHASE, FLEE };

    // 构造函数需传入玩家指针，以便AI判断距离
    Enemy(Player* target, QObject* parent = nullptr);

    EntityType getEntityType() const override { return TYPE_ENEMY; }

    void advance(int phase) override;

private:
    Player* m_target;
    int m_directionTimer; // 控制巡逻方向改变
    
    void performAI(); // AI 核心逻辑
};

#endif // ENEMY_H