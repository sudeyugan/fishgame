#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <QTimer>               // 必须包含
#include "../core/Constants.h"  // 必须包含 CharacterType 定义

class Player : public Entity {
    Q_OBJECT
public:
    // 构造函数接收 CharacterType
    explicit Player(CharacterType type, QObject* parent = nullptr);

    EntityType getEntityType() const override { return TYPE_PLAYER; }

    void grow(qreal amount);
    void updateMoveDirection(qreal dx, qreal dy);
    void applyEffect(int type);
    bool isInvincible() const { return m_invincibleTimer > 0; }

    CharacterType getType() const { return m_type; }

    // 技能激活函数
    void activateSkill();
    
    void resetState();

protected:
    // 游戏循环的核心更新函数是 advance，而不是 update
    void advance(int phase) override; 

private slots:
    void onSkillFinished();
    void onCooldownFinished();

private:
    CharacterType m_type;
    int m_faceCorrection;

    // 技能相关
    bool m_isSkillActive;
    bool m_isSkillCoolingDown;
    QTimer *m_skillDurationTimer;
    QTimer *m_skillCooldownTimer;

    // 属性备份
    qreal m_originalSpeed; 
    QSizeF m_originalSize; 
    
    // 旧属性
    int m_speedBoostTimer = 0;
    int m_invincibleTimer = 0;
    qreal m_normalSpeed = 5.0;
};

#endif // PLAYER_H