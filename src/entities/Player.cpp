#include "Player.h"
#include <QPainter>

Player::Player(QObject* parent) : Entity(parent) {
    // 1. 加载原始大图
    QPixmap originalPix(":/assets/images/fish1.jpg"); 
    
    // 2. 【核心修改】缩放图片到合适的大小

    QPixmap finalPix = originalPix.scaledToWidth(80, Qt::SmoothTransformation); 

    // 3. 设置缩放后的图片
    setPixmap(finalPix);

    setTransformOriginPoint(finalPix.width()/2, finalPix.height()/2);
    setOffset(-finalPix.width()/2, -finalPix.height()/2);
    
    m_speed = 5.0; 
    setSizeScale(1.0); 
    m_type = TYPE_PLAYER;
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