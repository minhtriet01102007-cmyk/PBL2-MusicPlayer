#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "music_service.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    MusicService musicService;
    QListWidget *songListView;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QLabel *statusLabel;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadSongsToUI();
    void onPlayClicked();
};