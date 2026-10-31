#pragma once
#include <QApplication>
#include <QFont>
#include <QString>

namespace Theme{

QString styleSheet(){
    return QString::fromUtf8(R"QSS(
* { color: #e8e8e8; font-size: 10pt; }
QWidget { background: #121212; }
QMainWindow, QDialog { background: #121212; }
QLabel { background: transparent; }
QLabel#title { font-size: 22pt; font-weight: 800; color: #1DB954; }
QLabel#sectionTitle { font-size: 12pt; font-weight: 700; color: #ffffff; }
QLabel#nowPlaying { font-size: 13pt; font-weight: 700; color: #ffffff; }
QLabel#error { color: #ff6b6b; }
QLabel#muted { color: #b3b3b3; }

QTabWidget::pane { border: none; }
QTabBar::tab { background: transparent; color: #b3b3b3; padding: 10px 24px; border: none; font-weight: 600; }
QTabBar::tab:hover { color: #ffffff; }
QTabBar::tab:selected { color: #ffffff; border-bottom: 3px solid #1DB954; }

QMenuBar { background: #181818; }
QMenuBar::item { background: transparent; padding: 6px 12px; }
QMenuBar::item:selected { background: #2a2a2a; }
QMenu { background: #242424; border: 1px solid #333333; padding: 4px; }
QMenu::item { padding: 6px 24px; border-radius: 4px; }
QMenu::item:selected { background: #1DB954; color: #000000; }
QStatusBar { background: #181818; color: #b3b3b3; }
QToolTip { background: #242424; color: #ffffff; border: 1px solid #333333; }

QListWidget { background: #181818; border: none; border-radius: 10px; padding: 6px; outline: 0; }
QListWidget::item { padding: 10px 12px; border-radius: 6px; color: #b3b3b3; }
QListWidget::item:hover { background: #242424; color: #ffffff; }
QListWidget::item:selected { background: #2a2a2a; color: #1DB954; font-weight: 600; }

QTableWidget { background: #181818; alternate-background-color: #1c1c1c; border: none; border-radius: 10px;
               gridline-color: transparent; selection-background-color: #24402f; selection-color: #ffffff; outline: 0; }
QTableWidget::item { padding: 6px 8px; border: none; }
QHeaderView { background: #181818; }
QHeaderView::section { background: #181818; color: #b3b3b3; border: none; border-bottom: 1px solid #2a2a2a;
                       padding: 10px 8px; font-weight: 600; }
QTableCornerButton::section { background: #181818; border: none; }

QLineEdit, QSpinBox { background: #242424; border: 1px solid #333333; border-radius: 16px; padding: 8px 14px;
                      selection-background-color: #1DB954; }
QLineEdit:focus, QSpinBox:focus { border: 1px solid #1DB954; }
QLineEdit:disabled { color: #666666; }

QPushButton { background: #2a2a2a; border: none; border-radius: 16px; padding: 8px 18px; font-weight: 600; }
QPushButton:hover { background: #3a3a3a; }
QPushButton:pressed { background: #1a1a1a; }
QPushButton:disabled { background: #1c1c1c; color: #555555; }
QPushButton#primary { background: #1DB954; color: #000000; }
QPushButton#primary:hover { background: #1ed760; }
QPushButton#primary:disabled { background: #1c3a28; color: #4a6b58; }
QPushButton#danger { background: #3a1f1f; color: #ff7b7b; }
QPushButton#danger:hover { background: #4d2828; }
QPushButton#danger:disabled { background: #1c1c1c; color: #555555; }
QPushButton#transport { border-radius: 20px; min-width: 40px; max-width: 40px; min-height: 40px; max-height: 40px;
                        padding: 0; font-size: 11pt; }
QPushButton#playBtn { background: #ffffff; color: #000000; border-radius: 26px; min-width: 52px; max-width: 52px;
                      min-height: 52px; max-height: 52px; padding: 0; font-size: 15pt; }
QPushButton#playBtn:hover { background: #1ed760; }

QSlider::groove:horizontal { height: 5px; background: #4d4d4d; border-radius: 2px; }
QSlider::sub-page:horizontal { background: #1DB954; border-radius: 2px; }
QSlider::handle:horizontal { background: #ffffff; width: 12px; height: 12px; margin: -4px 0; border-radius: 6px; }

QScrollBar:vertical { background: transparent; width: 10px; margin: 0; }
QScrollBar::handle:vertical { background: #444444; border-radius: 5px; min-height: 30px; }
QScrollBar::handle:vertical:hover { background: #666666; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QScrollBar:horizontal { background: transparent; height: 10px; margin: 0; }
QScrollBar::handle:horizontal { background: #444444; border-radius: 5px; min-width: 30px; }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }
)QSS");
}

void apply(QApplication& app){
    app.setStyle("Fusion");
    QFont f("Segoe UI", 10);
    app.setFont(f);
    app.setStyleSheet(styleSheet());
}

}