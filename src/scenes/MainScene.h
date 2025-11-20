// 在 checkCollisions 中：
if (eatSuccess) {
    GameEngine::instance().addScore(10); // 使用单例加分
    AudioManager::playEatSound();        // 播放音效
    
    // 检查通关
    LevelData currentLevel = LevelManager::getLevelData(GameEngine::instance().getCurrentLevel());
    if (GameEngine::instance().getScore() >= currentLevel.targetScore) {
        GameEngine::instance().nextLevel();
        // 这里可以触发加载下一关的逻辑
    }
}