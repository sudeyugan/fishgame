#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>

// 前置声明，减少头文件依赖
class StartScreen;
class MainScene;
class GameHud;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    // 监听全局按键（如ESC暂停）
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onStartGameClicked(); // 点击开始游戏
    void onBackToTitle();      // 从暂停/结束返回标题
    void onGamePaused(bool paused); // 处理暂停信号

private:
    QStackedWidget *m_stackedWidget;
    
    StartScreen *m_startScreen;
    QGraphicsView *m_gameView;
    MainScene *m_gameScene;
    GameHud *m_gameHud;

    void initUI();
};

#endif // MAINWINDOW_H