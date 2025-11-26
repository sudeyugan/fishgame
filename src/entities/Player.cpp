#include "Player.h"
#include "Item.h"
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
    setTransformOriginPoint(0, 0);
    setOffset(-originalPix.width()/2, -originalPix.height()/2);
    
    m_speed = 5.0; 
    m_normalSpeed = 5.0;

    m_speedBoostTimer = 0;
    m_invincibleTimer = 0;

    setSizeScale(1.0); // 逻辑大小设为 1.0 (视觉上会自动应用 ratio 变成 80px)
    m_type = TYPE_PLAYER;
}

void Player::applyEffect(int type) {
    // 这里 type 对应 Item::ItemType
    switch (type) {
        case Item::ITEM_GOLD:
             // 加分逻辑在 Scene 里处理，这里不做事
            break;
        case Item::ITEM_RED:
            grow(0.1); // 额外成长
            break;
        case Item::ITEM_GREEN:
            m_speedBoostTimer = 300; // 加速 5秒 (60帧*5)
            m_speed = m_normalSpeed * 1.5; // 速度 1.5倍
            break;
        case Item::ITEM_BLUE:
            m_invincibleTimer = 300; // 无敌 5秒
            setOpacity(0.5); // 变半透明表示无敌
            break;
    }
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

    // --- 处理状态倒计时 ---
    if (m_speedBoostTimer > 0) {
        m_speedBoostTimer--;
        if (m_speedBoostTimer <= 0) {
            m_speed = m_normalSpeed; // 恢复速度
        }
    }

    if (m_invincibleTimer > 0) {
        m_invincibleTimer--;
        // 简单的视觉特效：闪烁
        if (m_invincibleTimer <= 0) {
            setOpacity(1.0); // 恢复不透明
        } else {
             // 快速闪烁
             setOpacity((m_invincibleTimer % 10 < 5) ? 0.5 : 0.8);
        }
    }

    // 获取按键状态进行移动
    auto &keys = GameEngine::instance().keys();
    qreal dx = 0;
    qreal dy = 0;

    // --- 左右移动 & 翻转 ---
    if (keys[Qt::Key_A] || keys[Qt::Key_Left]) {
        dx = -m_speed;
        setTransform(QTransform::fromScale(-1, 1)); 
    } 
    else if (keys[Qt::Key_D] || keys[Qt::Key_Right]) {
        dx = m_speed;
        setTransform(QTransform::fromScale(1, 1));
    }

    // --- 上下移动 ---
    if (keys[Qt::Key_W] || keys[Qt::Key_Up])   dy = -m_speed;
    if (keys[Qt::Key_S] || keys[Qt::Key_Down]) dy = m_speed;

    // --- 计算新位置 ---
    qreal newX = x() + dx;
    qreal newY = y() + dy;

    if (scene()) {
        QRectF mapRect = scene()->sceneRect();
        
        // 获取实际显示的尺寸 (原图尺寸 * 当前缩放比例)
        // scale() 获取的是 setSizeScale/setBaseRatio 设置后的最终缩放值
        qreal currentScale = this->scale(); 
        qreal visualW = boundingRect().width() * currentScale;
        qreal visualH = boundingRect().height() * currentScale;

        //  setOffset 居中，pos() 是中心点
        // 我们需要计算“半宽”和“半高”来做碰撞限制
        qreal halfW = visualW / 2.0;
        qreal halfH = visualH / 2.0;

        // 限制 X 轴 (中心点不能小于半宽，不能大于 地图宽-半宽)
        if (newX < halfW) newX = halfW;
        if (newX > mapRect.width() - halfW) newX = mapRect.width() - halfW;

        // 限制 Y 轴
        if (newY < halfH) newY = halfH;
        if (newY > mapRect.height() - halfH) newY = mapRect.height() - halfH;
    }

    setPos(newX, newY);
}