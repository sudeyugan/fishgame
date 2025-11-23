#include "StartScreen.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

StartScreen::StartScreen(QWidget *parent) : QWidget(parent) {
    // 1. 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom); // 居中靠下
    mainLayout->setContentsMargins(0, 0, 0, 100); // 距离底部留出一定空间
    mainLayout->setSpacing(20); // 按钮之间的间距

    // 2. 按钮通用样式 (样式表)
    QString btnStyle = 
        "QPushButton {"
        "    background-color: rgba(0, 0, 0, 180);" // 半透明黑底
        "    color: #00ffff;"                       // 青色文字
        "    border: 2px solid #00ffff;"            // 青色边框
        "    border-radius: 30px;"                  // 圆角
        "    font-family: 'Microsoft YaHei';"
        "    font-size: 24px;"                      // 字体调大
        "    font-weight: bold;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 255, 255, 50);" // 悬停变亮
        "    color: white;"
        "    border-color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(0, 255, 255, 100);" // 按下高亮
        "}";

    // 3. 主入口按钮 (整合了 新游戏 和 读取存档)
    m_btnContinue = new QPushButton("进入游戏", this);
    m_btnContinue->setFixedSize(240, 65);
    m_btnContinue->setCursor(Qt::PointingHandCursor);
    m_btnContinue->setStyleSheet(btnStyle);

    // 连接信号：点击后发送 loadGameClicked
    // 注意：MainWindow 收到这个信号后，会弹出 SaveLoadDialog 供玩家选择
    connect(m_btnContinue, &QPushButton::clicked, this, &StartScreen::loadGameClicked);
    
    mainLayout->addWidget(m_btnContinue);

    // 4. 游戏指南按钮
    m_btnHelp = new QPushButton("游戏指南", this);
    m_btnHelp->setFixedSize(240, 65);
    m_btnHelp->setCursor(Qt::PointingHandCursor);
    m_btnHelp->setStyleSheet(btnStyle);
    connect(m_btnHelp, &QPushButton::clicked, this, &StartScreen::helpClicked);
    
    mainLayout->addWidget(m_btnHelp);

    // 5. 确保按钮状态正确
    checkSaveFile();
}

void StartScreen::checkSaveFile() {
    // 现在这个函数不再检查文件是否存在，而是始终启用按钮。
    // 因为即使没有存档，玩家也需要点击它来打开弹窗并选择 "新建存档"。
    if (m_btnContinue) {
        m_btnContinue->setEnabled(true);
        m_btnContinue->setToolTip("开始新的冒险或读取进度");
    }
}

void StartScreen::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bg(":/assets/images/start.jpg");

    if (!bg.isNull()) {
        // 绘制背景图并拉伸填满
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        // 兜底颜色
        painter.fillRect(rect(), QColor(0, 20, 60));
    }
}