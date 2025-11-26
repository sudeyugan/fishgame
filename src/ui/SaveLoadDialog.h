#ifndef SAVELOADDIALOG_H
#define SAVELOADDIALOG_H

#include <QDialog>
#include <QVBoxLayout>

class SaveLoadDialog : public QDialog {
    Q_OBJECT
public:
    // Mode: LOAD 表示读取/新建模式，SAVE 表示保存模式
    enum Mode { LOAD, SAVE };
    
    explicit SaveLoadDialog(Mode mode, QWidget *parent = nullptr);
    static void showMessageBox(QWidget *parent, const QString &title, const QString &content);
signals:
    void slotSelected(int slotIndex, bool isNewGame);

private:
    Mode m_mode;
    QVBoxLayout *m_slotsLayout; 

    void initUI();
    void refreshList();
};

#endif // SAVELOADDIALOG_H