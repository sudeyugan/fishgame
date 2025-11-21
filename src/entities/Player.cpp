#include "Player.h"
#include <QPainter>

Player::Player(QObject* parent) : Entity(parent) {
    // 尝试加载图片
    QPixmap pix(":/assets/fish1.png");
    if (!pix.isNull()) {
        setPixmap(pix);
        setTransformOriginPoint(pix.width()/2, pix.height()/2); // 中心旋转
        setOffset(-pix.width()/2, -pix.height()/2); // 中心对齐
    } else {
        // 如果没图片，设置一个虚拟的边界矩形，否则碰撞检测会失效
        // 假设鱼的大小是 60x40
    }
    
    m_speed = 5.0; 
    setSizeScale(1.0);
}

// 【美化】重写 paint，如果没图，手绘一条金鱼
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (pixmap().isNull()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        // 画鱼身 (橙色)
        painter->setBrush(QColor(255, 165, 0)); 
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPoint(0,0), 30, 20);

        // 画尾巴
        QPolygonF tail;
        tail << QPointF(-25, 0) << QPointF(-45, -15) << QPointF(-45, 15);
        painter->drawPolygon(tail);

        // 画眼睛
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(15, -5), 3, 3);

        painter->restore();
    } else {
        QGraphicsPixmapItem::paint(painter, option, widget);
    }
}

// 必须重写 boundingRect 否则没图时碰撞检测失效
QRectF Player::boundingRect() const {
    if (pixmap().isNull()) {
        return QRectF(-45, -20, 80, 40); // 手绘鱼的大小
    }
    return QGraphicsPixmapItem::boundingRect();
}

void Player::grow(qreal amount) {
    m_scale += amount;
    if (m_scale > 5.0) m_scale = 5.0; // 限制最大体积
    setSizeScale(m_scale);
}

void Player::updateMoveDirection(qreal dx, qreal dy) {
    m_dx = dx;
    m_dy = dy;

    // 翻转逻辑
    if (dx < 0) {
        // 向左游：如果原图朝右，需要翻转
        // 注意：setScale(-1, 1) 会影响子控件，这里只改 Transform
        QTransform t;
        t.scale(-1, 1); 
        setTransform(t);
    } else if (dx > 0) {
        setTransform(QTransform());
    }
}