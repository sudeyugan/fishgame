#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>

class GameOverDialog : public QDialog {
    Q_OBJECT
public:
    explicit GameOverDialog(bool win, int score, QWidget *parent = nullptr);

protected:
    // 重写绘制事件以实现自定义背景
    void paintEvent(QPaintEvent *event) override;

signals:
    void restartGame();
    void quitToTitle();
};

#endif // GAMEOVERDIALOG_H