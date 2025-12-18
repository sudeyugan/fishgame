#include "AudioManager.h"
#include <QUrl>
#include <QDebug>

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::setBGMVolume(float volume) {
    // 确保数值在 0.0 到 1.0 之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    if (m_bgmOutput) {
        m_bgmOutput->setVolume(volume);
    }
}

//设置所有短音效的音量
void AudioManager::setSFXVolume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    // 遍历目前加载的所有音效并设置音量
    for (auto effect : m_effects) {
        if (effect) {
            effect->setVolume(volume);
        }
    }
}

AudioManager::AudioManager(QObject* parent) : QObject(parent) {
    // 1. 初始化 BGM 播放器
    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmOutput = new QAudioOutput(this);
    m_bgmPlayer->setAudioOutput(m_bgmOutput);
    
    // 设置 BGM 音量 (0.0 - 1.0)
    m_bgmOutput->setVolume(0.5); 
    m_bgmPlayer->setLoops(QMediaPlayer::Infinite); // 无限循环

    // 2. 预加载所有短音效 (防止第一次播放卡顿)
    loadEffect("eat",  "qrc:/assets/sounds/eat.wav");
    loadEffect("win",  "qrc:/assets/sounds/win.wav");
    loadEffect("lose", "qrc:/assets/sounds/lose.wav");
    loadEffect("bubble", "qrc:/assets/sounds/bubble.wav");
}

AudioManager::~AudioManager() {
}

void AudioManager::loadEffect(const QString& name, const QString& path) {
    QSoundEffect* effect = new QSoundEffect(this);
    effect->setSource(QUrl(path));
    float vol = 1.0f;
    
    // 检查是否加载成功
    if (effect->status() == QSoundEffect::Error) {
        qDebug() << "Audio Load Error:" << path;
    }
    if (name == "eat") {
        vol = 0.4f; 
    } else if (name == "win") {
        vol = 0.4f; 
    }

    effect->setVolume(vol);
    
    m_effects.insert(name, effect);
}

void AudioManager::playBGM(const QString& name) {
    // 假设 BGM 路径是固定的
    QString path = QString("qrc:/assets/sounds/%1.wav").arg(name);

    // 如果已经在播放这首，就不重置
    if (m_bgmPlayer->source().toString() == path && m_bgmPlayer->playbackState() == QMediaPlayer::PlayingState) {
        return;
    }

    m_bgmPlayer->setSource(QUrl(path));
    m_bgmPlayer->play();
}

void AudioManager::playSound(const QString& name) {
    if (m_effects.contains(name)) {
        // 如果正在播放，停止并重新开始（实现快速连续吃鱼的声音）
        QSoundEffect* effect = m_effects[name];
        if (effect->isPlaying()) {
            effect->stop();
        }
        effect->play();
    } else {
        qDebug() << "Sound effect not found:" << name;
    }
}

void AudioManager::playWinSound() {
    m_bgmPlayer->stop(); // 胜利时停止 BGM
    playSound("win");
}

void AudioManager::playLoseSound() {
    m_bgmPlayer->stop(); // 失败时停止 BGM
    playSound("lose");
}