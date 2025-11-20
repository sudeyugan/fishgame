#include "MainWindow.h"
#include "StartScreen.h"
#include "GameHud.h"
#include "PauseDialog.h"
#include "../scenes/MainScene.h"
#include "../core/GameEngine.h"
#include "../core/Constants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    
    // 1. 堆栈窗口管理场景
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // 2. 初始化各个界面
    m_startScreen = new StartScreen(this);
    m_gameView = new QGraphicsView(this);
    m_gameScene = new MainScene(this);
    m_gameView->setScene(m_gameScene);
    
    // 优化视图渲染设置
    m_gameView->setRenderHint(QPainter::Antialiasing);
    m_gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 3. HUD (悬浮在View之上)
    m_gameHud = new GameHud(this);
    m_gameHud->setGeometry(0, 0, width(), 50); // 放在顶部
    m_gameHud->hide(); // 只有游戏开始才显示

    // 添加到堆栈
    m_stackedWidget->addWidget(m_startScreen); // index 0
    m_stackedWidget->addWidget(m_gameView);    // index 1

    // 4. 连接信号槽
    // 注意：这里要用头文件里定义的函数名 onStartGameClicked
    connect(m_startScreen, &StartScreen::startClicked, this, &MainWindow::onStartGameClicked);
    
    // 监听暂停信号
    connect(&GameEngine::instance(), &GameEngine::gamePaused, this, &MainWindow::onGamePaused);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // 传递按键事件给 MainScene (特别是 ESC 键)
    if (m_stackedWidget->currentWidget() == m_gameView) {
         // 可以在这里处理全局快捷键
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onStartGameClicked() {
    m_stackedWidget->setCurrentWidget(m_gameView);
    m_gameHud->show();
    GameEngine::instance().startGame(); // 重置分数
    m_gameScene->startGame();             // 启动定时器
}

void MainWindow::onGamePaused(bool paused) {
    if (paused) {
        PauseDialog dlg(this);
        connect(&dlg, &PauseDialog::quitToTitle, this, &MainWindow::onBackToTitle);
        // 如果点击继续，dlg会自动关闭，我们可以恢复游戏
        connect(&dlg, &PauseDialog::resumeGame, [](){
            GameEngine::instance().pauseGame(); 
        });
        dlg.exec(); // 阻塞显示
    }
}

void MainWindow::onBackToTitle() {
    m_gameScene->pauseGame(); // 确保停止
    m_gameHud->hide();
    m_stackedWidget->setCurrentWidget(m_startScreen);
}