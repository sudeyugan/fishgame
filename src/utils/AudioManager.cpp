#include "AudioManager.h"
#include <QUrl>
#include <QDebug>

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
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
    // 注意：QSoundEffect 对 WAV 支持最好。MP3 在某些平台可能有延迟或不支持。
    loadEffect("eat",  ":/assets/sounds/eat.mp3");
    loadEffect("win",  ":/assets/sounds/win.mp3");
    loadEffect("lose", ":/assets/sounds/lose.wav");
}

AudioManager::~AudioManager() {
    // 清理指针，虽然 QObject 会自动清理子对象，但好习惯要有
}

void AudioManager::loadEffect(const QString& name, const QString& path) {
    QSoundEffect* effect = new QSoundEffect(this);
    effect->setSource(QUrl(path));
    effect->setVolume(1.0); // 音效最大声
    
    // 检查是否加载成功
    if (effect->status() == QSoundEffect::Error) {
        qDebug() << "Audio Load Error:" << path;
    }
    
    m_effects.insert(name, effect);
}

void AudioManager::playBGM(const QString& name) {
    // 假设 BGM 路径是固定的
    QString path = QString(":/assets/sounds/%1.mp3").arg(name);
    
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