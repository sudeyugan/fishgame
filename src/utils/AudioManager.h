#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

class AudioManager : public QObject {
    Q_OBJECT
public:
    // 1. 单例访问点
    static AudioManager& instance();

    // 2. 成员函数 (注意：playBGM 接收参数以匹配 MainWindow 的调用)
    void playBGM(const QString& name);
    void playEatSound();
    void playWinSound();
    void playLoseSound();

private:
    // 3. 私有构造函数
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager();
    
    // 禁止拷贝
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    QMediaPlayer* m_bgmPlayer;
    QAudioOutput* m_bgmOutput;
};

#endif // AUDIOMANAGER_H