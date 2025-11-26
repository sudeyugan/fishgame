#include "SaveLoadDialog.h"
#include "../core/SaveManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>
#include <QTimer>

SaveLoadDialog::SaveLoadDialog(Mode mode, QWidget *parent) 
    : QDialog(parent), m_mode(mode) 
{
    setWindowTitle(mode == LOAD ? "选择存档" : "保存进度");
    // 增加高度以容纳更多存档，或者使用滚动区域
    setFixedSize(550, 600); 
    setModal(true);
    
    // 样式表优化
    setStyleSheet(
        "QDialog { background-color: #001a33; }"
        "QScrollArea { border: none; background: transparent; }"
        "QScrollBar:vertical { background: #001122; width: 10px; }"
        "QScrollBar::handle:vertical { background: #005580; border-radius: 5px; }"
        "QGroupBox { border: 2px solid #00aaaa; border-radius: 8px; margin-top: 10px; background-color: rgba(0, 30, 50, 150); }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 5px; color: #00ffff; }"
        "QLabel { color: #ccffff; font-size: 14px; font-family: 'Arial'; }"
        // 默认按钮样式
        "QPushButton { background-color: #005580; color: white; border: 1px solid #00aaaa; border-radius: 5px; padding: 6px 12px; }"
        "QPushButton:hover { background-color: #0077aa; border-color: white; }"
        // 删除按钮特定样式 (红色)
        "QPushButton#deleteBtn { background-color: #802020; border: 1px solid #aa4444; min-width: 30px; }"
        "QPushButton#deleteBtn:hover { background-color: #aa3030; border-color: #ff6666; }"
    );

    initUI();
}

void SaveLoadDialog::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel *title = new QLabel(m_mode == LOAD ? ">>> 选择存档启动 <<<" : ">>> 选择保存位置 <<<", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #00ffff; margin-bottom: 5px;");
    mainLayout->addWidget(title);

    // 创建滚动区域，防止5个存档超出屏幕
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setStyleSheet("background: transparent;"); // 透明背景
    m_slotsLayout = new QVBoxLayout(scrollWidget); // 将布局保存到成员变量，方便刷新
    m_slotsLayout->setSpacing(15);
    m_slotsLayout->setAlignment(Qt::AlignTop);

    refreshList(); // 初次填充列表

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // 底部取消按钮
    QPushButton *btnClose = new QPushButton("返回", this);
    btnClose->setFixedHeight(40);
    btnClose->setStyleSheet("background-color: #333; border: 1px solid #666; font-size: 16px;");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(btnClose);
}

void SaveLoadDialog::refreshList() {
    // 1. 清除旧的列表项
    QLayoutItem *item;
    while ((item = m_slotsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    // 2. 获取最新数据
    QList<SaveSlotInfo> slotList = SaveManager::getAllSlots(SaveManager::MAX_SLOTS);

    // 3. 重新生成列表
    for (const auto &info : slotList) {
        QGroupBox *box = new QGroupBox(QString(" SLOT %1 ").arg(info.slotIndex + 1));
        box->setFixedHeight(90); // 固定高度
        
        QHBoxLayout *boxLayout = new QHBoxLayout(box);
        boxLayout->setContentsMargins(15, 10, 15, 10);

        // 显示信息
        QLabel *infoLabel = new QLabel(box);
        if (info.isEmpty) {
            infoLabel->setText(" --  空  -- ");
            infoLabel->setStyleSheet("color: #666; font-style: italic; font-size: 16px;");
            infoLabel->setAlignment(Qt::AlignCenter);
        } else {
            infoLabel->setText(QString("<b style='font-size:16px; color:#ffcc00;'>LV.%1</b>  <span style='color:#aaa;'>|</span>  分数: %2\n<span style='color:#888; font-size:12px;'>%3</span>")
                               .arg(info.level).arg(info.score).arg(info.timestamp));
            infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
        boxLayout->addWidget(infoLabel, 1);

        // 操作按钮区域
        
        // [按钮1] 主要动作 (读取/保存)
        QPushButton *btnAction = new QPushButton(box);
        btnAction->setCursor(Qt::PointingHandCursor);
        btnAction->setFixedHeight(36);

        if (m_mode == LOAD) {
            btnAction->setText(info.isEmpty ? "新建游戏" : "开始读取");
            // 根据是否为空，标记是否是 NewGame
            connect(btnAction, &QPushButton::clicked, [=](){
                emit slotSelected(info.slotIndex, info.isEmpty); 
                accept();
            });
        } else {
            btnAction->setText(info.isEmpty ? "保存" : "覆盖");
            // 如果是覆盖，可以根据需要把字体变红提示
            if(!info.isEmpty) btnAction->setStyleSheet("color: #ffaaaa; border-color: #ffaaaa;");
            
            connect(btnAction, &QPushButton::clicked, [=](){
                emit slotSelected(info.slotIndex, false); 
                accept();
            });
        }
        boxLayout->addWidget(btnAction);

        // [按钮2] 删除按钮 (仅当存档存在时显示)
        if (!info.isEmpty) {
            QPushButton *btnDel = new QPushButton("X", box);
            btnDel->setObjectName("deleteBtn"); // 使用样式表中的 ID
            btnDel->setCursor(Qt::PointingHandCursor);
            btnDel->setToolTip("删除此存档");
            btnDel->setFixedSize(36, 36);
            
            connect(btnDel, &QPushButton::clicked, [=](){
                // 这里可以再加一个确认弹窗，为了演示简单直接删除
                SaveManager::deleteSave(info.slotIndex);
                refreshList(); // 删除后刷新界面！
            });
            boxLayout->addWidget(btnDel);
        } else {
            // 占位，保持对齐
            QWidget *spacer = new QWidget();
            spacer->setFixedSize(36, 36);
            boxLayout->addWidget(spacer);
        }

        m_slotsLayout->addWidget(box);
    }
}

// --- 静态美化弹窗实现 ---
void SaveLoadDialog::showMessageBox(QWidget *parent, const QString &title, const QString &content) {
    QDialog dlg(parent);
    dlg.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    dlg.setAttribute(Qt::WA_TranslucentBackground);
    dlg.setFixedSize(300, 180);
    dlg.setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(20, 20, 20, 20);

    // 背景板
    QFrame *bg = new QFrame(&dlg);
    bg->setStyleSheet(
        "QFrame { background-color: rgba(0, 25, 45, 240); border: 2px solid #00aaaa; border-radius: 10px; }"
    );
    QVBoxLayout *bgLayout = new QVBoxLayout(bg);

    QLabel *lblTitle = new QLabel(title, bg);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #00ffff; font-size: 18px; font-weight: bold; border: none; background: transparent;");

    QLabel *lblContent = new QLabel(content, bg);
    lblContent->setAlignment(Qt::AlignCenter);
    lblContent->setWordWrap(true);
    lblContent->setStyleSheet("color: white; font-size: 15px; margin: 10px 0; border: none; background: transparent;");

    QPushButton *btnOk = new QPushButton("确定", bg);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(
        "QPushButton { background-color: #005580; color: white; border: 1px solid #00aaaa; border-radius: 5px; padding: 5px; min-width: 80px; }"
        "QPushButton:hover { background-color: #0077aa; }"
    );
    QObject::connect(btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);

    bgLayout->addWidget(lblTitle);
    bgLayout->addWidget(lblContent);
    bgLayout->addWidget(btnOk, 0, Qt::AlignCenter);

    layout->addWidget(bg);
    dlg.exec();
}