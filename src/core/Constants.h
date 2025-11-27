#ifndef CONSTANTS_H
#define CONSTANTS_H

enum class CharacterType {
    Normal, // Fish1: 普通
    Agile   // Fish2: 敏捷 (新角色)
};

namespace Config {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const int FPS = 60;
    const int REFRESH_RATE = 1000 / FPS;
}

const int SKILL_DURATION = 3000; // 技能持续时间 (毫秒)
const int SKILL_COOLDOWN = 10000; // 技能冷却时间 (毫秒)

#endif // CONSTANTS_H