#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include <QMouseEvent> // 引入鼠标事件头文件

class StartScreen : public QWidget {
    Q_OBJECT
public:
    explicit StartScreen(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    
    // [新增] 重写鼠标按下事件
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void startGameClicked();
    void quitGameClicked();
};

#endif // STARTSCREEN_H