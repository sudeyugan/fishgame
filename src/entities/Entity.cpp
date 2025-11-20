#include "Entity.h"
#include <QGraphicsScene>
#include <QPainter>

Entity::Entity(QObject* parent) 
    : QObject(parent), m_speed(0), m_scale(1.0), m_dx(0), m_dy(0) {
    setTransformationMode(Qt::SmoothTransformation);
    setFlag(QGraphicsItem::ItemIsFocusable, false); // 只有Player需要Focus
}

void Entity::setSpeed(qreal speed) { m_speed = speed; }
qreal Entity::getSpeed() const { return m_speed; }

void Entity::setSizeScale(qreal scale) {
    m_scale = scale;
    setScale(m_scale);
}
qreal Entity::getSizeScale() const { return m_scale; }

void Entity::setVelocity(qreal dx, qreal dy) {
    m_dx = dx;
    m_dy = dy;
}

void Entity::advance(int phase) {
    if (!phase) return;

    // 1. 移动
    if (m_dx != 0 || m_dy != 0) {
        setPos(mapToParent(m_dx, m_dy));
    }

    // 2. 【核心修复】边界限制 (Clamping)
    // 无论速度多快，都不能飞出 sceneRect
    if (scene()) {
        QRectF bounds = scene()->sceneRect();
        QPointF curr = pos();
        qreal x = curr.x();
        qreal y = curr.y();
        bool modified = false;

        // 预留一点 buffer (比如半个身位)，防止贴边卡死
        qreal buffer = 10.0; 

        if (x < bounds.left() + buffer) { x = bounds.left() + buffer; modified = true; }
        else if (x > bounds.right() - buffer) { x = bounds.right() - buffer; modified = true; }

        if (y < bounds.top() + buffer) { y = bounds.top() + buffer; modified = true; }
        else if (y > bounds.bottom() - buffer) { y = bounds.bottom() - buffer; modified = true; }

        if (modified) {
            setPos(x, y);
        }
    }
}

// 3. 【美化】如果没图，画个形状
void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (pixmap().isNull()) {
        // 自动绘制替代图形
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        
        if (type() == Entity::Type) { // 基类不画
        } 
        // 子类可以通过覆盖 paint 或者 load 失败后进入这里
        // 这里做一个简单的 fallback
        painter->setBrush(Qt::gray);
        painter->drawEllipse(-10, -10, 20, 20);
        painter->restore();
    } else {
        QGraphicsPixmapItem::paint(painter, option, widget);
    }
}