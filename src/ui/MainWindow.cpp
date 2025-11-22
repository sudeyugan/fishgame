#include "MainWindow.h"
#include "StartScreen.h"
#include "GameHud.h"
#include "PauseDialog.h"
#include "../scenes/MainScene.h"
#include "../utils/AudioManager.h"
#include "../core/GameEngine.h" // 确保包含 GameEngine
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>
// #include <QGLWidget>  <-- 删除这行，它导致了报错

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    // 1. 窗口基础设置
    setWindowTitle("深海大作战 - Deep Sea Survival");
    resize(1280, 720);
    setMinimumSize(800, 600);

    // 2. 初始化中央堆叠部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_stack = new QStackedWidget(m_centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(m_centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stack);

    // 3. 初始化各个模块
    initUI();
    
    // 播放背景音乐 (确保你有 assets/sounds/bgm.mp3)
    AudioManager::instance().playBGM("bgm");
}

void MainWindow::initUI() {
    // --- 界面1：开始菜单 ---
    m_startScreen = new StartScreen(this);
    connect(m_startScreen, &StartScreen::startGameClicked, this, &MainWindow::startGame);
    m_stack->addWidget(m_startScreen);

    // --- 界面2：游戏视图 ---
    m_gameView = new QGraphicsView(this);
    m_gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gameView->setRenderHint(QPainter::Antialiasing); 
    m_gameView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_gameView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_gameView->scale(1.0, 1.0);

    m_stack->addWidget(m_gameView);

    // --- 游戏场景 ---
    m_scene = new MainScene(this);
    m_gameView->setScene(m_scene);



    // --- HUD ---
    m_hud = new GameHud(m_gameView);
    m_hud->setGeometry(0, 0, width(), 100);
    m_hud->hide(); 

    // 连接信号
    connect(&GameEngine::instance(), &GameEngine::scoreChanged, m_hud, &GameHud::updateScore);
    connect(&GameEngine::instance(), &GameEngine::levelChanged, m_hud, &GameHud::updateLevel); // 确保连接 Level
    connect(&GameEngine::instance(), &GameEngine::gameOver, this, &MainWindow::handleGameOver);

    connect(m_scene, &MainScene::gamePaused, this, [this](bool isPaused){
        if (isPaused) {
            PauseDialog dialog(this);
            
            // 连接弹窗的“继续游戏”信号 -> 再次调用 pauseGame 来恢复游戏
            connect(&dialog, &PauseDialog::resumeGame, m_scene, &MainScene::pauseGame);
            
            // 连接弹窗的“返回标题”信号
            connect(&dialog, &PauseDialog::quitToTitle, this, [this](){
                // 先恢复游戏状态（防止计时器状态错乱），然后结束
                m_scene->pauseGame(); 
                handleGameOver(false); // 复用游戏结束逻辑返回标题
            });

            // 显示模态对话框
            dialog.exec();
        }
    });
}

void MainWindow::startGame() {
    m_stack->setCurrentWidget(m_gameView);
    m_hud->show();
    m_hud->resize(width(), 100);
    
    m_scene->startGame();
    m_gameView->setFocus(); 
}

void MainWindow::handleGameOver(bool win) {
    m_hud->hide();
    m_stack->setCurrentWidget(m_startScreen);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (m_hud) m_hud->resize(width(), 100);
}