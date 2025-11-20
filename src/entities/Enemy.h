#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(qreal startX, qreal startY, qreal scale, qreal speed);
    int type() const override { return TYPE_ENEMY; }

protected:
    void advance(int phase) override; // 自动游动
};

#endif