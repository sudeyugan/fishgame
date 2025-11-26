#ifndef BACKGROUNDSELECTDIALOG_H
#define BACKGROUNDSELECTDIALOG_H

#include <QDialog>

class BackgroundSelectDialog : public QDialog {
    Q_OBJECT
public:
    explicit BackgroundSelectDialog(QWidget *parent = nullptr);
    int getSelectedIndex() const { return m_selectedIndex; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_selectedIndex; // 1, 2, 3
};

#endif // BACKGROUNDSELECTDIALOG_H