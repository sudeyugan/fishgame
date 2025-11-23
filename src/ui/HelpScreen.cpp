#include "HelpScreen.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

HelpScreen::HelpScreen(QWidget *parent) : QWidget(parent) {
    // 使用布局管理器来放置内容
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(100, 80, 100, 80); // 设置四周留白
    mainLayout->setSpacing(20);

    // 1. 标题
    QLabel *titleLabel = new QLabel("游戏指南", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #00ffff;"
        "font-family: 'Microsoft YaHei', 'Arial Black';"
        "font-size: 48px;"
        "font-weight: bold;"
        "margin-bottom: 30px;"
    );
    // 给标题加一点发光阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 255, 255, 150));
    shadow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(shadow);
    
    mainLayout->addWidget(titleLabel);

    // 2. 内容区域 (使用 HTML 格式排版，方便对齐和变色)
    QLabel *contentLabel = new QLabel(this);
    contentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    contentLabel->setWordWrap(true);
    contentLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 100);" // 半透明背景框
        "   border-radius: 15px;"
        "   padding: 30px;"
        "   color: white;"
        "   font-size: 18px;"
        "   line-height: 150%;"
        "}"
    );

    QString helpText = R"(
        <style>
            h3 { color: #ffcc00; margin-bottom: 10px; }
            p { margin-bottom: 15px; line-height: 1.6; }
            .key { color: #00ffff; font-weight: bold; }
            .highlight { color: #ff5555; font-weight: bold; }
        </style>
        
        <h3>🎮 游戏目标</h3>
        <p>在危机四伏的深海中生存下去！操控你的小鱼，通过<b>吞噬</b>比自己小的鱼类来变大变强，最终成为海洋霸主。</p>
        
        <h3>⌨️ 操作方法</h3>
        <p>
        • 移动：使用 <span class="key">W A S D</span> 键 或 <span class="key">方向键</span> 控制游动方向。<br>
        • 暂停：游戏中按下 <span class="key">ESC</span> 键可暂停游戏。
        </p>

        <h3>⚠️ 注意事项</h3>
        <p>
        • <span class="highlight">大鱼吃小鱼</span>：千万不要触碰比你大的鱼，否则游戏直接结束！<br>
        • 只要体型比对方大一点点，就可以吞噬它并获得分数。<br>
        • 达成关卡目标分数后，将自动进入下一关。
        </p>
    )";
    contentLabel->setText(helpText);
    
    // 设置 sizePolicy 让文字区域尽可能占满空间
    contentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(contentLabel);

    // 3. 返回按钮
    QPushButton *backBtn = new QPushButton("返回主菜单", this);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setFixedSize(200, 60);
    // 按钮样式：科技感蓝底白字
    backBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4facfe, stop:1 #00f2fe);"
        "    border: none;"
        "    border-radius: 30px;"
        "    color: white;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #66b2ff, stop:1 #33ffff);"
        "    margin-top: -2px;" // 悬停微动效果
        "}"
        "QPushButton:pressed {"
        "    background-color: #007aff;"
        "    margin-top: 2px;"
        "}"
    );

    // 连接信号
    connect(backBtn, &QPushButton::clicked, this, &HelpScreen::backClicked);

    // 按钮居中
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(backBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);
}

void HelpScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    
    // 1. 绘制深海背景图 (如果没有图，就用渐变色兜底)
    // 假设你有一个通用的背景图
    QPixmap bg(":/assets/images/background.jpg"); // 或者 start.jpg
    if (!bg.isNull()) {
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        
        // 2. 叠加一层半透明深蓝色蒙版，让背景变暗，凸显文字
        painter.fillRect(rect(), QColor(0, 20, 40, 200));
    } else {
        // 纯代码绘制深海渐变背景
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(0, 10, 30));   // 深海蓝黑
        gradient.setColorAt(1, QColor(0, 50, 100));  // 稍微亮一点的蓝
        painter.fillRect(rect(), gradient);
    }
}