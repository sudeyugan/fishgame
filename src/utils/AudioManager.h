#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QString>
#include <QSoundEffect>

class AudioManager {
public:
    static void playBGM();
    static void playEatSound();
    static void playWinSound();
    static void playLoseSound();
};

#endif