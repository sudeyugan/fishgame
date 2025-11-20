#include "MainWindow.h"
#include "StartScreen.h"
#include "GameHud.h"
#include "../scenes/MainScene.h"
#include "../utils/AudioManager.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QGLWidget> // 如果想开启硬件加速

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    // 1. 窗口基础设置
    setWindowTitle("深海大作战 - Deep Sea Survival");
    resize(1280, 720); // 设为高清分辨率
    setMinimumSize(800, 600);

    // 2. 初始化中央堆叠部件 (用于切换 菜单/游戏)
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_stack = new QStackedWidget(m_centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(m_centralWidget);
    layout->setContentsMargins(0, 0, 0, 0); // 去掉边距
    layout->addWidget(m_stack);

    // 3. 初始化各个模块
    initUI();
    
    // 播放背景音乐
    AudioManager::instance().playBGM("bgm");
}

void MainWindow::initUI() {
    // --- 界面1：开始菜单 ---
    m_startScreen = new StartScreen(this);
    connect(m_startScreen, &StartScreen::startGameClicked, this, &MainWindow::startGame);
    m_stack->addWidget(m_startScreen);

    // --- 界面2：游戏视图 ---
    // 使用自定义的 GraphicsView 配置
    m_gameView = new QGraphicsView(this);
    m_gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏滚动条
    m_gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gameView->setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    m_gameView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_gameView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    
    // 优化渲染：如果有 OpenGL 支持可以开启下面这行，否则注释掉
    // m_gameView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    m_stack->addWidget(m_gameView);

    // --- 游戏场景 ---
    m_scene = new MainScene(this);
    m_gameView->setScene(m_scene);

    // --- HUD (悬浮在游戏之上的 UI) ---
    // 注意：HUD 不加进 stack，而是作为 View 的子控件或者覆盖层
    m_hud = new GameHud(m_gameView);
    m_hud->setGeometry(0, 0, width(), 100); // 初始位置
    m_hud->hide(); // 菜单时不显示

    // 连接信号
    connect(&GameEngine::instance(), &GameEngine::scoreChanged, m_hud, &GameHud::updateScore);
    connect(&GameEngine::instance(), &GameEngine::gameOver, this, &MainWindow::handleGameOver);
}

void MainWindow::startGame() {
    m_stack->setCurrentWidget(m_gameView);
    m_hud->show();
    m_hud->resize(width(), 100); // 确保HUD宽度适配
    
    m_scene->startGame();
    m_gameView->setFocus(); // 夺取焦点，确保键盘能控制
}

void MainWindow::handleGameOver(bool win) {
    // 简单的处理：切回菜单 (实际项目中可以弹窗显示分数)
    m_hud->hide();
    m_stack->setCurrentWidget(m_startScreen);
    // 可以在这里重置 StartScreen 的显示文字
}

// 确保窗口大小改变时 HUD 跟着变
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (m_hud) m_hud->resize(width(), 100);
}