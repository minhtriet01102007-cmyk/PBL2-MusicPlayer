#pragma once
#include <functional>
#include <vector>
#include <QAbstractItemView>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVariantAnimation>
#include <QWidget>
#include "bll/musicsystem.h"
#include "dialogs.h"
#include "icon.h"
#include "theme.h"
#include "widgets.h"

// Thẻ thống kê có số chạy từ 0 đến giá trị thật
class StatCard : public QWidget{
    private:
        Icon icon;
        QString label;
        QColor color;
        qreal shown;
        QVariantAnimation* anim;
    protected:
        void paintEvent(QPaintEvent* e) override;
    public:
        StatCard(Icon cardIcon, const QString& text, const QColor& accent, QWidget* parent = nullptr);
        void setValue(long long value);
};

// Màn hình quản trị (chỉ Admin): thống kê, quản lý bài hát, nghệ sĩ, album và người dùng.
class AdminPage : public QWidget{
    private:
        MusicSystem& ms;
        QTabWidget* tabs;
        StatCard* cardSongs;
        StatCard* cardArtists;
        StatCard* cardAlbums;
        StatCard* cardUsers;
        StatCard* cardPlays;
        QVBoxLayout* topLayout;
        QTableWidget* songTable;
        QTableWidget* artistTable;
        QTableWidget* albumTable;
        QTableWidget* userTable;
        std::vector<SongInfo> songs;
        std::vector<ArtistInfo> artists;
        std::vector<AlbumInfo> albums;
        std::vector<UserInfo> users;
        QTableWidget* makeTable(const QStringList& headers);
        QWidget* makeTab(QTableWidget* table, const std::vector<QWidget*>& toolbar, QLineEdit* filter);
        int selectedRow(QTableWidget* t) const;
        void notify(const QString& text, bool isError);
        void changed();
        void reloadStats();
        void reloadSongs();
        void reloadArtists();
        void reloadAlbums();
        void reloadUsers();
    public:
        std::function<void()> onChanged;                          // dữ liệu danh mục đã đổi -> trang khác cần làm mới
        std::function<void(const QString&, bool)> onMessage;
        explicit AdminPage(MusicSystem& system, QWidget* parent = nullptr);
        void reloadAll();
};

StatCard::StatCard(Icon cardIcon, const QString& text, const QColor& accent, QWidget* parent)
    : QWidget(parent), icon(cardIcon), label(text), color(accent), shown(0.0){
    this->setMinimumSize(190, 110);
    this->anim = new QVariantAnimation(this);
    this->anim->setDuration(900);
    this->anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(this->anim, &QVariantAnimation::valueChanged, this, [this](const QVariant& v){
        this->shown = v.toReal();
        this->update();
    });
}
void StatCard::setValue(long long value){
    this->anim->stop();
    this->anim->setStartValue(this->shown);
    this->anim->setEndValue((qreal)value);
    this->anim->start();
}
void StatCard::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRectF r(this->rect());
    QLinearGradient g(r.topLeft(), r.bottomRight());
    g.setColorAt(0.0, QColor(34, 34, 34));
    g.setColorAt(1.0, theme::mixColor(QColor(24, 24, 24), this->color, 0.22));
    p.setPen(Qt::NoPen);
    p.setBrush(g);
    p.drawRoundedRect(r, 14, 14);
    p.setBrush(theme::mixColor(QColor(30, 30, 30), this->color, 0.9));
    p.drawEllipse(QRectF(18, 18, 38, 38));
    drawIcon(p, this->icon, QRectF(26, 26, 22, 22), QColor(255, 255, 255));
    QFont f = p.font();
    f.setPixelSize(32);
    f.setBold(true);
    p.setFont(f);
    p.setPen(theme::kText);
    p.drawText(QRectF(72, 14, r.width() - 84, 46), Qt::AlignVCenter | Qt::AlignRight, QString::number((long long)(this->shown + 0.5)));
    QFont lf = p.font();
    lf.setPixelSize(13);
    lf.setBold(false);
    p.setFont(lf);
    p.setPen(theme::kSubText);
    p.drawText(QRectF(18, 70, r.width() - 36, 28), Qt::AlignVCenter | Qt::AlignLeft, this->label);
}

AdminPage::AdminPage(MusicSystem& system, QWidget* parent) : QWidget(parent), ms(system){
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 22, 28, 20);
    root->setSpacing(12);
    QHBoxLayout* head = new QHBoxLayout;
    head->setSpacing(12);
    QLabel* shield = new QLabel(this);
    shield->setFixedSize(30, 30);
    root->addLayout(head);
    head->addWidget(makeLabel("Quan tri he thong", 30, true));
    head->addStretch();
    delete shield;

    this->tabs = new QTabWidget(this);
    root->addWidget(this->tabs, 1);

    // --- Tổng quan ---
    QWidget* overview = new QWidget;
    this->topLayout = new QVBoxLayout(overview);
    this->topLayout->setContentsMargins(0, 16, 0, 0);
    this->topLayout->setSpacing(18);
    QHBoxLayout* cards = new QHBoxLayout;
    cards->setSpacing(14);
    this->cardSongs = new StatCard(Icon::Note, "Bai hat", QColor(29, 185, 84));
    this->cardArtists = new StatCard(Icon::Mic, "Nghe si", QColor(120, 80, 230));
    this->cardAlbums = new StatCard(Icon::Album, "Album", QColor(0, 150, 200));
    this->cardUsers = new StatCard(Icon::User, "Nguoi dung", QColor(230, 140, 40));
    this->cardPlays = new StatCard(Icon::Play, "Tong luot nghe", QColor(230, 70, 110));
    cards->addWidget(this->cardSongs);
    cards->addWidget(this->cardArtists);
    cards->addWidget(this->cardAlbums);
    cards->addWidget(this->cardUsers);
    cards->addWidget(this->cardPlays);
    this->topLayout->addLayout(cards);
    this->topLayout->addStretch();
    this->tabs->addTab(overview, "Tong quan");

    // --- Bài hát ---
    this->songTable = this->makeTable({"ID", "Ten bai hat", "Nghe si", "Album", "The loai", "Thoi luong", "Luot nghe", "File nhac"});
    QLineEdit* songFilter = new QLineEdit;
    songFilter->setPlaceholderText("Loc theo ten, nghe si, the loai...");
    QPushButton* addSongBtn = makeButton("+ Them bai hat", "primary");
    QPushButton* editSongBtn = makeButton("Sua", "soft");
    QPushButton* delSongBtn = makeButton("Xoa", "danger");
    this->tabs->addTab(this->makeTab(this->songTable, {addSongBtn, editSongBtn, delSongBtn}, songFilter), "Bai hat");

    // --- Nghệ sĩ ---
    this->artistTable = this->makeTable({"ID", "Ten nghe si", "Quoc gia", "Nguoi theo doi", "So bai hat"});
    QPushButton* addArtistBtn = makeButton("+ Them nghe si", "primary");
    QPushButton* delArtistBtn = makeButton("Xoa", "danger");
    this->tabs->addTab(this->makeTab(this->artistTable, {addArtistBtn, delArtistBtn}, nullptr), "Nghe si");

    // --- Album ---
    this->albumTable = this->makeTable({"ID", "Ten album", "Nghe si", "Ngay phat hanh", "So bai", "Thoi luong"});
    QPushButton* addAlbumBtn = makeButton("+ Them album", "primary");
    QPushButton* delAlbumBtn = makeButton("Xoa", "danger");
    this->tabs->addTab(this->makeTab(this->albumTable, {addAlbumBtn, delAlbumBtn}, nullptr), "Album");

    // --- Người dùng ---
    this->userTable = this->makeTable({"ID", "Tai khoan", "Email", "Vai tro", "Trang thai", "Bai da nghe", "Playlist"});
    QLineEdit* userFilter = new QLineEdit;
    userFilter->setPlaceholderText("Loc theo ten tai khoan, email...");
    QPushButton* lockBtn = makeButton("Khoa / Mo khoa", "soft");
    this->tabs->addTab(this->makeTab(this->userTable, {lockBtn}, userFilter), "Nguoi dung");

    // --- Lọc dữ liệu trong bảng ---
    auto filterRows = [](QTableWidget* t, const QString& text){
        for (int r = 0; r < t->rowCount(); ++r){
            bool match = text.trimmed().isEmpty();
            for (int c = 0; c < t->columnCount() && !match; ++c){
                QTableWidgetItem* it = t->item(r, c);
                if (it != nullptr && it->text().contains(text.trimmed(), Qt::CaseInsensitive)) match = true;
            }
            t->setRowHidden(r, !match);
        }
    };
    QObject::connect(songFilter, &QLineEdit::textChanged, this, [this, filterRows](const QString& t){ filterRows(this->songTable, t); });
    QObject::connect(userFilter, &QLineEdit::textChanged, this, [this, filterRows](const QString& t){ filterRows(this->userTable, t); });

    // --- Hành động ---
    QObject::connect(addSongBtn, &QAbstractButton::clicked, this, [this](){
        if (this->ms.allArtists().empty()){
            this->notify("Hay them nghe si truoc khi them bai hat", true);
            return;
        }
        if (songDialog(this, this->ms, nullptr)){
            this->notify("Da them bai hat", false);
            this->changed();
        }
    });
    QObject::connect(editSongBtn, &QAbstractButton::clicked, this, [this](){
        int r = this->selectedRow(this->songTable);
        if (r < 0 || r >= (int)this->songs.size()){
            this->notify("Hay chon mot bai hat", true);
            return;
        }
        if (songDialog(this, this->ms, &this->songs[r])){
            this->notify("Da cap nhat bai hat", false);
            this->changed();
        }
    });
    QObject::connect(delSongBtn, &QAbstractButton::clicked, this, [this](){
        int r = this->selectedRow(this->songTable);
        if (r < 0 || r >= (int)this->songs.size()){
            this->notify("Hay chon mot bai hat", true);
            return;
        }
        SongInfo s = this->songs[r];
        if (!confirmDialog(this, "Xoa bai hat", "Xoa \"" + theme::qs(s.name) + "\"? Bai hat cung se bi go khoi moi playlist và danh sách yeu thich.")) return;
        std::string err = this->ms.removeSong(s.id);
        if (!err.empty()) this->notify(theme::qs(err), true);
        else{
            this->notify("Da xoa bai hat", false);
            this->changed();
        }
    });
    QObject::connect(addArtistBtn, &QAbstractButton::clicked, this, [this](){
        if (artistDialog(this, this->ms)){
            this->notify("Da them nghe si", false);
            this->changed();
        }
    });
    QObject::connect(delArtistBtn, &QAbstractButton::clicked, this, [this](){
        int r = this->selectedRow(this->artistTable);
        if (r < 0 || r >= (int)this->artists.size()){
            this->notify("Hay chon mot nghe si", true);
            return;
        }
        ArtistInfo a = this->artists[r];
        if (!confirmDialog(this, "Xoa nghe si", "Xoa nghe si \"" + theme::qs(a.name) + "\"?")) return;
        std::string err = this->ms.removeArtist(a.id);
        if (!err.empty()) this->notify(theme::qs(err), true);
        else{
            this->notify("Da xoa nghe si", false);
            this->changed();
        }
    });
    QObject::connect(addAlbumBtn, &QAbstractButton::clicked, this, [this](){
        if (this->ms.allArtists().empty()){
            this->notify("Hay them nghe si truoc khi them album", true);
            return;
        }
        if (albumDialog(this, this->ms)){
            this->notify("Da them album", false);
            this->changed();
        }
    });
    QObject::connect(delAlbumBtn, &QAbstractButton::clicked, this, [this](){
        int r = this->selectedRow(this->albumTable);
        if (r < 0 || r >= (int)this->albums.size()){
            this->notify("Hay chon mot album", true);
            return;
        }
        AlbumInfo al = this->albums[r];
        if (!confirmDialog(this, "Xoa album", "Xoa album \"" + theme::qs(al.name) + "\"? Cac bai hat trong album van duoc giu lai.")) return;
        std::string err = this->ms.removeAlbum(al.id);
        if (!err.empty()) this->notify(theme::qs(err), true);
        else{
            this->notify("Da xoa album", false);
            this->changed();
        }
    });
    QObject::connect(lockBtn, &QAbstractButton::clicked, this, [this](){
        int r = this->selectedRow(this->userTable);
        if (r < 0 || r >= (int)this->users.size()){
            this->notify("Hay chon mot nguoi dung", true);
            return;
        }
        UserInfo u = this->users[r];
        std::string err = this->ms.setUserLocked(u.id, u.active);
        if (!err.empty()) this->notify(theme::qs(err), true);
        else{
            this->notify(u.active ? "Da khoa tai khoan " + theme::qs(u.username) : "Da mo khoa tai khoan " + theme::qs(u.username), false);
            this->reloadUsers();
        }
    });
    this->reloadAll();
}

QTableWidget* AdminPage::makeTable(const QStringList& headers){
    QTableWidget* t = new QTableWidget(0, headers.size());
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    t->horizontalHeader()->setHighlightSections(false);
    t->verticalHeader()->hide();
    t->verticalHeader()->setDefaultSectionSize(42);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setShowGrid(false);
    t->setFocusPolicy(Qt::NoFocus);
    return t;
}
QWidget* AdminPage::makeTab(QTableWidget* table, const std::vector<QWidget*>& toolbar, QLineEdit* filter){
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);
    lay->setContentsMargins(0, 14, 0, 0);
    lay->setSpacing(12);
    QHBoxLayout* bar = new QHBoxLayout;
    bar->setSpacing(10);
    for (QWidget* b : toolbar) bar->addWidget(b);
    bar->addStretch();
    if (filter != nullptr){
        filter->setMinimumWidth(280);
        bar->addWidget(filter);
    }
    lay->addLayout(bar);
    lay->addWidget(table, 1);
    return w;
}
int AdminPage::selectedRow(QTableWidget* t) const{
    return t->currentRow();
}
void AdminPage::notify(const QString& text, bool isError){
    if (this->onMessage) this->onMessage(text, isError);
}
void AdminPage::changed(){
    this->reloadAll();
    if (this->onChanged) this->onChanged();
}
void AdminPage::reloadAll(){
    this->reloadStats();
    this->reloadSongs();
    this->reloadArtists();
    this->reloadAlbums();
    this->reloadUsers();
}
void AdminPage::reloadStats(){
    StatsInfo s = this->ms.stats();
    this->cardSongs->setValue(s.songs);
    this->cardArtists->setValue(s.artists);
    this->cardAlbums->setValue(s.albums);
    this->cardUsers->setValue(s.users);
    this->cardPlays->setValue(s.totalPlays);
}
void AdminPage::reloadSongs(){
    this->songs = this->ms.allSongs();
    this->songTable->setRowCount((int)this->songs.size());
    for (int i = 0; i < (int)this->songs.size(); ++i){
        const SongInfo& s = this->songs[i];
        this->songTable->setItem(i, 0, new QTableWidgetItem(theme::qs(s.id)));
        this->songTable->setItem(i, 1, new QTableWidgetItem(theme::qs(s.name)));
        this->songTable->setItem(i, 2, new QTableWidgetItem(theme::qs(s.artist)));
        this->songTable->setItem(i, 3, new QTableWidgetItem(theme::qs(s.album)));
        this->songTable->setItem(i, 4, new QTableWidgetItem(theme::qs(s.type)));
        this->songTable->setItem(i, 5, new QTableWidgetItem(theme::formatTime(s.duration)));
        this->songTable->setItem(i, 6, new QTableWidgetItem(QString::number(s.plays)));
        this->songTable->setItem(i, 7, new QTableWidgetItem("")); // cột file nhạc: điền ở dưới
    }
}
void AdminPage::reloadArtists(){
    this->artists = this->ms.allArtists();
    this->artistTable->setRowCount((int)this->artists.size());
    for (int i = 0; i < (int)this->artists.size(); ++i){
        const ArtistInfo& a = this->artists[i];
        this->artistTable->setItem(i, 0, new QTableWidgetItem(theme::qs(a.id)));
        this->artistTable->setItem(i, 1, new QTableWidgetItem(theme::qs(a.name)));
        this->artistTable->setItem(i, 2, new QTableWidgetItem(theme::qs(a.country)));
        this->artistTable->setItem(i, 3, new QTableWidgetItem(QString::number(a.followers)));
        this->artistTable->setItem(i, 4, new QTableWidgetItem(QString::number(a.songCount)));
    }
}
void AdminPage::reloadAlbums(){
    this->albums = this->ms.allAlbums();
    this->albumTable->setRowCount((int)this->albums.size());
    for (int i = 0; i < (int)this->albums.size(); ++i){
        const AlbumInfo& al = this->albums[i];
        this->albumTable->setItem(i, 0, new QTableWidgetItem(theme::qs(al.id)));
        this->albumTable->setItem(i, 1, new QTableWidgetItem(theme::qs(al.name)));
        this->albumTable->setItem(i, 2, new QTableWidgetItem(theme::qs(al.artist)));
        this->albumTable->setItem(i, 3, new QTableWidgetItem(theme::qs(al.date)));
        this->albumTable->setItem(i, 4, new QTableWidgetItem(QString::number(al.trackCount)));
        this->albumTable->setItem(i, 5, new QTableWidgetItem(theme::formatLong(al.totalSeconds)));
    }
}
void AdminPage::reloadUsers(){
    this->users = this->ms.listUsers();
    this->userTable->setRowCount((int)this->users.size());
    for (int i = 0; i < (int)this->users.size(); ++i){
        const UserInfo& u = this->users[i];
        this->userTable->setItem(i, 0, new QTableWidgetItem(theme::qs(u.id)));
        this->userTable->setItem(i, 1, new QTableWidgetItem(theme::qs(u.username)));
        this->userTable->setItem(i, 2, new QTableWidgetItem(theme::qs(u.email)));
        this->userTable->setItem(i, 3, new QTableWidgetItem(theme::qs(u.role)));
        QTableWidgetItem* status = new QTableWidgetItem(u.active ? "Hoạt động" : "Đã khóa");
        status->setForeground(u.active ? theme::kAccent : theme::kDanger);
        this->userTable->setItem(i, 4, status);
        this->userTable->setItem(i, 5, new QTableWidgetItem(QString::number(u.totalPlayed)));
        this->userTable->setItem(i, 6, new QTableWidgetItem(QString::number(u.playlistCount)));
    }
}