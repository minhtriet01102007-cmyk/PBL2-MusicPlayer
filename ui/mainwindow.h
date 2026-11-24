#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include <QAbstractSpinBox>
#include <QApplication>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QPalette>
#include <QPointer>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QShortcut>
#include <QStackedWidget>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include "adminpage.h"
#include "bll/musicsystem.h"
#include "dialogs.h"
#include "icon.h"
#include "loginpage.h"
#include "playerbar.h"
#include "theme.h"
#include "widgets.h"

// Cửa sổ chính: một QStackedWidget chứa [màn hình đăng nhập] và [giao diện ứng dụng].
// Giao diện ứng dụng (shell) được dựng lại mỗi lần đăng nhập và hủy khi đăng xuất (Ctrl+L), nên không còn dữ liệu của phiên trước.
// MainWindow chỉ nói chuyện với MusicSystem (tầng BLL), không đụng tới service, DAO hay entity.
class MainWindow : public QWidget{
    private:
        struct PageEntry{
            QString key;                          // "home", "search", "pl:PL001", "album:AL001"...
            std::function<QWidget*()> build;      // hàm dựng lại trang (luôn lấy dữ liệu mới nhất)
        };
        MusicSystem& ms;
        QStackedWidget* root;
        LoginPage* loginPage;
        QWidget* shell;
        Toast* toast;
        // --- các thành phần của shell (nullptr khi chưa đăng nhập) ---
        QVBoxLayout* libraryLayout;
        std::vector<std::pair<QString, QPointer<SideItem>>> sideItems;
        QLineEdit* searchEdit;
        QTimer* searchTimer;
        IconButton* backBtn;
        IconButton* fwdBtn;
        QPushButton* userBtn;
        FadeStack* pages;
        PlayerBar* player;
        QFrame* queuePanel;
        QVBoxLayout* queueLayout;
        bool queueOpen;
        // --- lịch sử điều hướng ---
        std::vector<PageEntry> history;
        int historyPos;
        std::vector<QPointer<SongRow>> rows;      // các dòng bài hát đang hiển thị (để đánh dấu bài đang phát, đổi tim)

        // dựng khung
        void buildShell();
        void buildSidebar(QVBoxLayout* sideLayout);
        QWidget* buildTopBar();
        void enterApp();
        void doLogout();
        void showUserMenu();
        void setupShortcuts();
        // thanh bên
        void rebuildSidebar();
        void rebuildLibrary();
        void setActiveKey(const QString& key);
        // điều hướng
        void navigate(const QString& key, const std::function<QWidget*()>& build);
        void showCurrent(bool animate);
        void refreshPage();
        void goBack();
        void goForward();
        void openSearch(const QString& text);
        void openArtist(const std::string& id);
        void openAlbum(const std::string& id);
        void openGenre(const std::string& name);
        void openPlaylist(const std::string& id);
        // các trang
        QWidget* pageHome();
        QWidget* pageSearch(const QString& text);
        QWidget* pageSongs();
        QWidget* pageArtists();
        QWidget* pageAlbums();
        QWidget* pageLiked();
        QWidget* pagePlaylist(const std::string& id);
        QWidget* pageAlbum(const std::string& id);
        QWidget* pageArtist(const std::string& id);
        QWidget* pageGenre(const std::string& name);
        QWidget* pageAdmin();
        QWidget* emptyPage(const QString& title, const QString& text);
        // thành phần dùng chung của các trang
        QWidget* emptyState(const QString& title, const QString& text);
        QWidget* makeHeader(CoverWidget* cover, const QString& kind, const QString& title, const QString& meta);
        QHBoxLayout* addActionBar(QVBoxLayout* lay, const std::function<void(bool)>& play);
        void addSection(QVBoxLayout* lay, const QString& title, const std::function<void()>& showAll = std::function<void()>());
        void addSongList(QVBoxLayout* lay, const std::vector<SongInfo>& songs, bool showAlbum, const QString& playlistId);
        Card* songCard(const SongInfo& s, const std::shared_ptr<std::vector<SongInfo>>& context);
        Card* artistCard(const ArtistInfo& a);
        Card* albumCard(const AlbumInfo& al);
        void songMenu(const SongInfo& s, const QPoint& pos, const QString& playlistId);
        // phát nhạc và hàng chờ
        void playSongs(const std::vector<SongInfo>& songs, const std::string& startId, bool shuffle);
        bool toggleLikeFor(const std::string& idSong);
        void applyCurrentHighlight();
        void onTrackChanged();
        void toggleQueue();
        void refreshQueuePanel();
        void say(const QString& text, bool isError = false);
        bool isTyping() const;
    public:
        explicit MainWindow(MusicSystem& system, QWidget* parent = nullptr);
};

MainWindow::MainWindow(MusicSystem& system, QWidget* parent) : QWidget(parent), ms(system){
    this->shell = nullptr;
    this->libraryLayout = nullptr;
    this->searchEdit = nullptr;
    this->searchTimer = nullptr;
    this->backBtn = nullptr;
    this->fwdBtn = nullptr;
    this->userBtn = nullptr;
    this->pages = nullptr;
    this->player = nullptr;
    this->queuePanel = nullptr;
    this->queueLayout = nullptr;
    this->queueOpen = false;
    this->historyPos = -1;
    this->setWindowTitle("Melodia");
    this->setMinimumSize(1100, 700);
    this->resize(1380, 840);

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    this->root = new QStackedWidget(this);
    lay->addWidget(this->root);
    this->loginPage = new LoginPage(this->ms);
    this->root->addWidget(this->loginPage);
    this->loginPage->onLoggedIn = [this](){ this->enterApp(); };
    this->toast = new Toast(this);
    this->setupShortcuts();
    this->root->setCurrentWidget(this->loginPage);
    this->loginPage->playIntro();
}

void MainWindow::say(const QString& text, bool isError){
    this->toast->showMessage(text, isError, 118);
}
bool MainWindow::isTyping() const{
    QWidget* f = QApplication::focusWidget();
    return qobject_cast<QLineEdit*>(f) != nullptr || qobject_cast<QAbstractSpinBox*>(f) != nullptr || qobject_cast<QComboBox*>(f) != nullptr;
}

void MainWindow::setupShortcuts(){
    auto add = [this](const QString& keys, Qt::ShortcutContext ctx, const std::function<void()>& fn){
        QShortcut* s = new QShortcut(QKeySequence(keys), this);
        s->setContext(ctx);
        QObject::connect(s, &QShortcut::activated, this, [fn](){ fn(); });
    };
    // Ctrl+L: đăng xuất NGAY (kể cả khi đang mở hộp thoại hay menu)
    add("Ctrl+L", Qt::ApplicationShortcut, [this](){ this->doLogout(); });
    add("Space", Qt::WindowShortcut, [this](){
        if (this->shell != nullptr && this->player != nullptr && !this->isTyping()) this->player->togglePlay();
    });
    add("Ctrl+Right", Qt::WindowShortcut, [this](){ if (this->player != nullptr) this->player->next(); });
    add("Ctrl+Left", Qt::WindowShortcut, [this](){ if (this->player != nullptr) this->player->previous(); });
    add("Ctrl+S", Qt::WindowShortcut, [this](){ if (this->player != nullptr) this->player->toggleShuffle(); });
    add("Ctrl+R", Qt::WindowShortcut, [this](){ if (this->player != nullptr) this->player->cycleRepeat(); });
    add("Ctrl+F", Qt::WindowShortcut, [this](){
        if (this->searchEdit != nullptr){
            this->searchEdit->setFocus();
            this->searchEdit->selectAll();
        }
    });
    add("Alt+Left", Qt::WindowShortcut, [this](){ this->goBack(); });
    add("Alt+Right", Qt::WindowShortcut, [this](){ this->goForward(); });
}

// ===================================================================== đăng nhập / đăng xuất
void MainWindow::enterApp(){
    this->buildShell();
    this->root->addWidget(this->shell);
    this->root->setCurrentWidget(this->shell);
    fadeIn(this->shell, 450);
    this->navigate("home", [this](){ return this->pageHome(); });
    this->say("Xin chào, " + theme::qs(this->ms.currentUsername()) + "!");
}

void MainWindow::doLogout(){
    if (!this->ms.isLoggedIn() || this->shell == nullptr) return;
    if (QWidget* modal = QApplication::activeModalWidget()) modal->close();     // đóng hộp thoại đang mở
    if (QWidget* popup = QApplication::activePopupWidget()) popup->close();     // đóng menu đang mở
    this->player->clear();
    this->ms.logout();
    QWidget* old = this->shell;
    this->shell = nullptr;
    this->libraryLayout = nullptr;
    this->searchEdit = nullptr;
    this->searchTimer = nullptr;
    this->backBtn = nullptr;
    this->fwdBtn = nullptr;
    this->userBtn = nullptr;
    this->pages = nullptr;
    this->player = nullptr;
    this->queuePanel = nullptr;
    this->queueLayout = nullptr;
    this->queueOpen = false;
    this->rows.clear();
    this->sideItems.clear();
    this->history.clear();
    this->historyPos = -1;
    this->root->setCurrentWidget(this->loginPage);
    this->root->removeWidget(old);
    old->deleteLater();
    this->loginPage->reset();
    fadeIn(this->loginPage, 350);
    this->loginPage->playIntro();
    this->setWindowTitle("Melodia");
    this->toast->showMessage("Da dang xuat", false, 70);
}

// ===================================================================== dựng khung giao diện
void MainWindow::buildShell(){
    this->shell = new QWidget;
    this->shell->setAutoFillBackground(true);
    QPalette pal = this->shell->palette();
    pal.setColor(QPalette::Window, QColor(0, 0, 0));
    this->shell->setPalette(pal);

    QVBoxLayout* outer = new QVBoxLayout(this->shell);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    QWidget* body = new QWidget(this->shell);
    QHBoxLayout* bl = new QHBoxLayout(body);
    bl->setContentsMargins(8, 8, 8, 0);
    bl->setSpacing(8);

    QWidget* side = new QWidget(body);
    side->setFixedWidth(300);
    QVBoxLayout* sl = new QVBoxLayout(side);
    sl->setContentsMargins(0, 0, 0, 0);
    sl->setSpacing(8);
    this->buildSidebar(sl);
    bl->addWidget(side);

    QFrame* center = new QFrame(body);
    center->setProperty("panel", true);
    QVBoxLayout* cl = new QVBoxLayout(center);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(0);
    cl->addWidget(this->buildTopBar());
    this->pages = new FadeStack(center);
    cl->addWidget(this->pages, 1);
    bl->addWidget(center, 1);

    this->queuePanel = new QFrame(body);
    this->queuePanel->setProperty("panel", true);
    this->queuePanel->setMinimumWidth(0);
    this->queuePanel->setMaximumWidth(0);
    QVBoxLayout* ql = new QVBoxLayout(this->queuePanel);
    ql->setContentsMargins(0, 0, 0, 0);
    QScrollArea* qs = new QScrollArea(this->queuePanel);
    qs->setWidgetResizable(true);
    qs->setFrameShape(QFrame::NoFrame);
    qs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* qc = new QWidget;
    qc->setMinimumWidth(320);
    this->queueLayout = new QVBoxLayout(qc);
    this->queueLayout->setContentsMargins(12, 14, 12, 14);
    this->queueLayout->setSpacing(4);
    qs->setWidget(qc);
    ql->addWidget(qs);
    bl->addWidget(this->queuePanel);
    outer->addWidget(body, 1);

    this->player = new PlayerBar(this->ms, this->shell);
    this->player->onTrackChanged = [this](){ this->onTrackChanged(); };
    this->player->onQueueToggle = [this](){ this->toggleQueue(); };
    this->player->onOpenArtist = [this](const std::string& id){ this->openArtist(id); };
    this->player->onOpenAlbum = [this](const std::string& id){ this->openAlbum(id); };
    this->player->onMessage = [this](const QString& t, bool err){ this->say(t, err); };
    this->player->onLikeToggled = [this](const std::string& id, bool liked){
        for (auto& r : this->rows){
            if (r && r->songId() == id) r->setLiked(liked);
        }
        this->rebuildLibrary();
    };
    outer->addWidget(this->player);
}

void MainWindow::buildSidebar(QVBoxLayout* sideLayout){
    // --- khung điều hướng ---
    QFrame* nav = new QFrame;
    nav->setProperty("panel", true);
    QVBoxLayout* nl = new QVBoxLayout(nav);
    nl->setContentsMargins(8, 12, 8, 10);
    nl->setSpacing(2);
    QHBoxLayout* brand = new QHBoxLayout;
    brand->setContentsMargins(10, 0, 0, 8);
    brand->setSpacing(10);
    CoverWidget* logo = new CoverWidget(34, 17);
    logo->setCircular(true);
    logo->setSpecial(Icon::Note, theme::kAccentHover, QColor(14, 120, 56));
    brand->addWidget(logo);
    brand->addWidget(makeLabel("Melodia", 21, true));
    brand->addStretch();
    nl->addLayout(brand);

    auto addNav = [this, nl](const QString& key, Icon icon, const QString& text, const std::function<void()>& go){
        SideItem* item = new SideItem(icon, text);
        this->sideItems.push_back(std::make_pair(key, QPointer<SideItem>(item)));
        nl->addWidget(item);
        QObject::connect(item, &QAbstractButton::clicked, this, [go](){ go(); });
    };
    addNav("home", Icon::Home, "Trang chu", [this](){ this->navigate("home", [this](){ return this->pageHome(); }); });
    addNav("search", Icon::Search, "Tim kiem", [this](){
        this->searchEdit->setFocus();
        this->openSearch(this->searchEdit->text());
    });
    addNav("songs", Icon::Note, "Bai hat", [this](){ this->navigate("songs", [this](){ return this->pageSongs(); }); });
    addNav("artists", Icon::Mic, "Nghe si", [this](){ this->navigate("artists", [this](){ return this->pageArtists(); }); });
    addNav("albums", Icon::Album, "Album", [this](){ this->navigate("albums", [this](){ return this->pageAlbums(); }); });
    if (this->ms.isAdmin()){
        addNav("admin", Icon::Shield, "Quan tri", [this](){ this->navigate("admin", [this](){ return this->pageAdmin(); }); });
    }
    sideLayout->addWidget(nav);

    // --- khung thư viện ---
    QFrame* lib = new QFrame;
    lib->setProperty("panel", true);
    QVBoxLayout* ll = new QVBoxLayout(lib);
    ll->setContentsMargins(8, 12, 8, 8);
    ll->setSpacing(6);
    QHBoxLayout* head = new QHBoxLayout;
    head->setContentsMargins(10, 0, 4, 0);
    IconButton* libIcon = new IconButton(Icon::Library, 22);
    libIcon->setEnabled(false);
    libIcon->setColors(theme::kSubText, theme::kSubText, theme::kSubText);
    head->addWidget(libIcon);
    head->addWidget(makeLabel("Thu vien cua ban", 15, true, theme::kSubText));
    head->addStretch();
    if (!this->ms.isAdmin()){
        IconButton* addBtn = new IconButton(Icon::Plus, 20);
        addBtn->setToolTip("Tao playlist moi");
        QObject::connect(addBtn, &QAbstractButton::clicked, this, [this](){
            QString name;
            if (!textPromptDialog(this, "Tao playlist", "Ten playlist", name, "Playlist cua toi")) return;
            std::string id = this->ms.createPlaylist(theme::ss(name));
            if (id.empty()){
                this->say("Khong tao duoc playlist", true);
                return;
            }
            this->rebuildLibrary();
            this->openPlaylist(id);
            this->say("Da tao playlist \"" + name + "\"");
        });
        head->addWidget(addBtn);
    }
    ll->addLayout(head);
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* content = new QWidget;
    this->libraryLayout = new QVBoxLayout(content);
    this->libraryLayout->setContentsMargins(0, 0, 0, 0);
    this->libraryLayout->setSpacing(2);
    scroll->setWidget(content);
    ll->addWidget(scroll, 1);
    sideLayout->addWidget(lib, 1);
    this->rebuildLibrary();
}

QWidget* MainWindow::buildTopBar(){
    QWidget* bar = new QWidget;
    bar->setFixedHeight(64);
    QHBoxLayout* h = new QHBoxLayout(bar);
    h->setContentsMargins(16, 10, 18, 6);
    h->setSpacing(10);
    this->backBtn = new IconButton(Icon::Back, 20);
    this->backBtn->setCircle(QColor(0, 0, 0, 140), 34);
    this->backBtn->setColors(theme::kText, theme::kText, theme::kText);
    this->backBtn->setToolTip("Quay lai");
    this->fwdBtn = new IconButton(Icon::Forward, 20);
    this->fwdBtn->setCircle(QColor(0, 0, 0, 140), 34);
    this->fwdBtn->setColors(theme::kText, theme::kText, theme::kText);
    this->fwdBtn->setToolTip("Tien toi");
    QObject::connect(this->backBtn, &QAbstractButton::clicked, this, [this](){ this->goBack(); });
    QObject::connect(this->fwdBtn, &QAbstractButton::clicked, this, [this](){ this->goForward(); });
    h->addWidget(this->backBtn);
    h->addWidget(this->fwdBtn);
    h->addSpacing(10);

    this->searchEdit = new QLineEdit;
    this->searchEdit->setProperty("search", true);
    this->searchEdit->setPlaceholderText("Ban muon nghe gi? (Ctrl+F)");
    this->searchEdit->setFixedSize(440, 42);
    this->searchEdit->setClearButtonEnabled(true);
    IconButton* si = new IconButton(Icon::Search, 18, this->searchEdit);
    si->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    si->setColors(theme::kSubText, theme::kSubText, theme::kSubText);
    si->move(8, (42 - si->height()) / 2);
    h->addWidget(this->searchEdit);
    h->addStretch();

    this->userBtn = makeButton(theme::qs(this->ms.currentUsername()) + (this->ms.isAdmin() ? "  •  Admin" : ""), "soft");
    this->userBtn->setIcon(QIcon());
    this->userBtn->setMinimumHeight(38);
    QObject::connect(this->userBtn, &QAbstractButton::clicked, this, [this](){ this->showUserMenu(); });
    h->addWidget(this->userBtn);

    this->searchTimer = new QTimer(this);
    this->searchTimer->setSingleShot(true);
    this->searchTimer->setInterval(280);
    QObject::connect(this->searchTimer, &QTimer::timeout, this, [this](){
        if (this->searchEdit != nullptr) this->openSearch(this->searchEdit->text());
    });
    QObject::connect(this->searchEdit, &QLineEdit::textChanged, this, [this](const QString&){
        if (this->searchTimer != nullptr) this->searchTimer->start();
    });
    return bar;
}

void MainWindow::showUserMenu(){
    QMenu menu(this);
    QAction* name = menu.addAction(theme::qs(this->ms.currentUsername()) + (this->ms.isAdmin() ? "  •  Admin" : "  •  Listener"));
    name->setEnabled(false);
    QAction* mail = menu.addAction(theme::qs(this->ms.currentEmail()));
    mail->setEnabled(false);
    menu.addSeparator();
    QAction* pw = menu.addAction("Doi mat khau…");
    QAction* out = menu.addAction("Dang xuat\tCtrl+L");
    QObject::connect(pw, &QAction::triggered, this, [this](){
        if (passwordDialog(this, this->ms)) this->say("Da doi mat khau");
    });
    QObject::connect(out, &QAction::triggered, this, [this](){ this->doLogout(); });
    menu.exec(this->userBtn->mapToGlobal(QPoint(this->userBtn->width() - 200, this->userBtn->height() + 6)));
}

// ===================================================================== thanh bên
void MainWindow::rebuildSidebar(){
    this->rebuildLibrary();
}
void MainWindow::setActiveKey(const QString& key){
    for (auto& it : this->sideItems){
        if (it.second) it.second->setActive(it.first == key);
    }
}
void MainWindow::rebuildLibrary(){
    if (this->libraryLayout == nullptr) return;
    // bỏ các mục thư viện cũ khỏi danh sách theo dõi
    std::vector<std::pair<QString, QPointer<SideItem>>> keep;
    for (auto& it : this->sideItems){
        if (it.first == "liked" || it.first.startsWith("pl:")) continue;
        keep.push_back(it);
    }
    this->sideItems = keep;
    clearLayout(this->libraryLayout);
    if (this->ms.isAdmin()){
        QLabel* hint = makeLabel("Tai khoan Admin khong co thu vien ca nhan. Hay quan ly nhac trong muc Quan tri.", 13, false, theme::kSubText);
        hint->setWordWrap(true);
        hint->setContentsMargins(10, 8, 10, 0);
        this->libraryLayout->addWidget(hint);
        this->libraryLayout->addStretch();
        return;
    }
    SideItem* liked = new SideItem("liked", "Bai hat da thich", QString("Danh sach • %1 bai hat").arg((int)this->ms.likedSongs().size()));
    liked->setSpecialCover(Icon::HeartFilled, QColor(88, 62, 214), QColor(160, 185, 255));
    QObject::connect(liked, &QAbstractButton::clicked, this, [this](){ this->navigate("liked", [this](){ return this->pageLiked(); }); });
    this->sideItems.push_back(std::make_pair(QString("liked"), QPointer<SideItem>(liked)));
    this->libraryLayout->addWidget(liked);
    for (const PlaylistInfo& pl : this->ms.myPlaylists()){
        SideItem* item = new SideItem(theme::qs(pl.id), theme::qs(pl.name), QString("Playlist • %1 bai hat").arg(pl.songCount));
        std::string id = pl.id;
        QObject::connect(item, &QAbstractButton::clicked, this, [this, id](){ this->openPlaylist(id); });
        this->sideItems.push_back(std::make_pair("pl:" + theme::qs(pl.id), QPointer<SideItem>(item)));
        this->libraryLayout->addWidget(item);
    }
    this->libraryLayout->addStretch();
    if (this->historyPos >= 0) this->setActiveKey(this->history[this->historyPos].key);
}

// ===================================================================== điều hướng
void MainWindow::navigate(const QString& key, const std::function<QWidget*()>& build){
    if (this->pages == nullptr) return;
    if (this->historyPos >= 0 && this->historyPos < (int)this->history.size() - 1){
        this->history.erase(this->history.begin() + this->historyPos + 1, this->history.end());
    }
    PageEntry e;
    e.key = key;
    e.build = build;
    this->history.push_back(e);
    this->historyPos = (int)this->history.size() - 1;
    this->showCurrent(true);
}
void MainWindow::showCurrent(bool animate){
    if (this->pages == nullptr || this->historyPos < 0) return;
    this->rows.clear();
    QWidget* page = this->history[this->historyPos].build();
    this->pages->showPage(page, animate);
    this->setActiveKey(this->history[this->historyPos].key);
    this->backBtn->setEnabled(this->historyPos > 0);
    this->fwdBtn->setEnabled(this->historyPos < (int)this->history.size() - 1);
    this->applyCurrentHighlight();
}
void MainWindow::refreshPage(){
    this->showCurrent(false);
}
void MainWindow::goBack(){
    if (this->pages == nullptr || this->historyPos <= 0) return;
    this->historyPos--;
    this->showCurrent(true);
}
void MainWindow::goForward(){
    if (this->pages == nullptr || this->historyPos >= (int)this->history.size() - 1) return;
    this->historyPos++;
    this->showCurrent(true);
}
void MainWindow::openSearch(const QString& text){
    if (this->pages == nullptr) return;
    QString t = text.trimmed();
    std::function<QWidget*()> build = [this, t](){ return this->pageSearch(t); };
    if (this->historyPos >= 0 && this->history[this->historyPos].key == "search"){
        this->history[this->historyPos].build = build;   // đang ở trang tìm kiếm: cập nhật tại chỗ, không thêm lịch sử
        this->showCurrent(false);
    } else{
        this->navigate("search", build);
    }
}
void MainWindow::openArtist(const std::string& id){
    this->navigate("artist:" + theme::qs(id), [this, id](){ return this->pageArtist(id); });
}
void MainWindow::openAlbum(const std::string& id){
    this->navigate("album:" + theme::qs(id), [this, id](){ return this->pageAlbum(id); });
}
void MainWindow::openGenre(const std::string& name){
    this->navigate("genre:" + theme::qs(name), [this, name](){ return this->pageGenre(name); });
}
void MainWindow::openPlaylist(const std::string& id){
    this->navigate("pl:" + theme::qs(id), [this, id](){ return this->pagePlaylist(id); });
}

// ===================================================================== thành phần dùng chung
QWidget* MainWindow::emptyState(const QString& title, const QString& text){
    QWidget* w = new QWidget;
    QVBoxLayout* v = new QVBoxLayout(w);
    v->setContentsMargins(0, 40, 0, 40);
    v->setSpacing(8);
    QLabel* t = makeLabel(title, 22, true);
    t->setAlignment(Qt::AlignCenter);
    QLabel* d = makeLabel(text, 14, false, theme::kSubText);
    d->setAlignment(Qt::AlignCenter);
    d->setWordWrap(true);
    v->addWidget(t);
    v->addWidget(d);
    return w;
}
QWidget* MainWindow::emptyPage(const QString& title, const QString& text){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::kHover, lay);
    lay->addWidget(this->emptyState(title, text));
    lay->addStretch();
    return page;
}
QWidget* MainWindow::makeHeader(CoverWidget* cover, const QString& kind, const QString& title, const QString& meta){
    QWidget* w = new QWidget;
    QHBoxLayout* h = new QHBoxLayout(w);
    h->setContentsMargins(0, 8, 0, 8);
    h->setSpacing(26);
    h->addWidget(cover, 0, Qt::AlignBottom);
    QVBoxLayout* v = new QVBoxLayout;
    v->setSpacing(8);
    v->addStretch();
    v->addWidget(makeLabel(kind.toUpper(), 12, true));
    QLabel* t = makeLabel(title, title.size() > 22 ? 36 : 54, true);
    t->setWordWrap(true);
    v->addWidget(t);
    QLabel* m = makeLabel(meta, 14, false, theme::kSubText);
    m->setWordWrap(true);
    v->addWidget(m);
    h->addLayout(v, 1);
    return w;
}
QHBoxLayout* MainWindow::addActionBar(QVBoxLayout* lay, const std::function<void(bool)>& play){
    QHBoxLayout* bar = new QHBoxLayout;
    bar->setContentsMargins(0, 10, 0, 6);
    bar->setSpacing(18);
    IconButton* playBtn = new IconButton(Icon::Play, 24);
    playBtn->setColors(QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0));
    playBtn->setCircle(theme::kAccent, 58);
    playBtn->setToolTip("Phat");
    IconButton* shuffleBtn = new IconButton(Icon::Shuffle, 30);
    shuffleBtn->setColors(theme::kSubText, theme::kText, theme::kText);
    shuffleBtn->setToolTip("Phat ngau nhien");
    QObject::connect(playBtn, &QAbstractButton::clicked, this, [play](){ play(false); });
    QObject::connect(shuffleBtn, &QAbstractButton::clicked, this, [play](){ play(true); });
    bar->addWidget(playBtn);
    bar->addWidget(shuffleBtn);
    lay->addLayout(bar);
    return bar;
}
void MainWindow::addSection(QVBoxLayout* lay, const QString& title, const std::function<void()>& showAll){
    QHBoxLayout* h = new QHBoxLayout;
    h->setContentsMargins(0, 14, 0, 0);
    h->addWidget(makeLabel(title, 24, true));
    h->addStretch();
    if (showAll){
        QPushButton* all = makeButton("Hien tat ca", "link");
        QObject::connect(all, &QAbstractButton::clicked, this, [showAll](){ showAll(); });
        h->addWidget(all);
    }
    lay->addLayout(h);
}
void MainWindow::addSongList(QVBoxLayout* lay, const std::vector<SongInfo>& songs, bool showAlbum, const QString& playlistId){
    std::shared_ptr<std::vector<SongInfo>> list = std::make_shared<std::vector<SongInfo>>(songs);
    // hàng tiêu đề cột
    QWidget* head = new QWidget;
    QHBoxLayout* hl = new QHBoxLayout(head);
    hl->setContentsMargins(12, 6, 12, 6);
    hl->setSpacing(12);
    QLabel* c0 = makeLabel("#", 13, true, theme::kSubText);
    c0->setFixedWidth(32);
    c0->setAlignment(Qt::AlignCenter);
    QLabel* c1 = makeLabel("TIEU DE", 12, true, theme::kSubText);
    c1->setContentsMargins(56, 0, 0, 0);
    QLabel* c2 = makeLabel("ALBUM", 12, true, theme::kSubText);
    QLabel* c3 = makeLabel("THOI LUONG", 12, true, theme::kSubText);
    c3->setFixedWidth(88);
    c3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hl->addWidget(c0);
    hl->addWidget(c1, 3);
    if (showAlbum) hl->addWidget(c2, 2);
    else hl->addStretch(2);
    hl->addWidget(c3);
    hl->addSpacing(30);
    lay->addWidget(head);
    QFrame* line = new QFrame;
    line->setFixedHeight(1);
    line->setStyleSheet("background:#2a2a2a;");
    lay->addWidget(line);

    int number = 1;
    for (const SongInfo& s : songs){
        SongRow* r = new SongRow(s, number++, showAlbum, true);
        r->onPlay = [this, list](const SongInfo& song){ this->playSongs(*list, song.id, false); };
        r->onToggleLike = [this](const SongInfo& song){ return this->toggleLikeFor(song.id); };
        r->onMore = [this, playlistId](const SongInfo& song, const QPoint& pos){ this->songMenu(song, pos, playlistId); };
        r->onOpenArtist = [this](const std::string& id){ this->openArtist(id); };
        r->onOpenAlbum = [this](const std::string& id){ this->openAlbum(id); };
        this->rows.push_back(QPointer<SongRow>(r));
        lay->addWidget(r);
    }
}
Card* MainWindow::songCard(const SongInfo& s, const std::shared_ptr<std::vector<SongInfo>>& context){
    Card* c = new Card(theme::qs(s.id), theme::qs(s.name), theme::qs(s.artist), false);
    std::string id = s.id;
    c->onClick = [this, context, id](){ this->playSongs(*context, id, false); };
    c->onPlay = c->onClick;
    return c;
}
Card* MainWindow::artistCard(const ArtistInfo& a){
    Card* c = new Card(theme::qs(a.id), theme::qs(a.name), QString("Nghe si • %1 nguoi theo doi").arg(a.followers), true);
    std::string id = a.id;
    c->onClick = [this, id](){ this->openArtist(id); };
    c->onPlay = [this, id](){ this->playSongs(this->ms.songsOfArtist(id), "", false); };
    return c;
}
Card* MainWindow::albumCard(const AlbumInfo& al){
    Card* c = new Card(theme::qs(al.id), theme::qs(al.name), theme::qs(al.date).left(4) + " • " + theme::qs(al.artist), false);
    std::string id = al.id;
    c->onClick = [this, id](){ this->openAlbum(id); };
    c->onPlay = [this, id](){ this->playSongs(this->ms.songsOfAlbum(id), "", false); };
    return c;
}

void MainWindow::songMenu(const SongInfo& s, const QPoint& pos, const QString& playlistId){
    QMenu menu(this);
    std::string id = s.id;
    std::string idArtist = s.idArtist;
    std::string idAlbum = s.idAlbum;
    auto add = [this, &menu](QMenu* m, const QString& text, const std::function<void()>& fn){
        QAction* a = (m != nullptr ? m : &menu)->addAction(text);
        QObject::connect(a, &QAction::triggered, this, [fn](){ fn(); });
        return a;
    };
    add(nullptr, "Phat bai nay", [this, s](){
        std::vector<SongInfo> one(1, s);
        this->playSongs(one, s.id, false);
    });
    add(nullptr, "Phat tiep theo", [this, id](){
        if (this->ms.playNext(id)){
            this->say("Se phat ngay sau bai hien tai");
            if (this->queueOpen) this->refreshQueuePanel();
        } else{
            this->say("Hay phat mot bai truoc", true);
        }
    });
    add(nullptr, "Them vao hang cho", [this, id](){
        if (this->ms.addToQueue(id)){
            this->say("Da them vao hang cho");
            if (this->queueOpen) this->refreshQueuePanel();
        } else{
            this->say("Hay phat mot bai truoc, hoặc bai nay da co trong hang cho", true);
        }
    });
    menu.addSeparator();
    if (!this->ms.isAdmin()){
        QMenu* sub = menu.addMenu("Them vao playlist");
        for (const PlaylistInfo& pl : this->ms.myPlaylists()){
            bool has = this->ms.playlistContains(pl.id, id);
            QAction* a = add(sub, theme::qs(pl.name) + (has ? "  ✓" : ""), [this, pl, id](){
                if (this->ms.addToPlaylist(pl.id, id)){
                    this->say("Da them vao \"" + theme::qs(pl.name) + "\"");
                    this->rebuildLibrary();
                } else{
                    this->say("Bai hat da co trong playlist nay", true);
                }
            });
            a->setEnabled(!has);
        }
        sub->addSeparator();
        add(sub, "＋ Playlist moi…", [this, id](){
            QString name;
            if (!textPromptDialog(this, "Tao playlist", "Ten playlist", name, "Playlist cua toi")) return;
            std::string plId = this->ms.createPlaylist(theme::ss(name));
            if (plId.empty()){
                this->say("Khong tao duoc playlist", true);
                return;
            }
            this->ms.addToPlaylist(plId, id);
            this->rebuildLibrary();
            this->say("Da tao \"" + name + "\" và them bai hat");
        });
        add(nullptr, s.liked ? "Bo khoi Bai hat da thich" : "Them vao Bai hat da thich", [this, id](){ this->toggleLikeFor(id); });
        if (!playlistId.isEmpty()){
            std::string plId = theme::ss(playlistId);
            add(nullptr, "Xoa khoi playlist nay", [this, plId, id](){
                if (this->ms.removeFromPlaylist(plId, id)){
                    this->rebuildLibrary();
                    this->refreshPage();
                    this->say("Da xoa khoi playlist");
                }
            });
        }
        menu.addSeparator();
    }
    add(nullptr, "Chuyen toi nghe si", [this, idArtist](){ this->openArtist(idArtist); });
    if (!idAlbum.empty()) add(nullptr, "Chuyen toi album", [this, idAlbum](){ this->openAlbum(idAlbum); });
    menu.exec(pos);
}

// ===================================================================== phát nhạc, hàng chờ
void MainWindow::playSongs(const std::vector<SongInfo>& songs, const std::string& startId, bool shuffle){
    if (songs.empty()){
        this->say("Danh sach dang trong", true);
        return;
    }
    std::vector<std::string> ids;
    for (const SongInfo& s : songs) ids.push_back(s.id);
    std::string start = startId;
    if (start.empty()){
        start = shuffle ? ids[QRandomGenerator::global()->bounded((int)ids.size())] : ids.front();
    }
    if (shuffle) this->player->setShuffle(true);
    SongInfo out;
    if (this->ms.startQueue(ids, start, out)) this->player->playTrack(out);
}
bool MainWindow::toggleLikeFor(const std::string& idSong){
    if (this->ms.isAdmin()){
        this->say("Tai khoan Admin khong co danh sach yeu thich", true);
        return false;
    }
    bool liked = this->ms.toggleLike(idSong);
    for (auto& r : this->rows){
        if (r && r->songId() == idSong) r->setLiked(liked);
    }
    this->player->syncLiked(idSong, liked);
    this->rebuildLibrary();
    this->say(liked ? "Da them vao Bai hat da thich" : "Da go khoi Bai hat da thich");
    return liked;
}
void MainWindow::applyCurrentHighlight(){
    if (this->player == nullptr) return;
    std::string id = this->player->currentId();
    bool playing = this->player->isPlaying();
    for (auto& r : this->rows){
        if (r) r->setCurrent(!id.empty() && r->songId() == id, playing);
    }
}
void MainWindow::onTrackChanged(){
    this->applyCurrentHighlight();
    if (this->queueOpen) this->refreshQueuePanel();
    SongInfo cur;
    if (this->player != nullptr && this->player->hasCurrent() && this->ms.currentTrack(cur))
        this->setWindowTitle(theme::qs(cur.name) + " • " + theme::qs(cur.artist));
    else
        this->setWindowTitle("Melodia");
}
void MainWindow::toggleQueue(){
    if (this->queuePanel == nullptr) return;
    this->queueOpen = !this->queueOpen;
    this->player->setQueueActive(this->queueOpen);
    if (this->queueOpen) this->refreshQueuePanel();
    QPropertyAnimation* a = new QPropertyAnimation(this->queuePanel, "maximumWidth", this);
    a->setDuration(280);
    a->setEasingCurve(QEasingCurve::OutCubic);
    a->setStartValue(this->queuePanel->maximumWidth());
    a->setEndValue(this->queueOpen ? 340 : 0);
    a->start(QAbstractAnimation::DeleteWhenStopped);
}
void MainWindow::refreshQueuePanel(){
    if (this->queueLayout == nullptr) return;
    clearLayout(this->queueLayout);
    QHBoxLayout* head = new QHBoxLayout;
    head->setContentsMargins(6, 0, 0, 8);
    head->addWidget(makeLabel("Danh sach cho", 20, true));
    head->addStretch();
    IconButton* close = new IconButton(Icon::Close, 18);
    QObject::connect(close, &QAbstractButton::clicked, this, [this](){ this->toggleQueue(); });
    head->addWidget(close);
    this->queueLayout->addLayout(head);

    SongInfo cur;
    if (!this->ms.currentTrack(cur)){
        this->queueLayout->addWidget(this->emptyState("Chua phat bai nao", "Chon mot bai hat de bat dau."));
        this->queueLayout->addStretch();
        return;
    }
    auto wire = [this](SongRow* r, const std::shared_ptr<std::vector<SongInfo>>& list){
        r->onPlay = [this, list](const SongInfo& s){ this->playSongs(*list, s.id, false); };
        r->onToggleLike = [this](const SongInfo& s){ return this->toggleLikeFor(s.id); };
        r->onMore = [this](const SongInfo& s, const QPoint& pos){ this->songMenu(s, pos, QString()); };
        r->onOpenArtist = [this](const std::string& id){ this->openArtist(id); };
        r->onOpenAlbum = [this](const std::string& id){ this->openAlbum(id); };
        this->rows.push_back(QPointer<SongRow>(r));
    };
    this->queueLayout->addWidget(makeLabel("Dang phat", 14, true, theme::kSubText));
    std::shared_ptr<std::vector<SongInfo>> nowList = std::make_shared<std::vector<SongInfo>>(1, cur);
    SongRow* now = new SongRow(cur, 0, false, true);
    wire(now, nowList);
    this->queueLayout->addWidget(now);
    std::vector<SongInfo> up = this->ms.upcoming();
    this->queueLayout->addSpacing(8);
    this->queueLayout->addWidget(makeLabel("Tiep theo", 14, true, theme::kSubText));
    if (up.empty()){
        QLabel* none = makeLabel("Khong con bai nao trong hang cho.", 13, false, theme::kSubText);
        none->setContentsMargins(6, 6, 0, 0);
        this->queueLayout->addWidget(none);
    } else{
        std::shared_ptr<std::vector<SongInfo>> upList = std::make_shared<std::vector<SongInfo>>(up);
        int shown = 0;
        for (const SongInfo& s : up){
            if (++shown > 40) break;
            SongRow* r = new SongRow(s, 0, false, true);
            wire(r, upList);
            this->queueLayout->addWidget(r);
        }
    }
    this->queueLayout->addStretch();
    this->applyCurrentHighlight();
}

// ===================================================================== các trang
QWidget* MainWindow::pageHome(){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::coverColor(theme::qs(this->ms.currentUsername())).darker(240), lay);
    int hour = QTime::currentTime().hour();
    QString greeting = hour < 11 ? "Chao buoi sang" : hour < 14 ? "Chao buoi trua" : hour < 18 ? "Chao buoi chieu" : "Chao buoi toi";
    lay->addWidget(makeLabel(greeting + ", " + theme::qs(this->ms.currentUsername()), 34, true));

    // --- Ô truy cập nhanh ---
    std::vector<QuickTile*> tiles;
    if (!this->ms.isAdmin()){
        QuickTile* liked = new QuickTile("liked", "Bai hat da thich");
        liked->setSpecial(Icon::HeartFilled, QColor(88, 62, 214), QColor(160, 185, 255));
        liked->onClick = [this](){ this->navigate("liked", [this](){ return this->pageLiked(); }); };
        liked->onPlay = [this](){ this->playSongs(this->ms.likedSongs(), "", false); };
        tiles.push_back(liked);
        for (const PlaylistInfo& pl : this->ms.myPlaylists()){
            if (tiles.size() >= 6) break;
            QuickTile* t = new QuickTile(theme::qs(pl.id), theme::qs(pl.name));
            std::string id = pl.id;
            t->onClick = [this, id](){ this->openPlaylist(id); };
            t->onPlay = [this, id](){ this->playSongs(this->ms.playlistSongs(id), "", false); };
            tiles.push_back(t);
        }
    } else{
        QuickTile* a = new QuickTile("admin", "Quan tri he thong");
        a->setSpecial(Icon::Shield, QColor(29, 185, 84), QColor(14, 100, 50));
        a->onClick = [this](){ this->navigate("admin", [this](){ return this->pageAdmin(); }); };
        tiles.push_back(a);
    }
    if (tiles.size() < 6){
        QuickTile* all = new QuickTile("songs", "Tat ca bai hat");
        all->onClick = [this](){ this->navigate("songs", [this](){ return this->pageSongs(); }); };
        all->onPlay = [this](){ this->playSongs(this->ms.allSongs(), "", false); };
        tiles.push_back(all);
    }
    if (tiles.size() < 6){
        QuickTile* ar = new QuickTile("artists", "Nghe si");
        ar->onClick = [this](){ this->navigate("artists", [this](){ return this->pageArtists(); }); };
        tiles.push_back(ar);
    }
    if (tiles.size() < 6){
        QuickTile* al = new QuickTile("albums", "Album");
        al->onClick = [this](){ this->navigate("albums", [this](){ return this->pageAlbums(); }); };
        tiles.push_back(al);
    }
    QGridLayout* grid = new QGridLayout;
    grid->setSpacing(12);
    for (int i = 0; i < (int)tiles.size(); ++i) grid->addWidget(tiles[i], i / 3, i % 3);
    for (int c = 0; c < 3; ++c) grid->setColumnStretch(c, 1);
    lay->addSpacing(8);
    lay->addLayout(grid);

    auto songSection = [this, lay](const QString& title, const std::vector<SongInfo>& v, const std::function<void()>& all){
        if (v.empty()) return;
        this->addSection(lay, title, all);
        std::shared_ptr<std::vector<SongInfo>> ctx = std::make_shared<std::vector<SongInfo>>(v);
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const SongInfo& s : v) flow->addCard(this->songCard(s, ctx));
        lay->addWidget(flow);
    };
    if (this->ms.allSongs().empty()){
        lay->addWidget(this->emptyState("Chua co bai hat nao", this->ms.isAdmin() ? "Hay vao muc Quan tri de them nghe si va bai hat." : "Admin chưa them bai hat vao he thong."));
        lay->addStretch();
        return page;
    }
    if (!this->ms.isAdmin()) songSection("Nghe gan day", this->ms.recentSongs(12), std::function<void()>());
    songSection("Top bài hát", this->ms.topSongs(12), [this](){ this->navigate("songs", [this](){ return this->pageSongs(); }); });
    std::vector<ArtistInfo> topArtists = this->ms.topArtists(12);
    if (!topArtists.empty()){
        this->addSection(lay, "Nghe si noi bat", [this](){ this->navigate("artists", [this](){ return this->pageArtists(); }); });
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const ArtistInfo& a : topArtists) flow->addCard(this->artistCard(a));
        lay->addWidget(flow);
    }
    songSection("Moi phat hanh", this->ms.newestSongs(12), std::function<void()>());
    std::vector<AlbumInfo> albums = this->ms.allAlbums();
    if (!albums.empty()){
        this->addSection(lay, "Album", [this](){ this->navigate("albums", [this](){ return this->pageAlbums(); }); });
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const AlbumInfo& al : albums) flow->addCard(this->albumCard(al));
        lay->addWidget(flow);
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageSearch(const QString& text){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(QColor(40, 40, 60), lay);
    if (text.isEmpty()){
        lay->addWidget(makeLabel("Duyet tat ca", 28, true));
        std::vector<std::string> genres = this->ms.genres();
        if (genres.empty()){
            lay->addWidget(this->emptyState("Chua co the loai nao", "Hay go ten bai hat, nghe si hoac album vao o tim kiem."));
        } else{
            CardFlow* flow = new CardFlow(200, 112, 0);
            for (const std::string& g : genres){
                GenreTile* t = new GenreTile(theme::qs(g));
                std::string name = g;
                t->onClick = [this, name](){ this->openGenre(name); };
                flow->addCard(t);
            }
            lay->addWidget(flow);
        }
        lay->addStretch();
        return page;
    }
    std::string kw = theme::ss(text);
    std::vector<SongInfo> songs = this->ms.searchSongs(kw);
    std::vector<ArtistInfo> artists = this->ms.searchArtists(kw);
    std::vector<AlbumInfo> albums = this->ms.searchAlbums(kw);
    if (songs.empty() && artists.empty() && albums.empty()){
        lay->addWidget(this->emptyState("Khong tim thay ket qua cho \"" + text + "\"", "Hay kiem tra chinh ta hoac thu tu khoa khac."));
        lay->addStretch();
        return page;
    }
    if (!songs.empty()){
        this->addSection(lay, "Bai hat");
        std::vector<SongInfo> top(songs.begin(), songs.begin() + std::min<size_t>(songs.size(), 8));
        this->addSongList(lay, top, true, QString());
    }
    if (!artists.empty()){
        this->addSection(lay, "Nghe si");
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const ArtistInfo& a : artists) flow->addCard(this->artistCard(a));
        lay->addWidget(flow);
    }
    if (!albums.empty()){
        this->addSection(lay, "Album");
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const AlbumInfo& al : albums) flow->addCard(this->albumCard(al));
        lay->addWidget(flow);
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageSongs(){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::kAccent.darker(260), lay);
    std::vector<SongInfo> songs = this->ms.allSongs();
    lay->addWidget(makeLabel("Tat ca bai hat", 34, true));
    lay->addWidget(makeLabel(QString("%1 bai hat").arg((int)songs.size()), 14, false, theme::kSubText));
    if (songs.empty()){
        lay->addWidget(this->emptyState("Chua co bai hat nao", "Admin co the them bai hat trong muc Quan tri."));
    } else{
        QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
        bar->addStretch();
        this->addSongList(lay, songs, true, QString());
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageArtists(){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(QColor(120, 80, 230).darker(250), lay);
    lay->addWidget(makeLabel("Nghe si", 34, true));
    std::vector<ArtistInfo> followed = this->ms.followedArtists();
    if (!followed.empty()){
        this->addSection(lay, "Dang theo doi");
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 0);
        for (const ArtistInfo& a : followed) flow->addCard(this->artistCard(a));
        lay->addWidget(flow);
    }
    std::vector<ArtistInfo> all = this->ms.allArtists();
    this->addSection(lay, "Tat ca nghe si");
    if (all.empty()){
        lay->addWidget(this->emptyState("Chua co nghe si nào", "Admin co the them nghe si trong muc Quan tri"));
    } else{
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 0);
        for (const ArtistInfo& a : all) flow->addCard(this->artistCard(a));
        lay->addWidget(flow);
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageAlbums(){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(QColor(0, 150, 200).darker(260), lay);
    lay->addWidget(makeLabel("Album", 34, true));
    std::vector<AlbumInfo> all = this->ms.allAlbums();
    if (all.empty()){
        lay->addWidget(this->emptyState("Chua co album nao", "Admin co the them album trong muc Quan tri"));
    } else{
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 0);
        for (const AlbumInfo& al : all) flow->addCard(this->albumCard(al));
        lay->addWidget(flow);
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageLiked(){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(QColor(88, 62, 214), lay);
    std::vector<SongInfo> songs = this->ms.likedSongs();
    int total = 0;
    for (const SongInfo& s : songs) total += s.duration;
    CoverWidget* cover = new CoverWidget(200, 8);
    cover->setSpecial(Icon::HeartFilled, QColor(88, 62, 214), QColor(160, 185, 255));
    lay->addWidget(this->makeHeader(cover, "Playlist", "Bai hat da thich",
        theme::qs(this->ms.currentUsername()) + QString(" • %1 bai hat").arg((int)songs.size()) + (songs.empty() ? QString() : " • " + theme::formatLong(total))));
    if (songs.empty()){
        lay->addWidget(this->emptyState("Chua co bai hat nao duoc thich", "Nhan bieu tuong trai tim o bat ky bai hat nao de luu vao day."));
    } else{
        QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
        bar->addStretch();
        this->addSongList(lay, songs, true, QString());
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pagePlaylist(const std::string& id){
    PlaylistInfo info;
    bool found = false;
    for (const PlaylistInfo& p : this->ms.myPlaylists()){
        if (p.id == id){
            info = p;
            found = true;
        }
    }
    if (!found) return this->emptyPage("Khong tim thay playlist", "Playlist nay co the da bi xoa");
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::coverColor(theme::qs(id)).darker(230), lay);
    std::vector<SongInfo> songs = this->ms.playlistSongs(id);
    CoverWidget* cover = new CoverWidget(200, 8);
    cover->setSeed(theme::qs(id), theme::qs(info.name));
    lay->addWidget(this->makeHeader(cover, "Playlist", theme::qs(info.name),
        theme::qs(this->ms.currentUsername()) + QString(" • %1 bai hat").arg(info.songCount) + (songs.empty() ? QString() : " • " + theme::formatLong(info.totalSeconds))));
    QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
    IconButton* edit = new IconButton(Icon::Edit, 26);
    edit->setColors(theme::kSubText, theme::kText, theme::kText);
    edit->setToolTip("Doi ten playlist");
    IconButton* del = new IconButton(Icon::Trash, 26);
    del->setColors(theme::kSubText, theme::kDanger, theme::kDanger);
    del->setToolTip("Xoa playlist");
    QObject::connect(edit, &QAbstractButton::clicked, this, [this, id, info](){
        QString name;
        if (!textPromptDialog(this, "Doi ten playlist", "Ten moi", name, theme::qs(info.name))) return;
        if (this->ms.renamePlaylist(id, theme::ss(name))){
            this->rebuildLibrary();
            this->refreshPage();
            this->say("Da doi ten playlist");
        }
    });
    QObject::connect(del, &QAbstractButton::clicked, this, [this, id, info](){
        if (!confirmDialog(this, "Xoa playlist", "Xoa playlist \"" + theme::qs(info.name) + "\"? Cac bai hat van con trong he thong.")) return;
        if (this->ms.deletePlaylist(id)){
            this->rebuildLibrary();
            this->navigate("home", [this](){ return this->pageHome(); });
            this->say("Da xoa playlist");
        }
    });
    bar->addWidget(edit);
    bar->addWidget(del);
    bar->addStretch();
    if (songs.empty()){
        lay->addWidget(this->emptyState("Playlist đang trong", "Tim bai hat ban thich, nhan ⋯ roi chon \"Them vao playlist\"."));
        QPushButton* find = makeButton("Tim bai hat", "primary");
        QObject::connect(find, &QAbstractButton::clicked, this, [this](){
            this->searchEdit->setFocus();
            this->openSearch(QString());
        });
        lay->addWidget(find, 0, Qt::AlignHCenter);
    } else{
        this->addSongList(lay, songs, true, theme::qs(id));
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageAlbum(const std::string& id){
    AlbumInfo info;
    if (!this->ms.albumInfo(id, info)) return this->emptyPage("Khong tim thay album", "Album nay co the da bi xoa.");
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::coverColor(theme::qs(id)).darker(230), lay);
    std::vector<SongInfo> songs = this->ms.songsOfAlbum(id);
    CoverWidget* cover = new CoverWidget(200, 8);
    cover->setSeed(theme::qs(id), theme::qs(info.name));
    lay->addWidget(this->makeHeader(cover, "Album", theme::qs(info.name),
        theme::qs(info.artist) + " • " + theme::qs(info.date).left(4) + QString(" • %1 bai hat • ").arg(info.trackCount) + theme::formatLong(info.totalSeconds)));
    QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
    QPushButton* artistBtn = makeButton(theme::qs(info.artist), "ghost");
    std::string idArtist = info.idArtist;
    QObject::connect(artistBtn, &QAbstractButton::clicked, this, [this, idArtist](){ this->openArtist(idArtist); });
    bar->addWidget(artistBtn);
    bar->addStretch();
    if (songs.empty()) lay->addWidget(this->emptyState("Album chua co bai hat", "Admin co the them bai hat vao album nay"));
    else this->addSongList(lay, songs, false, QString());
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageArtist(const std::string& id){
    ArtistInfo info;
    if (!this->ms.artistInfo(id, info)) return this->emptyPage("Khong tim thay nghe si", "Nghe si nay co the da bi xoa.");
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::coverColor(theme::qs(id)).darker(220), lay);
    std::vector<SongInfo> songs = this->ms.songsOfArtist(id);
    CoverWidget* cover = new CoverWidget(200, 100);
    cover->setSeed(theme::qs(id), theme::qs(info.name));
    cover->setCircular(true);
    lay->addWidget(this->makeHeader(cover, "Nghe si", theme::qs(info.name),
        QString("%1 nguoi theo doi • %2 bai hat • ").arg(info.followers).arg(info.songCount) + theme::qs(info.country)));
    QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
    if (!this->ms.isAdmin()){
        QPushButton* follow = makeButton(info.followed ? "Dang theo doi" : "Theo doi", "ghost");
        QObject::connect(follow, &QAbstractButton::clicked, this, [this, id](){
            bool now = this->ms.toggleFollow(id);
            this->refreshPage();
            this->rebuildLibrary();
            this->say(now ? "Da theo doi nghe si" : "Da bo theo doi");
        });
        bar->addWidget(follow);
    }
    bar->addStretch();
    if (!songs.empty()){
        this->addSection(lay, "Pho bien");
        this->addSongList(lay, songs, true, QString());
    } else{
        lay->addWidget(this->emptyState("Chua co bai hat", "Nghe si nay chua co bai hat nao."));
    }
    std::vector<AlbumInfo> albums = this->ms.albumsOfArtist(id);
    if (!albums.empty()){
        this->addSection(lay, "Album");
        CardFlow* flow = new CardFlow(Card::cardWidth(), Card::cardHeight(), 1);
        for (const AlbumInfo& al : albums) flow->addCard(this->albumCard(al));
        lay->addWidget(flow);
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageGenre(const std::string& name){
    QVBoxLayout* lay = nullptr;
    QScrollArea* page = makePage(theme::coverColor(theme::qs(name)).darker(200), lay);
    std::vector<SongInfo> songs = this->ms.songsByGenre(name);
    CoverWidget* cover = new CoverWidget(200, 8);
    cover->setSeed(theme::qs(name), theme::qs(name));
    lay->addWidget(this->makeHeader(cover, "The loai", theme::qs(name), QString("%1 bai hat").arg((int)songs.size())));
    if (songs.empty()){
        lay->addWidget(this->emptyState("Chua co bai hat", "The loai nay chua co bai hat nao."));
    } else{
        QHBoxLayout* bar = this->addActionBar(lay, [this, songs](bool sh){ this->playSongs(songs, "", sh); });
        bar->addStretch();
        this->addSongList(lay, songs, true, QString());
    }
    lay->addStretch();
    return page;
}

QWidget* MainWindow::pageAdmin(){
    if (!this->ms.isAdmin()) return this->emptyPage("Khong co quyen truy cap", "Admin moi duoc vao muc nay.");
    AdminPage* admin = new AdminPage(this->ms);
    admin->onMessage = [this](const QString& t, bool err){ this->say(t, err); };
    admin->onChanged = [this](){
        // danh mục nhạc đã đổi: bài đang phát có thể đã bị xóa
        if (this->player != nullptr && this->player->hasCurrent()){
            SongInfo cur;
            if (!this->ms.currentTrack(cur)) this->player->clear();
        }
        this->onTrackChanged();
    };
    return admin;
}