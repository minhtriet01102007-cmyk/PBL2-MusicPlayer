#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QString>

class Player : public QObject {
    Q_OBJECT
public:
    explicit Player(QObject* parent = nullptr) : QObject(parent) {
        out = new QAudioOutput(this);
        mp  = new QMediaPlayer(this);
        mp->setAudioOutput(out);
        out->setVolume(0.7f);
        connect(mp, &QMediaPlayer::positionChanged, this, &Player::positionChanged);
        connect(mp, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
        connect(mp, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState st) {
            emit playingChanged(st == QMediaPlayer::PlayingState);
        });
        connect(mp, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus s) {
            if (s == QMediaPlayer::EndOfMedia) emit finished();
        });
    }
    void play(const QString& file) { mp->setSource(QUrl::fromLocalFile(file)); mp->play(); }
    void togglePause() {
        if (mp->playbackState() == QMediaPlayer::PlayingState) mp->pause(); else mp->play();
    }
    bool isPlaying() const { return mp->playbackState() == QMediaPlayer::PlayingState; }
    void stop() { mp->stop(); }
    void seek(qint64 ms) { mp->setPosition(ms); }
    void setVolume(int percent) { out->setVolume(percent / 100.0f); }

signals:
    void positionChanged(qint64 ms);
    void durationChanged(qint64 ms);
    void finished();
    void playingChanged(bool playing);

private:
    QMediaPlayer* mp;
    QAudioOutput* out;
};
