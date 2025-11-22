#include "Enemy.h"
#include "Player.h" // 确保包含 Player 头文件以访问 pos() 和 getSizeScale()
#include <QtMath>
#include <QRandomGenerator>
#include <QVector2D>
#include <QDebug>

Enemy::Enemy(Player* target, QObject* parent)
    : Entity(parent), m_target(target), m_directionTimer(0) {
    
    // 1. 随机生成大小倍率 (0.5倍 到 2.5倍)
    // 这个 m_scale 应该是在 Entity 中定义的 protected 变量，或者 Enemy 的成员变量
    // 如果 Entity 里没有 m_scale，请在 Enemy.h 里加一个 qreal m_scale;
    qreal minScale = 0.5;
    qreal maxScale = 2.5;
    m_scale = minScale + QRandomGenerator::global()->generateDouble() * (maxScale - minScale);

    // 2. 根据大小决定加载哪张图
    QString imagePath;
    if (m_scale < 1.0) {
        imagePath = ":/assets/images/enemyfish1.png"; 
    } else if (m_scale < 1.8) {
        imagePath = ":/assets/images/enemyfish2.png"; 
    } else {
        imagePath = ":/assets/images/enemyfish3.png"; 
    }

    // 3. 【修改】加载并缩放图片
    QPixmap p(imagePath);
    if (p.isNull()) {
        p = QPixmap(40, 40);
        p.fill(Qt::red);
    } else {
        // 【关键】统一将图片缩小到标准尺寸 (例如宽度 80px)
        // 这样 m_scale = 1.0 时就是 80px，m_scale = 2.0 时就是 160px
        p = p.scaledToWidth(80, Qt::SmoothTransformation);
    }

    setPixmap(p);
    
    // 【关键】设置中心点为图片中心，这样缩放和翻转才正常
    // 使用 Offset 将坐标原点移动到图片中心
    setTransformOriginPoint(p.width()/2, p.height()/2);
    setOffset(-p.width()/2, -p.height()/2);

    // 4. 应用初始大小
    // 注意：这里只调用 setSizeScale 可能不够，因为 performAI 会覆盖 transform
    // 所以我们依靠 performAI 里每一帧的刷新，或者手动设置一次 transform
    setSizeScale(m_scale); 

    // 5. 随机速度
    m_speed = 4.0 / m_scale; 
    if (m_speed < 1.0) m_speed = 1.0;
}

void Enemy::advance(int phase) {
    if (!phase) return;

    performAI(); // 计算方向和翻转
    Entity::advance(phase); // 执行移动
}

void Enemy::performAI() {
    // 如果没有目标或目标已死亡，就不用追了
    if (!m_target) return; 

    QLineF line(pos(), m_target->pos());
    qreal dist = line.length();

    // --- 移动逻辑 ---
    if (dist < 350) { // 感知范围
        qreal angle = -line.angle(); 
        qreal rad = qDegreesToRadians(angle);

        // 比较体积大小 (确保 Player 也有 getSizeScale 方法)
        if (this->getSizeScale() > m_target->getSizeScale()) {
            // 追逐
            m_dx = std::cos(rad) * m_speed;
            m_dy = std::sin(rad) * m_speed;
        } else {
            // 逃跑
            m_dx = -std::cos(rad) * m_speed; 
            m_dy = -std::sin(rad) * m_speed;
        }
    } else {
        // 巡逻
        m_directionTimer++;
        if (m_directionTimer > 120) {
            int angle = QRandomGenerator::global()->bounded(0, 360);
            qreal rad = qDegreesToRadians((double)angle);
            m_dx = std::cos(rad) * m_speed * 0.5;
            m_dy = std::sin(rad) * m_speed * 0.5;
            m_directionTimer = 0;
        }
    }

    // --- 【修改】翻转逻辑 ---
    // 必须结合当前的 m_scale 进行翻转，否则会变回原大小
    QTransform t;
    
    // 根据 m_dx (X轴速度) 判断朝向
    if (m_dx < 0) {
        // 向左游：X 轴负缩放 (翻转)，Y 轴正缩放
        // 假设原图是朝右的
        t.scale(-m_scale, m_scale); 
    } else {
        // 向右游：X 轴正缩放，Y 轴正缩放
        t.scale(m_scale, m_scale);
    }
    
    // 应用变换
    setTransform(t);
}