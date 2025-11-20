#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Entity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    enum Type { TYPE_PLAYER, TYPE_ENEMY };

    Entity(QObject* parent = nullptr);
    
    // 设置类型和属性
    virtual int type() const override = 0; // 用于碰撞检测区分
    void setSpeed(qreal speed);
    void setSizeScale(qreal scale); // 改变大小

    // 核心移动逻辑，由Scene定时调用
    virtual void advance(int phase) override;

protected:
    qreal m_speed;
    qreal m_scale;
};

#endif // ENTITY_H