#include "MainWindow.h"
#include "StartScreen.h"
#include "GameHud.h"
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
    
    // 删除导致报错的硬件加速代码
    // m_gameView->setViewport(new QGLWidget(...)); 

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
    if (m_gameView) m_gameView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding); // 可选：自适应
}