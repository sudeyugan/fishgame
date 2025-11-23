#include "MainWindow.h"
#include "HelpScreen.h"
#include "StartScreen.h"
#include "GameOverDialog.h"
#include "GameHud.h"
#include "PauseDialog.h"
#include "../scenes/MainScene.h"
#include "../utils/AudioManager.h"
#include "../core/GameEngine.h" 
#include "SaveLoadDialog.h"
#include "../core/SaveManager.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGraphicsView>

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
    m_startScreen = new StartScreen(this);
    m_stack->addWidget(m_startScreen);

    // 1. 处理“进入游戏”按钮 (对应 loadGameClicked 信号)
    connect(m_startScreen, &StartScreen::loadGameClicked, this, [this](){
        // 弹出存档选择框
        SaveLoadDialog dialog(SaveLoadDialog::LOAD, this);
        
        connect(&dialog, &SaveLoadDialog::slotSelected, this, [this](int slot, bool isNewGame){
            if (isNewGame) {
                // 新建存档逻辑
                GameEngine::instance().startGame(); 
                this->startGame(); 
                SaveManager::saveGame(m_scene->getPlayer(), slot); 
            } else {
                // 读取存档逻辑
                this->startGame(); 
                SaveManager::loadGame(m_scene->getPlayer(), slot); 
            }
        });
        
        dialog.exec();
    });

    // 2. 处理帮助按钮
    m_helpScreen = new HelpScreen(this);
    m_stack->addWidget(m_helpScreen);

    connect(m_startScreen, &StartScreen::helpClicked, this, [this](){
        m_stack->setCurrentWidget(m_helpScreen);
    });
    
    connect(m_helpScreen, &HelpScreen::backClicked, this, [this](){
        m_stack->setCurrentWidget(m_startScreen);
    });

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
            
            // 1. 继续游戏
            connect(&dialog, &PauseDialog::resumeGame, m_scene, &MainScene::pauseGame);
            
            connect(&dialog, &PauseDialog::saveGame, this, [this](){
                // 点击暂停菜单的"保存" -> 弹出多存档选择窗口
                SaveLoadDialog saveDialog(SaveLoadDialog::SAVE, this);
                
                connect(&saveDialog, &SaveLoadDialog::slotSelected, this, [this](int slot, bool){
                    // 用户选定槽位后 -> 执行保存
                    if (m_scene && m_scene->getPlayer()) {
                        SaveManager::saveGame(m_scene->getPlayer(), slot);
                    }
                });
                
                saveDialog.exec();
            });
            // ------------------

            // 3. 处理 ESC 关闭弹窗 (一定要加，否则按ESC卡死)
            connect(&dialog, &QDialog::rejected, m_scene, &MainScene::pauseGame);

            // 4. 返回标题
            connect(&dialog, &PauseDialog::quitToTitle, this, [this](){
                m_scene->pauseGame(); 
                m_hud->hide();
                // 如果 StartScreen 有 checkSaveFile，可以调用一下刷新界面
                if(m_startScreen) m_startScreen->checkSaveFile();
                m_stack->setCurrentWidget(m_startScreen);
                AudioManager::instance().playBGM("bgm");
            });

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
    // 1. 播放对应的音效
    if (win) {
        AudioManager::instance().playWinSound();
    } else {
        AudioManager::instance().playLoseSound();
    }

    // 2. 创建并显示结算弹窗
    // 传入当前的胜负状态和 GameEngine 中的分数
    GameOverDialog dialog(win, GameEngine::instance().getScore(), this);
    
    // 连接信号：重新开始
    connect(&dialog, &GameOverDialog::restartGame, this, [this](){
        GameEngine::instance().startGame();
        // startGame() 会负责重置场景和 GameEngine 状态
        this->startGame(); 
    });
    
    // 连接信号：返回标题
    connect(&dialog, &GameOverDialog::quitToTitle, this, [this](){
        m_hud->hide();
        m_stack->setCurrentWidget(m_startScreen);
        
        // 重新播放 BGM (因为游戏结束可能会打断 BGM)
        AudioManager::instance().playBGM("bgm"); 
    });

    // 3. 模态显示 (阻塞直到用户点击按钮)
    dialog.exec();
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (m_hud) m_hud->resize(width(), 100);
}