#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QList>

class Player;

struct SaveSlotInfo {
    int slotIndex;      // 槽位编号 (0, 1, 2...)
    bool isEmpty;       // 是否为空
    int level;          // 关卡
    int score;          // 分数
    QString timestamp;  // 存档时间字符串
};

class SaveManager {
public:
    // 保存到指定槽位
    static bool saveGame(const Player* player, int slotIndex);
    
    // 从指定槽位读取
    static bool loadGame(Player* player, int slotIndex);
    
    // 获取指定槽位的信息（用于UI显示）
    static SaveSlotInfo getSlotInfo(int slotIndex);
    
    // 获取所有槽位信息
    static QList<SaveSlotInfo> getAllSlots(int maxSlots = 3);
};

#endif // SAVEMANAGER_H