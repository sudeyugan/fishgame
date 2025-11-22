#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
    Q_OBJECT
public:
    explicit Player(QObject* parent = nullptr);
    
    EntityType getEntityType() const override { return TYPE_PLAYER; }

    void advance(int phase) override;
    
    void grow(qreal amount);
    void updateMoveDirection(qreal dx, qreal dy);

};

#endif // PLAYER_H