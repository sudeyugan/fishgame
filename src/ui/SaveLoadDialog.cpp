#include "SaveLoadDialog.h"
#include "../core/SaveManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

SaveLoadDialog::SaveLoadDialog(Mode mode, QWidget *parent) 
    : QDialog(parent), m_mode(mode) 
{
    setWindowTitle(mode == LOAD ? "选择存档" : "保存进度");
    setFixedSize(500, 450);
    setModal(true);
    
    // 样式表 (深色科技风)
    setStyleSheet(
        "QDialog { background-color: #001a33; }"
        "QGroupBox { border: 2px solid #00aaaa; border-radius: 5px; margin-top: 10px; font-size: 14px; color: white; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 5px; }"
        "QLabel { color: #ccffff; font-size: 16px; }"
        "QPushButton { background-color: #005580; color: white; border: 1px solid #00aaaa; border-radius: 5px; padding: 8px; min-width: 80px; }"
        "QPushButton:hover { background-color: #0077aa; }"
        "QPushButton:disabled { background-color: #333; color: #888; border-color: #555; }"
    );

    initUI();
}

void SaveLoadDialog::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel(m_mode == LOAD ? "请选择一个存档开始游戏" : "请选择保存位置", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #00ffff; margin-bottom: 10px;");
    mainLayout->addWidget(title);

    // 获取所有存档信息
    QList<SaveSlotInfo> slot = SaveManager::getAllSlots(3);

    for (const auto &info : slot) {
        QGroupBox *box = new QGroupBox(QString(" 存档 %1 ").arg(info.slotIndex + 1));
        QHBoxLayout *boxLayout = new QHBoxLayout(box);

        QLabel *infoLabel = new QLabel(box);
        if (info.isEmpty) {
            infoLabel->setText(" -- 空槽位 -- ");
            infoLabel->setStyleSheet("color: #666; font-style: italic;");
        } else {
            infoLabel->setText(QString("第 %1 关  |  分数: %2\n时间: %3")
                               .arg(info.level).arg(info.score).arg(info.timestamp));
        }
        boxLayout->addWidget(infoLabel, 1); // 占据左侧空间

        // 按钮逻辑
        QPushButton *btnAction = new QPushButton(box);
        btnAction->setCursor(Qt::PointingHandCursor);

        if (m_mode == LOAD) {
            // 读档模式
            if (info.isEmpty) {
                btnAction->setText("新建存档"); // 没档 -> 新建
                // 点击新建：触发 slotSelected(index, true)
                connect(btnAction, &QPushButton::clicked, [=](){
                    emit slotSelected(info.slotIndex, true); 
                    accept();
                });
            } else {
                btnAction->setText("读取进度"); // 有档 -> 读取
                // 点击读取：触发 slotSelected(index, false)
                connect(btnAction, &QPushButton::clicked, [=](){
                    emit slotSelected(info.slotIndex, false);
                    accept();
                });
            }
        } else {
            // 存档模式
            btnAction->setText(info.isEmpty ? "保存到此" : "覆盖保存");
            connect(btnAction, &QPushButton::clicked, [=](){
                emit slotSelected(info.slotIndex, false); // Save模式下第二个参数无所谓
                accept();
            });
        }
        boxLayout->addWidget(btnAction);

        mainLayout->addWidget(box);
    }

    // 底部关闭按钮
    QPushButton *btnClose = new QPushButton("取消", this);
    btnClose->setStyleSheet("background-color: #444; border-color: #666; margin-top: 10px;");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(btnClose);
}