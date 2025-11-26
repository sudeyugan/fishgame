#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"

class Item : public Entity {
    Q_OBJECT
public:
    // 定义四种道具类型
    enum ItemType {
        ITEM_GOLD,   // 金色：加分
        ITEM_RED,    // 红色：变大 (成长)
        ITEM_GREEN,  // 绿色：加速
        ITEM_BLUE    // 蓝色：无敌 (暂时免疫伤害)
    };

    explicit Item(QObject* parent = nullptr);

    // 重写类型获取
    EntityType getEntityType() const override { return TYPE_ITEM; }
    
    // 获取具体的道具功能类型
    ItemType getItemType() const { return m_itemType; }

    // 核心逻辑
    void advance(int phase) override;

private:
    ItemType m_itemType;
    int m_lifeTime;       // 存活时间计数
    int m_maxLifeTime;    // 最大存活时间
    int m_moveTimer;      // 移动方向改变计时
};

#endif // ITEM_H