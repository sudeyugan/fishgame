#ifndef HELPSCREEN_H
#define HELPSCREEN_H

#include <QWidget>

class HelpScreen : public QWidget {
    Q_OBJECT
public:
    explicit HelpScreen(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void backClicked(); // 点击返回按钮时触发
};

#endif // HELPSCREEN_H