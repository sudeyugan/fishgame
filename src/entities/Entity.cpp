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

    // 1. 移动修复
    // 错误写法: setPos(mapToParent(m_dx, m_dy)); 
    // 正确写法: 直接在当前坐标(x, y)上加上位移量(m_dx, m_dy)
    // 假设 m_dx, m_dy 是基于屏幕的世界坐标系速度 (例如按左键就是 x-5)
    if (m_dx != 0 || m_dy != 0) {
        setPos(x() + m_dx, y() + m_dy);
    }

    // 2. 边界限制 (保持不变，但这部分逻辑是好的)
    if (scene()) {
        QRectF bounds = scene()->sceneRect();
        QPointF curr = pos();
        qreal px = curr.x();
        qreal py = curr.y();
        bool modified = false;

        // 建议：这里的 buffer 最好考虑物体实际宽度的一半，而不是写死 10.0
        // qreal buffer = boundingRect().width() * scale() / 2.0; 
        qreal buffer = 20.0; // 暂时稍微加大一点

        if (px < bounds.left() + buffer) { px = bounds.left() + buffer; modified = true; }
        else if (px > bounds.right() - buffer) { px = bounds.right() - buffer; modified = true; }

        if (py < bounds.top() + buffer) { py = bounds.top() + buffer; modified = true; }
        else if (py > bounds.bottom() - buffer) { py = bounds.bottom() - buffer; modified = true; }

        if (modified) {
            setPos(px, py);
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