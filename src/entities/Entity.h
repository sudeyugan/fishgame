#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Entity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    enum EntityType { TYPE_PLAYER, TYPE_ENEMY, TYPE_ITEM };

    explicit Entity(QObject* parent = nullptr);
    
    int type() const override { return Type; }
    virtual EntityType getEntityType() const = 0;

    // 属性控制
    void setSpeed(qreal speed);
    qreal getSpeed() const;

    void setSizeScale(qreal scale);
    qreal getSizeScale() const;

    // 移动控制
    void setVelocity(qreal dx, qreal dy);
    
    // 核心逻辑更新
    virtual void advance(int phase) override;

    // 【新增】重写绘图函数，实现"无图自动手绘"
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    qreal m_speed;
    qreal m_scale;
    qreal m_dx;
    qreal m_dy;
};

#endif // ENTITY_H