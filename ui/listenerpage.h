#pragma once
#include "musicsystem.h"
#include "uihelper.h"
#include "player.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QWidget>
#include <vector>

class ListenerPage : public QWidget{
    Q_OBJECT
public:
    explicit ListenerPage(MusicSystem* sys, QWidget* parent = nullptr);
public slots:
    void refresh();                     // nap lai danh sach nguon + bang bai hat
private slots:
    void onSearch();
    void onPlaySelected();
    void onPrev();
    void onNext();
    void onFinished();
    void onNewPlaylist();
    void onDeletePlaylist();
    void onAddToPlaylist();
    void onRemoveFromPlaylist();
private:
    void rebuildSources();
    void reloadView();
    void fill(const std::vector<SongInfo>& list);
    void playInfo(const SongInfo& s);
    QString viewKey() const;
    int selectedRow() const;
    static QString fmtMs(qint64 ms);

    MusicSystem* sys;
    Player* player;
    std::vector<SongInfo> shown;
    QListWidget* sources;
    QLineEdit* search;
    QTableWidget* table;
    QLabel *nowPlaying, *timeLbl;
    QSlider* seek;
    QPushButton *btnDelPl, *btnRemPl;
};
ListenerPage::ListenerPage(MusicSystem* s, QWidget* parent) : QWidget(parent), sys(s){
    player = new Player(this);
    sources = new QListWidget;
    sources->setMaximumWidth(240);
    sources->setMinimumWidth(200);
    auto* btnNewPl = new QPushButton("+ Create playlist");
    btnDelPl = new QPushButton("Remove playlist");

    auto* left = new QVBoxLayout;
    auto* libTitle = new QLabel("My library"); libTitle->setObjectName("sectionTitle");
    left->addWidget(libTitle);
    left->addWidget(sources, 1);
    left->addWidget(btnNewPl);
    left->addWidget(btnDelPl);

    search = new QLineEdit; search->setPlaceholderText("Tim theo ten bai, ca si, album...");
    table = new QTableWidget(0, 6);
    table->setHorizontalHeaderLabels({"Ten bai hat", "Ca si", "Album", "The loai", "Thoi luong", "Luot nghe"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setShowGrid(false);
    table->verticalHeader()->hide();
    table->verticalHeader()->setDefaultSectionSize(40);
    table->horizontalHeader()->setHighlightSections(false);

    auto* btnPlay = new QPushButton("Phat bai dang chon");
    auto* btnAddPl = new QPushButton("Them vao playlist...");
    btnRemPl = new QPushButton("Xoa khoi playlist");
    btnPlay->setObjectName("primary");
    btnRemPl->setObjectName("danger");
    btnDelPl->setObjectName("danger");
    btnNewPl->setObjectName("primary");

    nowPlaying = new QLabel("Chua phat bai nao");
    nowPlaying->setObjectName("nowPlaying");
    timeLbl = new QLabel("0:00 / 0:00");
    timeLbl->setObjectName("muted");
    seek = new QSlider(Qt::Horizontal);

    const QString playSym  = sym({0x25B6});            // tam dung -> hien nut phat
    const QString pauseSym = sym({0x275A, 0x275A});    // dang phat -> hien nut tam dung
    auto* btnPrev  = new QPushButton(sym({0x25C0, 0x25C0}));
    auto* btnPause = new QPushButton(playSym);
    auto* btnNext  = new QPushButton(sym({0x25B6, 0x25B6}));
    btnPrev->setObjectName("transport");  btnPrev->setToolTip("Bai truoc");
    btnNext->setObjectName("transport");  btnNext->setToolTip("Bai tiep theo");
    btnPause->setObjectName("playBtn");   btnPause->setToolTip("Phat / Tam dung");
    auto* vol = new QSlider(Qt::Horizontal); vol->setRange(0, 100); vol->setValue(70); vol->setMaximumWidth(120);

    auto* actions = new QHBoxLayout;
    actions->addWidget(btnPlay); actions->addWidget(btnAddPl); actions->addWidget(btnRemPl); actions->addStretch();

    auto* seekRow = new QHBoxLayout;
    seekRow->addWidget(seek, 1); seekRow->addWidget(timeLbl);

    auto* transport = new QHBoxLayout;
    transport->addSpacing(160);
    transport->addStretch();
    transport->addWidget(btnPrev); transport->addWidget(btnPause); transport->addWidget(btnNext);
    transport->addStretch();
    auto* volLbl = new QLabel("Am luong"); volLbl->setObjectName("muted");
    transport->addWidget(volLbl); transport->addWidget(vol);

    auto* right = new QVBoxLayout;
    right->addWidget(search);
    right->addWidget(table, 1);
    right->addLayout(actions);
    right->addWidget(nowPlaying);
    right->addLayout(seekRow);
    right->addLayout(transport);

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(16, 12, 16, 16);
    root->setSpacing(16);
    right->setSpacing(10);
    root->addLayout(left);
    root->addLayout(right, 1);

    connect(sources, &QListWidget::currentRowChanged, this, [this]{ reloadView(); });
    connect(search, &QLineEdit::textChanged, this, &ListenerPage::onSearch);
    connect(btnNewPl, &QPushButton::clicked, this, &ListenerPage::onNewPlaylist);
    connect(btnDelPl, &QPushButton::clicked, this, &ListenerPage::onDeletePlaylist);
    connect(btnPlay, &QPushButton::clicked, this, &ListenerPage::onPlaySelected);
    connect(table, &QTableWidget::cellDoubleClicked, this, &ListenerPage::onPlaySelected);
    connect(btnAddPl, &QPushButton::clicked, this, &ListenerPage::onAddToPlaylist);
    connect(btnRemPl, &QPushButton::clicked, this, &ListenerPage::onRemoveFromPlaylist);
    connect(btnPrev, &QPushButton::clicked, this, &ListenerPage::onPrev);
    connect(btnNext, &QPushButton::clicked, this, &ListenerPage::onNext);
    connect(btnPause, &QPushButton::clicked, player, &Player::togglePause);
    connect(vol, &QSlider::valueChanged, player, &Player::setVolume);
    connect(player, &Player::playingChanged, this, [btnPause, playSym, pauseSym](bool playing){
        btnPause->setText(playing ? pauseSym : playSym);
    });
    connect(seek, &QSlider::sliderMoved, player, [this](int v){ player->seek(v); });
    connect(player, &Player::durationChanged, this, [this](qint64 d){ seek->setMaximum((int)d); });
    connect(player, &Player::positionChanged, this, [this](qint64 p){
        if (!seek->isSliderDown()) seek->setValue((int)p);
        timeLbl->setText(fmtMs(p) + " / " + fmtMs(seek->maximum()));
    });
    connect(player, &Player::finished, this, &ListenerPage::onFinished);

    if (sys->isAdmin()) {   // chi Listener moi co playlist
        btnNewPl->setEnabled(false);
        btnAddPl->setEnabled(false);
        btnNewPl->setToolTip("Chi tai khoan Listener moi co playlist");
        btnAddPl->setToolTip("Chi tai khoan Listener moi co playlist");
    }

    refresh();
}
QString ListenerPage::fmtMs(qint64 ms){ 
    return fmtSec((int)(ms / 1000)); 
}
QString ListenerPage::viewKey() const{
    auto* it = sources->currentItem();
    return it ? it->data(Qt::UserRole).toString() : QString("lib");
}
int ListenerPage::selectedRow() const{
    int r = table->currentRow();
    return (r >= 0 && r < (int)shown.size()) ? r : -1;
}
void ListenerPage::rebuildSources(){
    QString keep = viewKey();
    sources->blockSignals(true);
    sources->clear();
    auto add = [this](const QString& text, const QString& key){
        auto* it = new QListWidgetItem(text);
        it->setData(Qt::UserRole, key);
        sources->addItem(it);
    };
    add("Tat ca bai hat", "lib");
    add("Top luot nghe", "top");
    for (const auto& pl : sys->myPlaylists())
        add(QString("%1 (%2)").arg(q(pl.name)).arg(pl.songCount), "pl:" + q(pl.id));

    int row = 0;
    for (int i = 0; i < sources->count(); ++i)
        if (sources->item(i)->data(Qt::UserRole).toString() == keep) row = i;
    sources->setCurrentRow(row);
    sources->blockSignals(false);
}
void ListenerPage::reloadView(){
    QString key = viewKey();
    bool isPl = key.startsWith("pl:");
    search->setEnabled(key == "lib");
    btnDelPl->setEnabled(isPl);
    btnRemPl->setEnabled(isPl);

    if (key == "lib"){
        std::string k = search->text().toStdString();
        fill(k.empty() ? sys->allSongs() : sys->search(k));
    } else if (key == "top"){
        fill(sys->topSongs(10));                            
    } else if (isPl){
        fill(sys->playlistSongs(key.mid(3).toStdString()));   
    }
}
void ListenerPage::refresh(){ 
    rebuildSources(); 
    reloadView(); 
}
void ListenerPage::fill(const std::vector<SongInfo>& list){
    shown = list;
    table->setRowCount((int)list.size());
    for (int i = 0; i < (int)list.size(); ++i){
        const SongInfo& s = list[i];
        table->setItem(i, 0, new QTableWidgetItem(q(s.name)));
        table->setItem(i, 1, new QTableWidgetItem(q(s.artist)));
        table->setItem(i, 2, new QTableWidgetItem(q(s.album)));
        table->setItem(i, 3, new QTableWidgetItem(q(s.type)));
        table->setItem(i, 4, new QTableWidgetItem(fmtSec(s.duration)));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(s.count)));
    }
}
void ListenerPage::onSearch(){
    if (viewKey() != "lib") sources->setCurrentRow(0);   
    else reloadView();
}
void ListenerPage::playInfo(const SongInfo& s){
    if (s.path.empty()) {
        nowPlaying->setText("Khong tim thay file nhac: " + q(s.name));
        return;
    }
    player->play(q(s.path));
    nowPlaying->setText(QString("Dang phat: %1 - %2").arg(q(s.name), q(s.artist)));
    sys->recordPlay(s.id);

    int row = table->currentRow();
    reloadView();
    if (row >= 0 && row < table->rowCount()) table->selectRow(row);
}
void ListenerPage::onPlaySelected(){
    int r = selectedRow();
    if (r < 0){ 
        QMessageBox::information(this, "Thong bao", "Hay chon mot bai hat."); 
        return; 
    }
    SongInfo cur;
    bool ok;
    QString key = viewKey();
    if (key.startsWith("pl:")){
        ok = sys->startPlaylist(key.mid(3).toStdString(), shown[r].id, cur);
    } else{
        std::vector<std::string> ids;
        for (const auto& s : shown) ids.push_back(s.id);
        ok = sys->startQueue(ids, shown[r].id, cur);
    }
    if (ok) playInfo(cur);
}
void ListenerPage::onPrev(){
    SongInfo s;
    if (sys->prevTrack(s)) playInfo(s);
}
void ListenerPage::onNext(){
    SongInfo s;
    if (sys->nextTrack(s)) playInfo(s);
    else nowPlaying->setText("Da het danh sach phat");
}
void ListenerPage::onFinished(){ 
    onNext(); 
}
void ListenerPage::onNewPlaylist(){
    bool ok;
    QString name = QInputDialog::getText(this, "Tao playlist", "Ten playlist:", QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) return;
    if (sys->createPlaylist(name.toStdString()).empty()){
        QMessageBox::warning(this, "Khong tao duoc", "Chi tai khoan Listener moi tao duoc playlist.");
        return;
    }
    refresh();
}
void ListenerPage::onDeletePlaylist(){
    QString key = viewKey();
    if (!key.startsWith("pl:")) return;
    if (QMessageBox::question(this, "Xac nhan", "Xoa playlist nay?") != QMessageBox::Yes) return;
    sys->deletePlaylist(key.mid(3).toStdString());
    refresh();
}
void ListenerPage::onAddToPlaylist(){
    int r = selectedRow();
    if (r < 0){ 
        QMessageBox::information(this, "Thong bao", "Hay chon mot bai hat."); 
        return; 
    }
    auto pls = sys->myPlaylists();
    if (pls.empty()){ 
        QMessageBox::information(this, "Thong bao", "Hay tao playlist truoc."); 
        return; 
    }
    QStringList names;
    for (const auto& p : pls) names << q(p.name);
    bool ok;
    QString chosen = QInputDialog::getItem(this, "Them vao playlist", "Chon playlist:", names, 0, false, &ok);
    if (!ok) return;
    int idx = names.indexOf(chosen);
    if (!sys->addToPlaylist(pls[idx].id, shown[r].id))
        QMessageBox::information(this, "Thong bao", "Bai hat da co trong playlist.");
    else
        refresh();
}
void ListenerPage::onRemoveFromPlaylist(){
    int r = selectedRow();
    QString key = viewKey();
    if (r < 0 || !key.startsWith("pl:")) return;
    sys->removeFromPlaylist(key.mid(3).toStdString(), shown[r].id);
    refresh();
}
