#include "Player.h"
#include "Item.h"
#include "../core/GameEngine.h"
#include "FloatingText.h"
#include <QGraphicsScene> // 必须包含，用于获取地图边界
#include <QDebug>

// 构造函数实现：参数不能带默认值
Player::Player(CharacterType type, QObject* parent)
    : Entity(parent), m_type(type), m_isSkillActive(false), m_isSkillCoolingDown(false)
{
    // 初始化计时器
    m_skillDurationTimer = new QTimer(this);
    m_skillDurationTimer->setSingleShot(true);
    connect(m_skillDurationTimer, &QTimer::timeout, this, &Player::onSkillFinished);

    m_skillCooldownTimer = new QTimer(this);
    m_skillCooldownTimer->setSingleShot(true);
    connect(m_skillCooldownTimer, &QTimer::timeout, this, &Player::onCooldownFinished);

    QPixmap pix;
    if (m_type == CharacterType::Agile) {
        pix.load(":/assets/images/fish2.png");
        m_speed = 6.0; 
        m_faceCorrection = -1;
    } else {
        pix.load(":/assets/images/fish1.png");
        m_speed = 5.0;
        m_faceCorrection = 1;
    }

    if (!pix.isNull()) {
        qreal targetWidth = 80.0; // 标准宽度，和 Enemy 保持一致
        qreal ratio = targetWidth / pix.width();
        setBaseRatio(ratio);
        setPixmap(pix);
        
        // 设置变换中心在图片中心
        setTransformOriginPoint(0, 0);
        setOffset(-pix.width() / 2, -pix.height() / 2); // 让坐标点位于图片中心
    }

    if (m_type == CharacterType::Agile) {
        setSizeScale(0.8); 
    } else {
        setSizeScale(1.0);
    }

    m_originalSpeed = m_speed;
    m_normalSpeed = m_speed; // 确保吃绿色加速球时基于当前角色的速度计算
    m_originalSize = pixmap().size(); // 如果需要用到原始尺寸的话
    
}

void Player::onSkillFinished() {
    if (m_type == CharacterType::Agile) {
        m_isSkillActive = false;
        
        // 恢复状态
        setSizeScale(m_scale);
        m_speed = m_originalSpeed;
        
        m_isSkillCoolingDown = true;
        m_skillCooldownTimer->start(SKILL_COOLDOWN); // 8秒冷却
        qDebug() << "Skill Finished, Cooldown started.";
    }
}

void Player::activateSkill() {
    if (m_type == CharacterType::Agile && !m_isSkillActive && !m_isSkillCoolingDown) {
        m_isSkillActive = true;
        
        // 缩小并加速
        setScale(this->scale() * 0.6); // 变小
        m_speed = m_originalSpeed * 2.0; // 2倍速
        
        m_skillDurationTimer->start(SKILL_DURATION); // 3秒

        if (scene()) {
            FloatingText* text = new FloatingText("Rapid Dash!", Qt::cyan, this->pos());
            scene()->addItem(text);
        }
        qDebug() << "Skill Activated!";
    }
}

void Player::onCooldownFinished() {
    m_isSkillCoolingDown = false;
    if (scene()) {
        FloatingText* text = new FloatingText("Skill Ready!", Qt::green, this->pos());
        scene()->addItem(text);
    }
    qDebug() << "Skill Ready!";
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
        setTransform(QTransform::fromScale(-1 * m_faceCorrection, 1)); 
    } 
    else if (keys[Qt::Key_D] || keys[Qt::Key_Right]) {
        dx = m_speed;
        setTransform(QTransform::fromScale(1 * m_faceCorrection, 1));
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

void Player::resetState() {
    // 1. 根据角色类型重置初始大小
    if (m_type == CharacterType::Agile) {
        m_scale = 0.8;
    } else {
        m_scale = 1.0;
    }
    setSizeScale(m_scale); // 应用大小

    // 2. 清除所有临时 Buff 状态
    m_speed = m_originalSpeed;
    m_speedBoostTimer = 0;
    m_invincibleTimer = 0;
    setOpacity(1.0); // 恢复不透明
    
    // 3. 重置技能冷却
    m_isSkillActive = false;
    m_isSkillCoolingDown = false;
    if(m_skillDurationTimer) m_skillDurationTimer->stop();
    if(m_skillCooldownTimer) m_skillCooldownTimer->stop();
}