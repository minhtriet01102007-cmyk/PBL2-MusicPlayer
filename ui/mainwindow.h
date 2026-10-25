#pragma once
#include "musicsystem.h"
#include "logindialog.h"
#include "listenerpage.h"
#include "adminpage.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(MusicSystem* sys, QWidget* parent = nullptr);
private slots:
    void onLogout();
private:
    void buildUi();
    MusicSystem* sys;
    ListenerPage* listener = nullptr;
    AdminPage* admin = nullptr;
};

MainWindow::MainWindow(MusicSystem* s, QWidget* parent) : QMainWindow(parent), sys(s) {
    setWindowTitle("He thong phat va quan ly bai hat");
    resize(1100, 680);

    auto* menu = menuBar()->addMenu("Tai khoan");
    auto* actLogout = menu->addAction("Dang xuat");
    actLogout->setShortcut(QKeySequence("Ctrl+L"));
    auto* actQuit = menu->addAction("Thoat");
    connect(actLogout, &QAction::triggered, this, &MainWindow::onLogout);
    connect(actQuit, &QAction::triggered, this, &QWidget::close);

    buildUi();
}

inline void MainWindow::buildUi() {
    auto* tabs = new QTabWidget;
    listener = new ListenerPage(sys);
    tabs->addTab(listener, "Nghe nhac");

    admin = nullptr;
    if (sys->isAdmin()) {                       // phan quyen: chi Admin thay tab quan ly
        admin = new AdminPage(sys);
        tabs->addTab(admin, "Quan ly bai hat");
        connect(admin, &AdminPage::songsChanged, listener, &ListenerPage::refresh);
    }
    setCentralWidget(tabs);

    statusBar()->showMessage(QString("Xin chao, %1 (%2)")
        .arg(QString::fromStdString(sys->currentUsername()))
        .arg(sys->isAdmin() ? "Admin" : "Listener"));
}

inline void MainWindow::onLogout() {
    setCentralWidget(new QWidget);              // huy giao dien cu (dung phat nhac)
    listener = nullptr;
    admin = nullptr;
    statusBar()->clearMessage();
    sys->logout();

    LoginDialog dlg(sys, this);
    if (dlg.exec() != QDialog::Accepted) { close(); return; }   // huy dang nhap -> thoat app
    buildUi();
}
