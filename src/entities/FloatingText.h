#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QFont>
#include <QPen>

class FloatingText : public QGraphicsSimpleTextItem {
public:
    // 构造函数：需要显示的内容、颜色、以及产生的位置
    FloatingText(const QString& text, const QColor& color, const QPointF& pos);

    // 重写 advance 以实现动画
    void advance(int phase) override;

    // 我们可以给文字加个类型标记，使得它不被视为普通的游戏Entity（防止碰撞逻辑出错）
    enum { Type = UserType + 2 }; 
    int type() const override { return Type; }

private:
    int m_lifeTime;      // 存活时间
    int m_maxLifeTime;   // 最大存活时间
    qreal m_opacity;     // 透明度
};

#endif // FLOATINGTEXT_H