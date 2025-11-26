#include "Player.h"
#include "../core/GameEngine.h"
#include <QGraphicsScene> // 必须包含，用于获取地图边界
#include <QDebug>

// 构造函数实现：参数不能带默认值
Player::Player(QObject* parent) : Entity(parent) {
    // 1. 加载并缩放图片
QPixmap originalPix(":/assets/images/fish1.png"); 
    
    // 目标是让它在 scale=1.0 时显示为 80 像素宽
    // 如果原图是 400px，ratio 就是 0.2
    qreal ratio = 80.0 / originalPix.width();
    setBaseRatio(ratio);

    // 3. 设置完整图片
    setPixmap(originalPix);

    // 4. 设置中心点 (用原图的尺寸)
    setTransformOriginPoint(originalPix.width()/2, originalPix.height()/2);
    setOffset(-originalPix.width()/2, -originalPix.height()/2);
    
    m_speed = 5.0; 
    setSizeScale(1.0); // 逻辑大小设为 1.0 (视觉上会自动应用 ratio 变成 80px)
    m_type = TYPE_PLAYER;
}

void Player::grow(qreal amount) {
    m_scale += amount;
    if (m_scale > 5.0) m_scale = 5.0;
    setSizeScale(m_scale);
}

void Player::updateMoveDirection(qreal dx, qreal dy) {
    // 这里只更新意图，实际移动在 advance 里处理
    m_dx = dx;
    m_dy = dy;
}

void Player::advance(int phase) {
    if (!phase) return;

    // 获取按键状态进行移动
    auto &keys = GameEngine::instance().keys();
    qreal dx = 0;
    qreal dy = 0;

    // --- 左右移动 & 翻转 ---
    if (keys[Qt::Key_A] || keys[Qt::Key_Left]) {
        dx = -m_speed;
        // 向左翻转：水平镜像
        setTransform(QTransform::fromScale(-1, 1)); 
    } 
    else if (keys[Qt::Key_D] || keys[Qt::Key_Right]) {
        dx = m_speed;
        // 恢复正常
        setTransform(QTransform::fromScale(1, 1));
    }

    // --- 上下移动 ---
    if (keys[Qt::Key_W] || keys[Qt::Key_Up])   dy = -m_speed;
    if (keys[Qt::Key_S] || keys[Qt::Key_Down]) dy = m_speed;

    // --- 计算新位置并限制在地图内 ---
    qreal newX = x() + dx;
    qreal newY = y() + dy;

    if (scene()) {
        QRectF mapRect = scene()->sceneRect();
        qreal w = boundingRect().width();
        qreal h = boundingRect().height();

        // 限制 X 轴
        if (newX < 0) newX = 0;
        if (newX > mapRect.width() - w) newX = mapRect.width() - w;

        // 限制 Y 轴
        if (newY < 0) newY = 0;
        if (newY > mapRect.height() - h) newY = mapRect.height() - h;
    }

    setPos(newX, newY);
}