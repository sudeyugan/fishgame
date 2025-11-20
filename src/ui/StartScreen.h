#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include <QPushButton>

class StartScreen : public QWidget {
    Q_OBJECT
public:
    explicit StartScreen(QWidget *parent = nullptr);

protected:
    // 重写绘图事件，绘制海洋渐变背景
    void paintEvent(QPaintEvent *event) override;

signals:
    void startGameClicked(); // 点击开始时发出
    void quitGameClicked();  // 点击退出时发出

private:
    QPushButton *m_startBtn;
    QPushButton *m_quitBtn;
};

#endif // STARTSCREEN_H