#include "AudioManager.h"
// 如果没有音频文件，这些函数留空即可防止报错
void AudioManager::playBGM() { 
    // QSoundEffect *bgm = new QSoundEffect; 
    // bgm->setSource(QUrl::fromLocalFile(":/assets/sounds/bgm.wav"));
    // bgm->setLoopCount(QSoundEffect::Infinite);
    // bgm->play();
}
void AudioManager::playEatSound() { /* 播放吃东西音效 */ }
void AudioManager::playWinSound() { /* 播放胜利音效 */ }
void AudioManager::playLoseSound() { /* 播放失败音效 */ }