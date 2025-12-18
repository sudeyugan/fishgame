#ifndef REWARDDIALOG_H
#define REWARDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class RewardDialog : public QDialog {
    Q_OBJECT
public:
    explicit RewardDialog(QWidget *parent = nullptr);

signals:
    void rewardSelected(int type); // 0:速度, 1:体型, 2:成长

private:
    void setupUI();
    QPushButton* createFancyButton(const QString& title, const QString& desc, const QString& iconPath, const QString& themeColor);
};

#endif // REWARDDIALOG_H