#include "RewardDialog.h"
#include <QGraphicsDropShadowEffect>

RewardDialog::RewardDialog(QWidget *parent) : QDialog(parent) {
    // 1. 窗口基础设置
    setWindowTitle("恭喜过关！");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint); // 无边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明
    setModal(true);
    resize(700, 450);

    setupUI();
}

void RewardDialog::setupUI() {
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 2. 背景容器层
    QWidget *bgWidget = new QWidget(this);
    bgWidget->setObjectName("bgWidget");
    bgWidget->setStyleSheet(
        "QWidget#bgWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #001a33, stop:1 #00334e);"
        "    border: 3px solid #00e5ff;"
        "    border-radius: 20px;"
        "}"
    );

    // 添加阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 229, 255, 120));
    shadow->setOffset(0, 0);
    bgWidget->setGraphicsEffect(shadow);
    mainLayout->addWidget(bgWidget);

    // 内容布局
    QVBoxLayout *contentLayout = new QVBoxLayout(bgWidget);
    contentLayout->setContentsMargins(30, 30, 30, 40);
    contentLayout->setSpacing(25);

    // 3. 标题栏
    QLabel *titleLabel = new QLabel("恭喜过关!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-family: 'Arial Black'; font-size: 36px; font-weight: bold; font-style: italic;"
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ffd700, stop:1 #ffaa00);"
        "background: transparent;"
    );
    contentLayout->addWidget(titleLabel);

    QLabel *subTitleLabel = new QLabel("选择你的进化路径 / CHOOSE YOUR EVOLUTION", this);
    subTitleLabel->setAlignment(Qt::AlignCenter);
    subTitleLabel->setStyleSheet("font-size: 16px; color: #a0e0ff; background: transparent;");
    contentLayout->addWidget(subTitleLabel);

    // 4. 选项按钮区域
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(25);

    // 选项1：疾风之鳍 (使用绿色气泡)
    QPushButton *btnSpeed = createFancyButton(
        "疾风之鳍", 
        "移动速度永久 +10%", 
        ":/assets/images/greenbubble.png", 
        "#00e5ff"
    );
    connect(btnSpeed, &QPushButton::clicked, [this](){
        emit rewardSelected(0);
        accept();
    });
    btnLayout->addWidget(btnSpeed);

    // 选项2：巨人之心 (使用红色气泡)
    QPushButton *btnSize = createFancyButton(
        "巨人之心", 
        "初始体型永久 +0.2", 
        ":/assets/images/redbubble.png", 
        "#ff5555"
    );
    connect(btnSize, &QPushButton::clicked, [this](){
        emit rewardSelected(1);
        accept();
    });
    btnLayout->addWidget(btnSize);

    // 选项3：贪婪之口 (使用金色气泡)
    QPushButton *btnGrowth = createFancyButton(
        "贪婪之口", 
        "成长效率提升 +20%", 
        ":/assets/images/goldbubble.png", 
        "#d500f9"
    );
    connect(btnGrowth, &QPushButton::clicked, [this](){
        emit rewardSelected(2);
        accept();
    });
    btnLayout->addWidget(btnGrowth);

    contentLayout->addLayout(btnLayout);
}

QPushButton* RewardDialog::createFancyButton(const QString& title, const QString& desc, const QString& iconPath, const QString& themeColor) {
    QPushButton *btn = new QPushButton();
    btn->setFixedSize(180, 220);
    btn->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *layout = new QVBoxLayout(btn);
    layout->setContentsMargins(15, 20, 15, 20);
    layout->setSpacing(10);

    // 图标
    QLabel *iconLabel = new QLabel(btn);
    // 稍微调大一点图标显示，64x64 -> 80x80，看起来更饱满
    iconLabel->setPixmap(QPixmap(iconPath).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("background: transparent; border: none;");
    layout->addWidget(iconLabel);

    // 标题
    QLabel *titleLabel = new QLabel(title, btn);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QString("font-size: 20px; font-weight: bold; color: %1; background: transparent; border: none;").arg(themeColor));
    layout->addWidget(titleLabel);

    // 描述
    QLabel *descLabel = new QLabel(desc, btn);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("font-size: 14px; color: #cccccc; background: transparent; border: none;");
    layout->addWidget(descLabel);

    // 按钮样式
    btn->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: rgba(0, 40, 80, 180);"
        "    border: 2px solid %1;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(%2, 80);"
        "    border: 2px solid %1;"
        "    background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1, fx:0.5, fy:0.5, stop:0 rgba(%2, 100), stop:1 rgba(0, 40, 80, 220));"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(%2, 150);"
        "    border: 2px solid white;"
        "    margin: 2px;"
        "}"
    ).arg(themeColor).arg(QColor(themeColor).name()));

    return btn;
}