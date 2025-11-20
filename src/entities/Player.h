#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <QKeyEvent>

class Player : public Entity {
public:
    Player();
    int type() const override { return TYPE_PLAYER; }
    
    // 处理键盘事件
    void handleInput(const QSet<int>& keys);
    void grow(); // 吃到鱼变大

protected:
    void advance(int phase) override;
};

#endif