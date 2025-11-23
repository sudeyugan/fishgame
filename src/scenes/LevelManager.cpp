#include "LevelManager.h"

LevelData LevelManager::getLevelData(int level) {
    LevelData data;
    
    data.bgImage = ":/assets/images/background1.jpg"; 

    // 根据等级动态计算参数，而不是写死 if-else
    // 1. 目标分数：第一关50，每升一级增加 50 分 (50, 100, 150...)
    data.targetScore = 50 + (level - 1) * 50;

    // 2. 敌人生成速度：初始 2000ms，每级减少 200ms，最快 1000ms
    int spawnRate = 2000 - (level - 1) * 200;
    if (spawnRate < 1000) spawnRate = 1000;
    data.enemySpawnRate = spawnRate;

    // 3. 关卡描述
    data.description = QString("第 %1 关\n目标：%2 分\n难度提升：敌人更敏锐了！").arg(level).arg(data.targetScore);

    return data;
}