#include "LevelManager.h"

LevelData LevelManager::getLevelData(int level) {
    LevelData data;
    if (level == 1) {
        data.targetScore = 50;
        data.bgImage = ":/assets/images/bg_level1.png"; // 确保你有这些图，或者用纯色代替
        data.enemySpawnRate = 2000;
        data.description = "第一关：初入海洋\n目标：获得50分\n提示：躲避大鱼，吃掉小鱼";
    } else if (level == 2) {
        data.targetScore = 150;
        data.bgImage = ":/assets/images/bg_level2.png";
        data.enemySpawnRate = 1500;
        data.description = "第二关：深海危机\n目标：获得150分\n提示：敌人速度变快了！";
    } else {
        // 默认/无限模式
        data.targetScore = 9999;
        data.bgImage = ":/assets/images/bg_level3.png";
        data.enemySpawnRate = 1000;
        data.description = "无尽模式\n目标：活下去！";
    }
    return data;
}