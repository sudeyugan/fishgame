#include "Entity.h"

Entity::Entity(QObject* parent) : QObject(parent), m_speed(1.0), m_scale(1.0) {
    // 如果没有图片，先画一个默认的矩形，保证你能看到东西
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::gray); 
    setPixmap(pixmap);
}

void Entity::setSpeed(qreal speed) {
    m_speed = speed;
}

void Entity::setSizeScale(qreal scale) {
    m_scale = scale;
    setScale(m_scale); // QGraphicsItem 的内置缩放
}

void Entity::advance(int phase) {
    // 默认不移动，由子类实现
}