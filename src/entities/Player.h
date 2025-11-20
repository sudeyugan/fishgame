#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
    Q_OBJECT
public:
    explicit Player(QObject* parent = nullptr);

    // 实现基类虚函数
    EntityType getEntityType() const override { return TYPE_PLAYER; }

    // 玩家特有功能：吃鱼变大
    void grow(qreal amount);
    
    // 处理输入向量
    void updateMoveDirection(qreal dx, qreal dy);
};

#endif // PLAYER_H