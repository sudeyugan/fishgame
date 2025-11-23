#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>
#include <QJsonObject>

class Player;

class SaveManager {
public:
    // 保存游戏：需要传入主角对象以获取位置和大小，同时从 GameEngine 获取分数等
    static bool saveGame(const Player* player);
    
    // 读取游戏：读取文件并应用到主角和 GameEngine
    static bool loadGame(Player* player);
    
    // 检查是否存在存档文件
    static bool hasSaveFile();
};

#endif // SAVEMANAGER_H