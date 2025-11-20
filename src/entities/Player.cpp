#include "Player.h"
#include "Constants.h"
#include <QBrush>

Player::Player() {
    // 玩家默认显示为绿色
    QPixmap pix(50, 50);
    pix.fill(Qt::green);
    setPixmap(pix);
    
    m_speed = 5.0;
    setZValue(10); // 保证玩家在最上层
}

void Player::grow() {
    m_scale += 0.1;
    setScale(m_scale);
}

void Player::handleInput(const QSet<int>& keys) {
    qreal dx = 0;
    qreal dy = 0;

    if (keys.contains(Qt::Key_W) || keys.contains(Qt::Key_Up)) dy = -m_speed;
    if (keys.contains(Qt::Key_S) || keys.contains(Qt::Key_Down)) dy = m_speed;
    if (keys.contains(Qt::Key_A) || keys.contains(Qt::Key_Left)) dx = -m_speed;
    if (keys.contains(Qt::Key_D) || keys.contains(Qt::Key_Right)) dx = m_speed;

    moveBy(dx, dy);
}

void Player::advance(int phase) {
    if (!phase) return;
    // 边界检测
    if (x() < 0) setX(0);
    if (y() < 0) setY(0);
    if (x() > Config::WINDOW_WIDTH - 50) setX(Config::WINDOW_WIDTH - 50);
    if (y() > Config::WINDOW_HEIGHT - 50) setY(Config::WINDOW_HEIGHT - 50);
}