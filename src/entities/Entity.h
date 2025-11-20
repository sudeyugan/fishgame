#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Entity : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    // 定义实体类型常量，用于安全的类型转换
    enum { Type = UserType + 1 };
    enum EntityType { TYPE_PLAYER, TYPE_ENEMY, TYPE_ITEM };

    explicit Entity(QObject* parent = nullptr);
    
    // 必须重写 type() 以便 qgraphicsitem_cast 工作
    int type() const override { return Type; }
    
    // 纯虚函数，子类必须告知自己是什么类型
    virtual EntityType getEntityType() const = 0;

    // --- 属性设置 ---
    void setSpeed(qreal speed);
    qreal getSpeed() const;

    void setSizeScale(qreal scale);
    qreal getSizeScale() const;

    // --- 移动逻辑 ---
    // 设置移动向量 (dx, dy)
    void setVelocity(qreal dx, qreal dy);
    
    // QGraphicsItem 的核心更新函数
    virtual void advance(int phase) override;

protected:
    qreal m_speed;
    qreal m_scale;
    qreal m_dx; // X轴速度分量
    qreal m_dy; // Y轴速度分量
};

#endif // ENTITY_H