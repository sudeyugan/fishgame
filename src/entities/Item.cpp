#include "Item.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QtMath>

Item::Item(QObject* parent) : Entity(parent), m_lifeTime(0), m_moveTimer(0) {
    // 1. 随机生成一种类型
    int type = QRandomGenerator::global()->bounded(4); // 0-3
    m_itemType = static_cast<ItemType>(type);

    // 2. 根据类型设置图片
    QString imgPath;
    switch (m_itemType) {
        case ITEM_GOLD:  imgPath = ":/assets/images/goldbubble.png"; break;
        case ITEM_RED:   imgPath = ":/assets/images/redbubble.png"; break;
        case ITEM_GREEN: imgPath = ":/assets/images/greenbubble.png"; break;
        case ITEM_BLUE:  imgPath = ":/assets/images/bluebubble.png"; break;
    }

    QPixmap p(imgPath);
    // 道具统一设置较小的尺寸，例如 40-50像素
    qreal ratio = 50.0 / p.width();
    setBaseRatio(ratio);
    setPixmap(p);

    // 修正坐标中心 (参考之前修复 Enemy 的逻辑)
    setTransformOriginPoint(0, 0);
    setOffset(-p.width()/2, -p.height()/2);

    // 3. 初始随机速度 (漂浮感，速度较慢)
    m_speed = 1.0 + QRandomGenerator::global()->generateDouble(); 
    
    // 4. 设置消失时间 (例如 600帧，约10秒)
    m_maxLifeTime = 600; 
}

void Item::advance(int phase) {
    if (!phase) return;

    // --- 1. 随机漂浮移动逻辑 ---
    m_moveTimer++;
    // 每 60 帧 (约1秒) 改变一次微小方向，模拟气泡不规则漂浮
    if (m_moveTimer > 60) {
        int angle = QRandomGenerator::global()->bounded(360);
        qreal rad = qDegreesToRadians((double)angle);
        
        m_dx = std::cos(rad);
        m_dy = std::sin(rad);
        
        m_moveTimer = 0;
    }

    // 移动
    qreal newX = x() + m_dx * m_speed;
    qreal newY = y() + m_dy * m_speed;
    
    // 简单的边界反弹 (防止气泡飘出地图太远)
    if (scene()) {
        if (newX < 0 || newX > scene()->width()) m_dx = -m_dx;
        if (newY < 0 || newY > scene()->height()) m_dy = -m_dy;
    }
    
    setPos(newX, newY);

    // --- 2. 存活时间逻辑 ---
    m_lifeTime++;
    
    // 快消失时闪烁效果 (透明度变化)
    if (m_lifeTime > m_maxLifeTime - 100) {
        if ((m_lifeTime / 10) % 2 == 0) setOpacity(0.5);
        else setOpacity(1.0);
    }

    // 超时销毁
    if (m_lifeTime >= m_maxLifeTime) {
        if (scene()) scene()->removeItem(this);
        delete this;
    }
}