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
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // 2. 初始化各个界面
    startScreen = new StartScreen(this);
    gameView = new QGraphicsView(this);
    gameScene = new MainScene(this);
    gameView->setScene(gameScene);
    
    // 3. HUD (悬浮在View之上)
    gameHud = new GameHud(this);
    gameHud->setGeometry(0, 0, width(), 50); // 放在顶部
    gameHud->hide(); // 只有游戏开始才显示

    // 添加到堆栈
    stackedWidget->addWidget(startScreen); // index 0
    stackedWidget->addWidget(gameView);    // index 1

    // 4. 连接信号槽
    connect(startScreen, &StartScreen::startClicked, this, &MainWindow::startGame);
    
    // 监听暂停信号
    connect(&GameEngine::instance(), &GameEngine::gamePaused, this, &MainWindow::handlePause);
}

void MainWindow::startGame() {
    stackedWidget->setCurrentWidget(gameView);
    gameHud->show();
    GameEngine::instance().startGame(); // 重置分数
    gameScene->startGame();             // 启动定时器
}

void MainWindow::handlePause(bool paused) {
    if (paused) {
        PauseDialog dlg(this);
        connect(&dlg, &PauseDialog::quitToTitle, this, &MainWindow::backToTitle);
        // 如果点击继续，dlg会自动关闭，我们可以恢复游戏
        connect(&dlg, &PauseDialog::resumeGame, [](){
            GameEngine::instance().pauseGame(); 
        });
        dlg.exec(); // 阻塞显示
    }
}

void MainWindow::backToTitle() {
    gameScene->pauseGame(); // 确保停止
    gameHud->hide();
    stackedWidget->setCurrentWidget(startScreen);
}