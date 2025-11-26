#include "MainWindow.h"
#include "HelpScreen.h"
#include "StartScreen.h"
#include "GameOverDialog.h"
#include "GameHud.h"
#include "PauseDialog.h"
#include "LevelInfoDialog.h"
#include "../scenes/MainScene.h"
#include "../utils/AudioManager.h"
#include "../core/GameEngine.h" 
#include "BackgroundSelectDialog.h"
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

    m_isSwitchingLevel = false;
    m_isLoadingGame = false;
}

void MainWindow::initUI() {
    m_startScreen = new StartScreen(this);
    m_stack->addWidget(m_startScreen);

    // 1. 处理“进入游戏”按钮 (对应 loadGameClicked 信号)
    connect(m_startScreen, &StartScreen::loadGameClicked, this, [this](){
        // 弹出存档选择框
        SaveLoadDialog dialog(SaveLoadDialog::LOAD, this);
        
        connect(&dialog, &SaveLoadDialog::slotSelected, this, [this](int slot, bool isNewGame){
            this->startGame();
            if (isNewGame) {
                // --- 新建存档流程 ---
                
                // 1. 弹出背景选择窗口
                BackgroundSelectDialog bgDlg(this);
                if (bgDlg.exec() == QDialog::Accepted) {
                    // 2. 将用户选择存入 GameEngine
                    GameEngine::instance().setBackgroundIndex(bgDlg.getSelectedIndex());
                    
                    // 3. 开始游戏初始化 (这会触发 LevelManager 读取刚才存的背景)
                    GameEngine::instance().startGame(); 
                    
                    // 4. 立即保存初始存档 (包含背景信息)
                    if (m_scene && m_scene->getPlayer()) {
                        SaveManager::saveGame(m_scene->getPlayer(), slot); 
                    }
                } else {
                    // 如果用户关掉了背景选择窗，可以选择返回或者默认开始
                    // 这里我们默认直接开始(默认为背景1)
                    GameEngine::instance().setBackgroundIndex(1);
                    GameEngine::instance().startGame();
                    SaveManager::saveGame(m_scene->getPlayer(), slot);
                }

            } else {
                // --- 读取存档流程 (保持不变) ---
                m_isLoadingGame = true; 
                SaveManager::loadGame(m_scene->getPlayer(), slot); 
                m_isLoadingGame = false; 
                handleLevelChange(GameEngine::instance().getCurrentLevel());
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
    connect(&GameEngine::instance(), &GameEngine::levelChanged, this, &MainWindow::handleLevelChange, Qt::QueuedConnection);

    // 连接分数变化 (HUD 还是直接更新)
    connect(&GameEngine::instance(), &GameEngine::scoreChanged, m_hud, &GameHud::updateScore);
    
    // 连接游戏结束
    connect(&GameEngine::instance(), &GameEngine::gameOver, this, &MainWindow::handleGameOver);
    connect(m_scene, &MainScene::gamePaused, this, [this](bool isPaused){
        if (isPaused && !m_isSwitchingLevel) {
            PauseDialog dialog(this);
            
            // 1. 继续游戏
            connect(&dialog, &PauseDialog::resumeGame, m_scene, &MainScene::pauseGame);
            
            // 2. 保存游戏 
            connect(&dialog, &PauseDialog::saveGame, this, [this](){
                // 创建存档选择窗口
                SaveLoadDialog saveDialog(SaveLoadDialog::SAVE, this);
                
                // 监听用户选择的槽位
                connect(&saveDialog, &SaveLoadDialog::slotSelected, this, [this](int slot, bool){
                    
                    bool success = false;
                    // 执行保存
                    if (m_scene && m_scene->getPlayer()) {
                         success = SaveManager::saveGame(m_scene->getPlayer(), slot);
                    }

                    // [关键] 根据保存结果，弹出美化后的提示框
                    if (success) {
                        SaveLoadDialog::showMessageBox(this, "系统提示", "游戏进度已成功保存！");
                    } else {
                        SaveLoadDialog::showMessageBox(this, "保存失败", "写入存档时发生错误，请重试。");
                    }
                });
                
                // 显示存档窗口
                saveDialog.exec();
            });

            // 3. 处理 ESC 关闭 (保持不变)
            connect(&dialog, &QDialog::rejected, m_scene, &MainScene::pauseGame);

            // 4. 返回标题 (保持不变)
            connect(&dialog, &PauseDialog::quitToTitle, this, [this](){
                // ... 原有逻辑 ...
                m_scene->pauseGame(); 
                m_hud->hide();
                if(m_startScreen) m_startScreen->checkSaveFile(); // 记得刷新标题画面的存档检测
                m_stack->setCurrentWidget(m_startScreen);
                AudioManager::instance().playBGM("bgm");
            });

            dialog.exec();
        }
    });
}

void MainWindow::handleLevelChange(int level) {
    // 如果正在读档（连续升级中），直接忽略，等读档完了手动调一次
    if (m_isLoadingGame) return;

    GameEngine::instance().setScore(0);

    //标记开始切关卡，抑制 PauseDialog
    m_isSwitchingLevel = true; 

    // 1. 暂停游戏逻辑
    if (m_scene) {
        m_scene->setPaused(true); 
    }

    // 2. 获取数据 & 更新 HUD
    LevelData data = LevelManager::getLevelData(level);
    m_hud->updateLevel(level);
    m_hud->updateTarget(data.targetScore);
    m_hud->updateScore(GameEngine::instance().getScore());

    // 3. 弹出关卡说明窗口
    LevelInfoDialog dialog(level, data.description, this);
    dialog.exec(); // 阻塞等待用户点击

    // 4. 加载关卡 & 恢复游戏
    if (m_scene) {
        m_scene->loadLevel(data);
        
        if (m_stack->currentWidget() != m_gameView) {
            m_stack->setCurrentWidget(m_gameView);
            m_hud->show();
        }
        
        m_scene->setPaused(false); // 恢复定时器
        m_gameView->setFocus();
    }
    
    // 切关卡结束，恢复正常状态
    m_isSwitchingLevel = false; 
}

void MainWindow::startGame() {
    // 1. 切换到游戏画面
    m_stack->setCurrentWidget(m_gameView);
    
    // 2. 显示并重置 HUD 尺寸
    m_hud->show();
    m_hud->resize(width(), 100);
    
    // 3. 让视图获取焦点（以便键盘控制）
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