#include "Player.h"
#include <QDebug>

Player::Player(QObject* parent) : Entity(parent) {
    // 加载主角图片 (请确保路径正确，或者先用颜色块代替测试)
    setPixmap(QPixmap(":/assets/player.png")); // 如果没有图片，会显示空白，需注意
    if (pixmap().isNull()) {
        qDebug() << "Error: Player image not found!";
        // 创建一个默认的绿色方块代替图片，防止看不见
        QPixmap defaultPix(40, 40);
        defaultPix.fill(Qt::green);
        setPixmap(defaultPix);
    }
    
    m_speed = 4.0; // 玩家速度稍快
    setSizeScale(1.0);
    
    // 居中原点，方便旋转和碰撞
    setTransformOriginPoint(boundingRect().center());
}

void Player::grow(qreal amount) {
    m_scale += amount;
    setSizeScale(m_scale);
}

void Player::updateMoveDirection(qreal dx, qreal dy) {
    m_dx = dx;
    m_dy = dy;

    // 简单的视觉翻转：向左游时翻转图片
    if (dx < 0) {
        QTransform transform;
        transform.scale(-1, 1); // 水平翻转
        // 需要修正位置偏移，或者简单地使用 setScale(-m_scale, m_scale) 但这会影响 logic scale
        // 这里为了简单，只做 scale 的负值处理
        setTransform(QTransform().scale(-1, 1).translate(-boundingRect().width(), 0));
    } else if (dx > 0) {
        setTransform(QTransform());
    }
}