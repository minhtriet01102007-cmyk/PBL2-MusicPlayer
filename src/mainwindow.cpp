#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PBL2 - Music Player System");
    resize(600, 400);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("<h2>Danh Sách Bài Hát (PBL2)</h2>", this);
    mainLayout->addWidget(titleLabel);

    songListView = new QListWidget(this);
    mainLayout->addWidget(songListView);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    playButton = new QPushButton("Play", this);
    pauseButton = new QPushButton("Pause", this);
    btnLayout->addWidget(playButton);
    btnLayout->addWidget(pauseButton);
    mainLayout->addLayout(btnLayout);

    statusLabel = new QLabel("Trạng thái: Sẵn sàng", this);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);

    loadSongsToUI();

    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
}

void MainWindow::loadSongsToUI() {
    std::vector<Song> songs = musicService.getAllSongs();
    for (const auto& song : songs) {
        QString itemText = QString::fromStdString(song.getIdSong() + " - " + song.getName() + " (" + song.getArtist() + ")");
        songListView->addItem(itemText);
    }
    statusLabel->setText(QString("Đã nạp thành công %1 bài hát lên GUI.").arg(songs.size()));
}

void MainWindow::onPlayClicked() {
    QListWidgetItem *selectedItem = songListView->currentItem();
    if (selectedItem) {
        statusLabel->setText("Đang phát: " + selectedItem->text());
    } else {
        statusLabel->setText("Vui lòng chọn một bài hát trong danh sách!");
    }
}

MainWindow::~MainWindow() {}