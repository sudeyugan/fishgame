#include "Entity.h"
#include <QGraphicsScene>

Entity::Entity(QObject* parent) 
    : QObject(parent), m_speed(2.0), m_scale(1.0), m_dx(0), m_dy(0) {
    // 开启平滑缩放，防止图片锯齿
    setTransformationMode(Qt::SmoothTransformation);
}

void Entity::setSpeed(qreal speed) {
    m_speed = speed;
}

qreal Entity::getSpeed() const {
    return m_speed;
}

void Entity::setSizeScale(qreal scale) {
    m_scale = scale;
    setScale(m_scale); // 调用 QGraphicsItem 的 setScale
}

qreal Entity::getSizeScale() const {
    return m_scale;
}

void Entity::setVelocity(qreal dx, qreal dy) {
    m_dx = dx;
    m_dy = dy;
}

void Entity::advance(int phase) {
    // phase 0 是预计算，phase 1 是实际更新
    if (!phase) return;

    // 如果有速度，则更新位置
    if (m_dx != 0 || m_dy != 0) {
        setPos(mapToParent(m_dx, m_dy));
    }

    // 简单的边界限制 (防止无限跑出地图)
    if (scene()) {
        qreal w = scene()->width();
        qreal h = scene()->height();
        if (x() < 0) setPos(0, y());
        else if (x() > w) setPos(w, y());
        
        if (y() < 0) setPos(x(), 0);
        else if (y() > h) setPos(x(), h);
    }
}