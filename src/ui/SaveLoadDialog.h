#ifndef SAVELOADDIALOG_H
#define SAVELOADDIALOG_H

#include <QDialog>

class SaveLoadDialog : public QDialog {
    Q_OBJECT
public:
    // Mode: LOAD 表示读取/新建模式，SAVE 表示保存模式
    enum Mode { LOAD, SAVE };
    
    explicit SaveLoadDialog(Mode mode, QWidget *parent = nullptr);

signals:
    // 信号：用户选择了一个槽位进行操作
    // slotIndex: 槽位号
    // isNewGame: 是否是点击了“新建存档” (仅 Load 模式有效)
    void slotSelected(int slotIndex, bool isNewGame);

private:
    Mode m_mode;
    void initUI();
};

#endif // SAVELOADDIALOG_H