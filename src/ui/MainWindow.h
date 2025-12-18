#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>

// 前向声明，避免循环引用
class MainScene;
class GameHud;
class StartScreen;
class HelpScreen;

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
    void handleLevelChange(int level);
    void onLevelCompleted();

private:
    void initUI();

    QWidget *m_centralWidget;
    QStackedWidget *m_stack;  // 用于在 菜单/游戏 之间切换

    StartScreen *m_startScreen;
    HelpScreen *m_helpScreen;
    QGraphicsView *m_gameView;
    MainScene *m_scene;
    GameHud *m_hud;

    bool m_isSwitchingLevel; // 是否正在切换关卡
    bool m_isLoadingGame;    // 是否正在读取存档
};

#endif // MAINWINDOW_H