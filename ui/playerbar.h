
#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <QAudioOutput>
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QPainter>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include "bll/musicsystem.h"
#include "icon.h"
#include "theme.h"
#include "widgets.h"

// Thanh phát nhạc ở cuối cửa sổ
// Âm thanh thật do QMediaPlayer phát.
// Bài chưa có file nhạc sẽ được phát mô phỏng.
// PlayerBar chỉ phát bài mà PlayerService chọn.
class PlayerBar : public QWidget{
    private:
        MusicSystem& ms;
        QMediaPlayer* player;
        QAudioOutput* audio;
        CoverWidget* cover;
        LinkLabel* titleLabel;
        LinkLabel* artistLabel;
        IconButton* heartBtn;
        IconButton* shuffleBtn;
        IconButton* prevBtn;
        IconButton* playBtn;
        IconButton* nextBtn;
        IconButton* repeatBtn;
        QLabel* posLabel;
        QLabel* durLabel;
        ClickSlider* seek;
        IconButton* queueBtn;
        IconButton* volBtn;
        ClickSlider* volume;
        QTimer* simTimer;

        SongInfo current;
        bool hasTrack;
        bool playing;
        bool counted;
        bool simulated;
        qint64 simPos;

        void setPlaying(bool on);
        void showPosition(qint64 ms);
        qint64 position() const;
        void seekTo(qint64 ms);
        void finishTrack();
        void startSimulation();
        void maybeCount(qint64 ms);
        void updateVolumeIcon();
        void resetIdleUi();

    protected:
        void paintEvent(QPaintEvent* e) override;

    public:
        std::function<void()> onTrackChanged;
        std::function<void()> onQueueToggle;
        std::function<void(const std::string&)> onOpenArtist;
        std::function<void(const std::string&)> onOpenAlbum;
        std::function<void(const QString&, bool)> onMessage;
        std::function<void(const std::string&, bool)> onLikeToggled;

        PlayerBar(MusicSystem& system, QWidget* parent = nullptr);

        void playTrack(const SongInfo& info);
        void togglePlay();
        void next();
        void previous();

        void toggleShuffle();
        void setShuffle(bool on);  // Bật/tắt Shuffle và đồng bộ giao diện

        void cycleRepeat();
        void clear();

        bool hasCurrent() const;
        bool isPlaying() const;
        std::string currentId() const;

        void syncLiked(const std::string& idSong, bool liked);
        void setQueueActive(bool on);
};

PlayerBar::PlayerBar(MusicSystem& system, QWidget* parent)
    : QWidget(parent), ms(system){

    this->hasTrack = false;
    this->playing = false;
    this->counted = false;
    this->simulated = false;
    this->simPos = 0;
    this->setFixedHeight(94);

    this->player = new QMediaPlayer(this);
    this->audio = new QAudioOutput(this);
    this->player->setAudioOutput(this->audio);
    this->audio->setVolume(0.7f);

    this->simTimer = new QTimer(this);
    this->simTimer->setInterval(200);

    QHBoxLayout* root = new QHBoxLayout(this);
    root->setContentsMargins(16, 8, 16, 8);
    root->setSpacing(12);

    // --- Trái: bìa + tên bài + tim ---
    QWidget* left = new QWidget(this);
    QHBoxLayout* ll = new QHBoxLayout(left);
    ll->setContentsMargins(0, 0, 0, 0);
    ll->setSpacing(12);

    this->cover = new CoverWidget(58, 6, left);
    ll->addWidget(this->cover);

    QVBoxLayout* titles = new QVBoxLayout;
    titles->setSpacing(2);
    titles->addStretch();

    this->titleLabel = new LinkLabel(left);
    this->titleLabel->setStyleSheet(
        "color:#ffffff; font-size:14px; font-weight:700;"
    );

    this->artistLabel = new LinkLabel(left);
    this->artistLabel->setStyleSheet(
        "color:#b3b3b3; font-size:12px;"
    );

    titles->addWidget(this->titleLabel);
    titles->addWidget(this->artistLabel);
    titles->addStretch();

    ll->addLayout(titles, 1);

    this->heartBtn = new IconButton(Icon::Heart, 18, left);
    this->heartBtn->setCheckable(true);
    this->heartBtn->setColors(
        theme::kSubText,
        theme::kText,
        theme::kAccent
    );

    ll->addWidget(this->heartBtn);
    root->addWidget(left, 3);

    // --- Giữa: nút điều khiển + thanh tua ---
    QWidget* center = new QWidget(this);
    QVBoxLayout* cl = new QVBoxLayout(center);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(4);

    QHBoxLayout* controls = new QHBoxLayout;
    controls->setSpacing(10);

    this->shuffleBtn = new IconButton(Icon::Shuffle, 20, center);
    this->shuffleBtn->setCheckable(true);
    this->shuffleBtn->setToolTip("Phat ngau nhien");

    this->prevBtn = new IconButton(Icon::Prev, 20, center);
    this->prevBtn->setColors(
        theme::kText,
        theme::kText,
        theme::kText
    );
    this->prevBtn->setToolTip("Bai truoc (Ctrl+←)");

    this->playBtn = new IconButton(Icon::Play, 18, center);
    this->playBtn->setColors(
        QColor(0, 0, 0),
        QColor(0, 0, 0),
        QColor(0, 0, 0)
    );
    this->playBtn->setCircle(QColor(255, 255, 255), 38);
    this->playBtn->setToolTip("Phat / Tam dung (Space)");

    this->nextBtn = new IconButton(Icon::Next, 20, center);
    this->nextBtn->setColors(
        theme::kText,
        theme::kText,
        theme::kText
    );
    this->nextBtn->setToolTip("Bai tiep (Ctrl+→)");

    this->repeatBtn = new IconButton(Icon::Repeat, 20, center);
    this->repeatBtn->setCheckable(true);
    this->repeatBtn->setToolTip("Lap lai");

    controls->addStretch();
    controls->addWidget(this->shuffleBtn);
    controls->addWidget(this->prevBtn);
    controls->addWidget(this->playBtn);
    controls->addWidget(this->nextBtn);
    controls->addWidget(this->repeatBtn);
    controls->addStretch();

    cl->addLayout(controls);

    QHBoxLayout* seekRow = new QHBoxLayout;
    seekRow->setSpacing(8);

    this->posLabel = makeLabel(
        "0:00", 12, false, theme::kSubText, center
    );
    this->posLabel->setFixedWidth(44);
    this->posLabel->setAlignment(
        Qt::AlignRight | Qt::AlignVCenter
    );

    this->seek = new ClickSlider(Qt::Horizontal, center);
    this->seek->setRange(0, 0);

    this->durLabel = makeLabel(
        "0:00", 12, false, theme::kSubText, center
    );
    this->durLabel->setFixedWidth(44);

    seekRow->addWidget(this->posLabel);
    seekRow->addWidget(this->seek, 1);
    seekRow->addWidget(this->durLabel);

    cl->addLayout(seekRow);
    root->addWidget(center, 4);

    // --- Phải: hàng chờ + âm lượng ---
    QWidget* right = new QWidget(this);
    QHBoxLayout* rl = new QHBoxLayout(right);
    rl->setContentsMargins(0, 0, 0, 0);
    rl->setSpacing(6);

    this->queueBtn = new IconButton(Icon::Queue, 20, right);
    this->queueBtn->setCheckable(true);
    this->queueBtn->setToolTip("Danh sach cho");

    this->volBtn = new IconButton(Icon::Volume, 20, right);
    this->volBtn->setToolTip("Tat / bat tieng");

    this->volume = new ClickSlider(Qt::Horizontal, right);
    this->volume->setRange(0, 100);
    this->volume->setValue(70);
    this->volume->setFixedWidth(110);

    rl->addStretch();
    rl->addWidget(this->queueBtn);
    rl->addWidget(this->volBtn);
    rl->addWidget(this->volume);

    root->addWidget(right, 3);

    // --- Kết nối ---

    QObject::connect(
        this->player,
        &QMediaPlayer::positionChanged,
        this,
        [this](qint64 pos){
            if (this->simulated) return;

            this->showPosition(pos);
            this->maybeCount(pos);
        }
    );

    QObject::connect(
        this->player,
        &QMediaPlayer::durationChanged,
        this,
        [this](qint64 d){
            if (this->simulated || d <= 0) return;

            this->seek->setRange(0, (int)d);
            this->durLabel->setText(
                theme::formatTime((int)(d / 1000))
            );
        }
    );

    QObject::connect(
        this->player,
        &QMediaPlayer::mediaStatusChanged,
        this,
        [this](QMediaPlayer::MediaStatus st){
            if (this->simulated) return;

            if (st == QMediaPlayer::EndOfMedia){
                this->finishTrack();
            }
            else if (st == QMediaPlayer::InvalidMedia){
                this->startSimulation();
            }
        }
    );

    QObject::connect(
        this->player,
        &QMediaPlayer::errorOccurred,
        this,
        [this](QMediaPlayer::Error err, const QString&){
            if (!this->simulated &&
                err != QMediaPlayer::NoError &&
                this->hasTrack){

                this->startSimulation();
            }
        }
    );

    QObject::connect(
        this->simTimer,
        &QTimer::timeout,
        this,
        [this](){
            this->simPos += 200;

            this->showPosition(this->simPos);
            this->maybeCount(this->simPos);

            if (this->simPos >= this->seek->maximum()){
                this->finishTrack();
            }
        }
    );

    QObject::connect(
        this->playBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            this->togglePlay();
        }
    );

    QObject::connect(
        this->nextBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            this->next();
        }
    );

    QObject::connect(
        this->prevBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            this->previous();
        }
    );

    // Nút Shuffle
    QObject::connect(
        this->shuffleBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            bool on = this->shuffleBtn->isChecked();

            this->setShuffle(on);

            if (this->onMessage){
                this->onMessage(
                    on ? "Da bat phat ngau nhien"
                       : "Da tat phat ngau nhien",
                    false
                );
            }

            if (this->onTrackChanged){
                this->onTrackChanged();
            }
        }
    );

    QObject::connect(
        this->repeatBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            this->cycleRepeat();
        }
    );

    QObject::connect(
        this->heartBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            if (!this->hasTrack){
                this->heartBtn->setChecked(false);
                return;
            }

            bool liked = this->ms.toggleLike(this->current.id);

            this->syncLiked(this->current.id, liked);

            if (this->onLikeToggled){
                this->onLikeToggled(this->current.id, liked);
            }

            if (this->onMessage){
                if (this->ms.isAdmin()){
                    this->onMessage(
                        "Tai khoan Admin khong co danh sach yeu thich",
                        true
                    );
                }
                else{
                    this->onMessage(
                        liked ? "Da them vao Bai hat da thich"
                              : "Da go khoi Bai hat da thich",
                        false
                    );
                }
            }
        }
    );

    QObject::connect(
        this->queueBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            if (this->onQueueToggle){
                this->onQueueToggle();
            }
        }
    );

    QObject::connect(
        this->volBtn,
        &QAbstractButton::clicked,
        this,
        [this](){
            this->audio->setMuted(!this->audio->isMuted());
            this->updateVolumeIcon();
        }
    );

    this->volume->onScrub = [this](int v){
        this->audio->setVolume(v / 100.0f);

        if (v > 0){
            this->audio->setMuted(false);
        }

        this->updateVolumeIcon();
    };

    this->seek->onScrub = [this](int v){
        this->posLabel->setText(
            theme::formatTime(v / 1000)
        );
    };

    this->seek->onUserSeek = [this](int v){
        if (this->hasTrack){
            this->seekTo(v);
        }
    };

    this->titleLabel->onClick = [this](){
        if (this->hasTrack &&
            !this->current.idAlbum.empty() &&
            this->onOpenAlbum){

            this->onOpenAlbum(this->current.idAlbum);
        }
    };

    this->artistLabel->onClick = [this](){
        if (this->hasTrack && this->onOpenArtist){
            this->onOpenArtist(this->current.idArtist);
        }
    };

    this->resetIdleUi();
}

void PlayerBar::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.fillRect(this->rect(), QColor(0, 0, 0));
}

void PlayerBar::resetIdleUi(){
    this->cover->setSpecial(
        Icon::Note,
        QColor(60, 60, 60),
        QColor(36, 36, 36)
    );

    this->titleLabel->setFullText("Chua phat bai nao");
    this->titleLabel->setClickable(false);

    this->artistLabel->setFullText(
        "Chon mot bai hat de bat dau"
    );
    this->artistLabel->setClickable(false);

    this->heartBtn->setChecked(false);
    this->heartBtn->setGlyph(Icon::Heart);

    this->playBtn->setGlyph(Icon::Play);

    this->posLabel->setText("0:00");
    this->durLabel->setText("0:00");

    this->seek->setRange(0, 0);
    this->seek->setValue(0);

    this->shuffleBtn->setChecked(false);

    this->repeatBtn->setChecked(false);
    this->repeatBtn->setGlyph(Icon::Repeat);
}

qint64 PlayerBar::position() const{
    return this->simulated
        ? this->simPos
        : this->player->position();
}

void PlayerBar::showPosition(qint64 posMs){
    if (!this->seek->isDragging()){
        this->seek->setValue((int)posMs);
    }

    this->posLabel->setText(
        theme::formatTime((int)(posMs / 1000))
    );
}

void PlayerBar::seekTo(qint64 posMs){
    if (this->simulated){
        this->simPos = posMs;
    }
    else{
        this->player->setPosition(posMs);
    }

    this->showPosition(posMs);
}

void PlayerBar::setPlaying(bool on){
    this->playing = on;

    this->playBtn->setGlyph(
        on ? Icon::Pause : Icon::Play
    );
}

void PlayerBar::startSimulation(){
    this->simulated = true;
    this->player->stop();

    this->simPos = 0;

    this->seek->setRange(
        0,
        this->current.duration * 1000
    );

    this->durLabel->setText(
        theme::formatTime(this->current.duration)
    );

    this->simTimer->start();
    this->setPlaying(true);
}

void PlayerBar::playTrack(const SongInfo& info){
    this->current = info;
    this->hasTrack = true;
    this->counted = false;

    this->cover->setSeed(
        theme::qs(info.id),
        theme::qs(info.name)
    );

    this->titleLabel->setFullText(
        theme::qs(info.name)
    );
    this->titleLabel->setClickable(
        !info.idAlbum.empty()
    );

    this->artistLabel->setFullText(
        theme::qs(info.artist)
    );
    this->artistLabel->setClickable(true);

    this->heartBtn->setChecked(info.liked);
    this->heartBtn->setGlyph(
        info.liked ? Icon::HeartFilled : Icon::Heart
    );

    this->posLabel->setText("0:00");

    this->durLabel->setText(
        theme::formatTime(info.duration)
    );

    this->seek->setRange(
        0,
        info.duration * 1000
    );
    this->seek->setValue(0);

    this->simTimer->stop();
    this->player->stop();

    if (!info.path.empty()){
        this->simulated = false;

        this->player->setSource(
            QUrl::fromLocalFile(theme::qs(info.path))
        );

        this->player->play();
        this->setPlaying(true);
    }
    else{
        this->startSimulation();

        if (this->onMessage){
            this->onMessage(
                "Chua co file nhac cho bai nay, dang phat mo phong",
                false
            );
        }
    }

    if (this->onTrackChanged){
        this->onTrackChanged();
    }
}

void PlayerBar::togglePlay(){
    if (!this->hasTrack) return;

    if (this->playing){
        if (this->simulated){
            this->simTimer->stop();
        }
        else{
            this->player->pause();
        }

        this->setPlaying(false);
    }
    else{
        if (this->simulated){
            if (this->simPos >= this->seek->maximum()){
                this->simPos = 0;
            }

            this->simTimer->start();
        }
        else{
            this->player->play();
        }

        this->setPlaying(true);
    }

    if (this->onTrackChanged){
        this->onTrackChanged();
    }
}

void PlayerBar::next(){
    if (!this->hasTrack) return;

    SongInfo out;

    if (this->ms.nextTrack(out)){
        this->playTrack(out);
    }
    else if (this->onMessage){
        this->onMessage("Da het danh sach phat", false);
    }
}

void PlayerBar::previous(){
    if (!this->hasTrack) return;

    if (this->position() > 3000){
        this->seekTo(0);
        return;
    }

    SongInfo out;

    if (this->ms.prevTrack(out)){
        this->playTrack(out);
    }
    else{
        this->seekTo(0);
    }
}

void PlayerBar::finishTrack(){
    SongInfo out;

    if (this->ms.trackFinished(out)){
        this->playTrack(out);
        return;
    }

    this->simTimer->stop();
    this->player->stop();

    this->simPos = 0;
    this->seek->setValue(0);
    this->posLabel->setText("0:00");

    this->setPlaying(false);

    if (this->onTrackChanged){
        this->onTrackChanged();
    }
}

void PlayerBar::maybeCount(qint64 posMs){
    if (this->counted || !this->hasTrack) return;

    qint64 threshold = std::min<qint64>(
        5000,
        std::max<qint64>(
            1000,
            (qint64)this->current.duration * 500
        )
    );

    if (posMs < threshold) return;

    this->counted = true;

    this->ms.recordPlay(this->current.id);
}

// Đảo trạng thái Shuffle thông qua nút giao diện
void PlayerBar::toggleShuffle(){
    this->shuffleBtn->click();
}

// Bật/tắt Shuffle và đồng bộ giao diện với nghiệp vụ
void PlayerBar::setShuffle(bool on){
    this->ms.setShuffle(on);
    this->shuffleBtn->setChecked(on);
}

void PlayerBar::cycleRepeat(){
    PlayerService::RepeatMode mode = this->ms.getRepeatMode();

    PlayerService::RepeatMode nextMode =
        (mode == PlayerService::RepeatNone)
            ? PlayerService::RepeatAll
            : (mode == PlayerService::RepeatAll)
                ? PlayerService::RepeatOne
                : PlayerService::RepeatNone;

    this->ms.setRepeatMode(nextMode);

    this->repeatBtn->setChecked(
        nextMode != PlayerService::RepeatNone
    );

    this->repeatBtn->setGlyph(
        nextMode == PlayerService::RepeatOne
            ? Icon::RepeatOne
            : Icon::Repeat
    );

    if (this->onMessage){
        this->onMessage(
            nextMode == PlayerService::RepeatNone
                ? "Tat lap lai"
                : nextMode == PlayerService::RepeatAll
                    ? "Lap lai danh sach"
                    : "Lap lai mot bai",
            false
        );
    }
}

void PlayerBar::clear(){
    this->simTimer->stop();
    this->player->stop();

    this->hasTrack = false;
    this->playing = false;
    this->simulated = false;
    this->simPos = 0;

    this->current = SongInfo();

    this->resetIdleUi();
    this->setQueueActive(false);
}

bool PlayerBar::hasCurrent() const{
    return this->hasTrack;
}

bool PlayerBar::isPlaying() const{
    return this->playing;
}

std::string PlayerBar::currentId() const{
    return this->hasTrack
        ? this->current.id
        : std::string();
}

void PlayerBar::syncLiked(
    const std::string& idSong,
    bool liked
){
    if (!this->hasTrack || this->current.id != idSong){
        return;
    }

    this->current.liked = liked;
    this->heartBtn->setChecked(liked);

    this->heartBtn->setGlyph(
        liked ? Icon::HeartFilled : Icon::Heart
    );
}

void PlayerBar::setQueueActive(bool on){
    this->queueBtn->setChecked(on);
}

void PlayerBar::updateVolumeIcon(){
    bool mute = this->audio->isMuted() ||
                this->volume->value() == 0;

    this->volBtn->setGlyph(
        mute ? Icon::Mute : Icon::Volume
    );
}
