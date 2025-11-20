#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>

// 前向声明，避免循环引用
class MainScene;
class GameHud;
class StartScreen;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    // 重写 resizeEvent 以便让 HUD 自适应窗口大小
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void startGame();         // 切换到游戏界面
    void handleGameOver(bool win); // 处理游戏结束

private:
    void initUI();

    QWidget *m_centralWidget;
    QStackedWidget *m_stack;  // 用于在 菜单/游戏 之间切换

    StartScreen *m_startScreen;
    QGraphicsView *m_gameView;
    MainScene *m_scene;
    GameHud *m_hud;
};

#endif // MAINWINDOW_H