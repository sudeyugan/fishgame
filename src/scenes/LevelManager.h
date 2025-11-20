#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QString>
#include <QMap>

struct LevelData {
    int targetScore;      // 通关分数
    QString bgImage;      // 背景图路径
    int enemySpawnRate;   // 敌人生成速度(ms)
    QString description;  // 关卡说明
};

class LevelManager {
public:
    static LevelData getLevelData(int level);
};

#endif