#include "AudioManager.h"
#include <QUrl>
// #include <QSoundEffect> // 暂时不用，改用 QMediaPlayer 以获得更好的 MP3 兼容性

AudioManager& AudioManager::instance() {
    static AudioManager _instance;
    return _instance;
}

AudioManager::AudioManager(QObject *parent) : QObject(parent) {
    // 初始化背景音乐播放器
    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmOutput = new QAudioOutput(this);
    m_bgmPlayer->setAudioOutput(m_bgmOutput);
    m_bgmOutput->setVolume(0.3); // 设置默认音量
}

AudioManager::~AudioManager() {
}

void AudioManager::playBGM(const QString& name) {
    // 假设资源路径是 :/assets/sounds/bgm.mp3
    QString path = QString("qrc:/assets/sounds/%1.mp3").arg(name);
    
    m_bgmPlayer->setSource(QUrl(path));
    m_bgmPlayer->setLoops(QMediaPlayer::Infinite); // 循环播放
    m_bgmPlayer->play();
}

// 辅助函数：创建一个临时的播放器播放一次性音效
void playOneShot(const QString& path, float volume) {
    auto* player = new QMediaPlayer;
    auto* output = new QAudioOutput;
    player->setAudioOutput(output);
    
    player->setSource(QUrl(path));
    output->setVolume(volume);
    
    // 播放结束或出错时自动清理内存
    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, [player, output](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia || status == QMediaPlayer::InvalidMedia) {
            player->deleteLater();
            output->deleteLater();
        }
    });
    
    player->play();
}

void AudioManager::playEatSound() {
    // 使用 QMediaPlayer 替代 QSoundEffect 以避免 MP3 解码警告
    playOneShot("qrc:/assets/sounds/eat.mp3", 0.8);
}

void AudioManager::playWinSound() {
    playOneShot("qrc:/assets/sounds/win.mp3", 1.0);
}

void AudioManager::playLoseSound() {
    // 注意：这里是 wav 文件
    playOneShot("qrc:/assets/sounds/lose.wav", 1.0);
}