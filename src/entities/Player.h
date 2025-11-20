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

    // 【新增】重写这两个函数非常重要
    // 1. 确保没有图片时，碰撞盒依然存在
    QRectF boundingRect() const override;
    // 2. 确保没有图片时，能画出一条"鱼"
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // PLAYER_H