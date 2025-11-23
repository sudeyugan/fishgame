#include "SaveManager.h"
#include "GameEngine.h"
#include "../entities/Player.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QDir>

// 存档文件路径
const QString SAVE_FILE = "savegame.json";

bool SaveManager::saveGame(const Player* player) {
    if (!player) return false;

    QJsonObject json;
    
    // 1. 保存全局游戏状态
    json["score"] = GameEngine::instance().getScore();
    json["level"] = GameEngine::instance().getCurrentLevel();
    
    // 2. 保存主角状态
    json["playerX"] = player->x();
    json["playerY"] = player->y();
    json["playerScale"] = player->scale(); // 假设使用了 setScale 来控制大小

    // 3. 写入文件
    QFile file(SAVE_FILE);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open save file for writing";
        return false;
    }

    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());
    return true;
}

bool SaveManager::loadGame(Player* player) {
    if (!player) return false;

    QFile file(SAVE_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    // 1. 恢复全局游戏状态
    // 注意：需要在 GameEngine 中添加 loadState 方法，或者先 reset 再 addScore
    GameEngine::instance().resetGame(); 
    GameEngine::instance().addScore(json["score"].toInt());
    // 简单的等级恢复逻辑：
    int targetLevel = json["level"].toInt();
    while(GameEngine::instance().getCurrentLevel() < targetLevel) {
        GameEngine::instance().nextLevel();
    }

    // 2. 恢复主角状态
    player->setPos(json["playerX"].toDouble(), json["playerY"].toDouble());
    player->setScale(json["playerScale"].toDouble());

    return true;
}

bool SaveManager::hasSaveFile() {
    return QFile::exists(SAVE_FILE);
}