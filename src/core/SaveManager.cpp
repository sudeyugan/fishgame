#include "SaveManager.h"
#include "GameEngine.h"
#include "../entities/Player.h"
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <QDebug>

// 辅助函数：获取文件名
QString getFilePath(int slotIndex) {
    return QString("save_slot_%1.json").arg(slotIndex);
}

bool SaveManager::saveGame(const Player* player, int slotIndex) {
    if (!player) return false;

    QJsonObject json;
    // 游戏状态
    json["score"] = GameEngine::instance().getScore();
    json["level"] = GameEngine::instance().getCurrentLevel();
    // 玩家状态
    json["playerX"] = player->x();
    json["playerY"] = player->y();
    json["playerScale"] = player->getSizeScale(); // 注意确保 Player 有 getSizeScale 或 scale()
    // 时间戳
    json["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");

    QFile file(getFilePath(slotIndex));
    if (!file.open(QIODevice::WriteOnly)) return false;

    file.write(QJsonDocument(json).toJson());
    return true;
}

bool SaveManager::loadGame(Player* player, int slotIndex) {
    if (!player) return false;

    QFile file(getFilePath(slotIndex));
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();

    // 恢复数据
    GameEngine::instance().startGame(); // 重置基础状态
    GameEngine::instance().addScore(json["score"].toInt());
    
    // 恢复等级
    int targetLevel = json["level"].toInt();
    // 简单的循环升级，或者直接由 GameEngine 提供 setLevel 接口
    // 这里使用 hack 方式：循环 nextLevel 直到目标
    while(GameEngine::instance().getCurrentLevel() < targetLevel) {
        GameEngine::instance().nextLevel();
    }

    player->setPos(json["playerX"].toDouble(), json["playerY"].toDouble());
    player->setSizeScale(json["playerScale"].toDouble());

    return true;
}

SaveSlotInfo SaveManager::getSlotInfo(int slotIndex) {
    SaveSlotInfo info;
    info.slotIndex = slotIndex;
    info.isEmpty = true;

    QFile file(getFilePath(slotIndex));
    if (file.open(QIODevice::ReadOnly)) {
        QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
        if (json.contains("score")) {
            info.isEmpty = false;
            info.score = json["score"].toInt();
            info.level = json["level"].toInt();
            info.timestamp = json["timestamp"].toString();
        }
    }
    return info;
}

QList<SaveSlotInfo> SaveManager::getAllSlots(int maxSlots) {
    QList<SaveSlotInfo> list;
    for (int i = 0; i < maxSlots; i++) {
        list.append(getSlotInfo(i));
    }
    return list;
}