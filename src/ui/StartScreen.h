#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>


class StartScreen : public QWidget {
    Q_OBJECT
public:
    explicit StartScreen(QWidget *parent = nullptr);
    void checkSaveFile();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void startGameClicked();
    void helpClicked(); // 新增帮助信号
    void loadGameClicked();

private:
    QPushButton *m_btnContinue;
};

#endif // STARTSCREEN_H