#include "Entity.h"
#include <QPainter>

Entity::Entity(QObject* parent) 
    : QObject(parent), m_speed(0), m_scale(1.0), m_dx(0), m_dy(0) {
}

void Entity::setSpeed(qreal speed) {
    m_speed = speed;
}

qreal Entity::getSpeed() const {
    return m_speed;
}

void Entity::setSizeScale(qreal scale) {
    m_scale = scale;
    setScale(m_scale); // QGraphicsItem 的内置缩放
}

qreal Entity::getSizeScale() const {
    return m_scale;
}

void Entity::setVelocity(qreal dx, qreal dy) {
    m_dx = dx;
    m_dy = dy;
}

void Entity::advance(int phase) {
    if (!phase) return;
    
    // 基础移动逻辑
    if (m_dx != 0 || m_dy != 0) {
        setPos(mapToParent(m_dx * m_speed, m_dy * m_speed));
    }
}