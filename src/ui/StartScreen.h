#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include <QPushButton>

class StartScreen : public QWidget {
    Q_OBJECT
public:
    explicit StartScreen(QWidget *parent = nullptr);
    void checkSaveFile(); 

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void helpClicked();
    void loadGameClicked();  // 点击“进入游戏”触发此信号

private:
    QPushButton *m_btnContinue; // 主按钮（进入游戏）
    QPushButton *m_btnHelp;     // 帮助按钮
};

#endif // STARTSCREEN_H