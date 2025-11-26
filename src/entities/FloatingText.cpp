#include "FloatingText.h"
#include <QGraphicsScene>

FloatingText::FloatingText(const QString& text, const QColor& color, const QPointF& pos) 
    : QGraphicsSimpleTextItem(text), m_lifeTime(0), m_maxLifeTime(60), m_opacity(1.0) {
    
    // 1. 设置字体样式
    setFont(QFont("Microsoft YaHei", 16, QFont::Bold)); // 字体、大小、加粗
    setBrush(QBrush(color)); // 文字颜色
    
    // 2. 设置初始位置
    // 先居中对齐：让 (0,0) 点位于文字中心，这样设置 pos 时更准确
    QRectF rect = boundingRect();
    setTransformOriginPoint(rect.width()/2, rect.height()/2);
    
    // 将文字放到指定位置上方一点点
    setPos(pos.x() - rect.width()/2, pos.y() - 40);

    // 3. 确保文字在最上层
    setZValue(100); 
}

void FloatingText::advance(int phase) {
    if (!phase) return;

    // --- 动画逻辑 ---
    
    // 1. 向上飘动
    moveBy(0, -1.5); 

    // 2. 逐渐淡出 (生命周期后半段开始淡出)
    m_lifeTime++;
    if (m_lifeTime > 30) {
        m_opacity -= 0.03; // 快速变透明
        if (m_opacity < 0) m_opacity = 0;
        setOpacity(m_opacity);
    }

    // 3. 销毁
    if (m_lifeTime >= m_maxLifeTime) {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    }
}