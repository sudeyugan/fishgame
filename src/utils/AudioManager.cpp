#include "AudioManager.h"
#include <QUrl>
#include <QDebug>

// 静态成员变量需要在这里定义（虽然简单的静态方法可以不用，但建议如下实现）

void AudioManager::playBGM() { 
    // 既然安装了 Multimedia，请取消注释并确保路径正确
    static QSoundEffect *bgm = new QSoundEffect(); 
    // 注意：确保 resources.qrc 里真的有这个文件，否则不会响
    bgm->setSource(QUrl("qrc:/assets/sounds/bgmmp3v")); 
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(0.5f);
    bgm->play();
}

void AudioManager::playEatSound() { 
    static QSoundEffect *eat = new QSoundEffect();
    eat->setSource(QUrl("qrc:/assets/sounds/eat.mp3"));
    eat->play();
}

void AudioManager::playWinSound() {
    // 示例实现
    static QSoundEffect *win = new QSoundEffect();
    win->setSource(QUrl("qrc:/assets/sounds/win.mp3"));
    win->play();
}

void AudioManager::playLoseSound() {
    // 示例实现
    static QSoundEffect *lose = new QSoundEffect();
    lose->setSource(QUrl("qrc:/assets/sounds/lose.wav"));
    lose->play();
}