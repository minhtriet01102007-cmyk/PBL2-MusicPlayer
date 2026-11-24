#pragma once
#include <string>
#include <QColor>
#include <QString>

// Bảng màu, hàm tiện ích và stylesheet (QSS) dùng chung cho toàn bộ giao diện, phong cách tối kiểu Spotify.
namespace theme{
const QColor kBg(18, 18, 18);
const QColor kPanel(24, 24, 24);
const QColor kElevated(32, 32, 32);
const QColor kHover(42, 42, 42);
const QColor kText(255, 255, 255);
const QColor kSubText(179, 179, 179);
const QColor kAccent(29, 185, 84);
const QColor kAccentHover(30, 215, 96);
const QColor kDanger(229, 72, 77);

QString qs(const std::string& s);                           // std::string (UTF-8) -> QString
std::string ss(const QString& s);                           // QString -> std::string (UTF-8)
QString formatTime(int seconds);                            // 3:05 hoặc 1:02:03
QString formatLong(int seconds);                            // "1 giờ 5 phút", "12 phút"
QColor mixColor(const QColor& a, const QColor& b, qreal t); // pha màu a -> b theo t (0..1)
QColor coverColor(const QString& seed, int hueShift = 0);   // màu ổn định sinh từ một chuỗi (id bài hát, nghệ sĩ...)
QString styleSheet();
}

QString theme::qs(const std::string& s){
    return QString::fromStdString(s);
}
std::string theme::ss(const QString& s){
    return s.toStdString();
}
QString theme::formatTime(int seconds){
    if (seconds < 0) seconds = 0;
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    if (h > 0) return QString("%1:%2:%3").arg(h).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    return QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
}
QString theme::formatLong(int seconds){
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    if (h > 0) return QString("%1 giờ %2 phút").arg(h).arg(m);
    if (m > 0) return QString("%1 phút").arg(m);
    return QString("%1 giây").arg(seconds);
}
QColor theme::mixColor(const QColor& a, const QColor& b, qreal t){
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return QColor(int(a.red() + (b.red() - a.red()) * t), int(a.green() + (b.green() - a.green()) * t),
                  int(a.blue() + (b.blue() - a.blue()) * t), int(a.alpha() + (b.alpha() - a.alpha()) * t));
}
QColor theme::coverColor(const QString& seed, int hueShift){
    unsigned long long h = 5381;
    QByteArray bytes = seed.toUtf8();
    for (char c : bytes) h = h * 33 + (unsigned char)c;
    int hue = (int)((h % 360) + hueShift + 360) % 360;
    return QColor::fromHsv(hue, 170, 205);
}
QString theme::styleSheet(){
    return QString::fromUtf8(R"QSS(
QWidget{ color:#ffffff; font-family:"Segoe UI","SF Pro Text","Helvetica Neue","Noto Sans","Arial",sans-serif; font-size:14px; }
QToolTip{ background:#282828; color:#ffffff; border:1px solid #3a3a3a; padding:5px 9px; border-radius:4px; }
QLabel{ background:transparent; }
QScrollArea{ border:none; background:transparent; }
QScrollArea > QWidget > QWidget{ background:transparent; }
QScrollBar:vertical{ background:transparent; width:12px; margin:0; }
QScrollBar::handle:vertical{ background:rgba(255,255,255,70); border-radius:4px; min-height:40px; margin:2px 3px; }
QScrollBar::handle:vertical:hover{ background:rgba(255,255,255,130); }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical{ height:0; width:0; }
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{ background:transparent; }
QScrollBar:horizontal{ background:transparent; height:12px; margin:0; }
QScrollBar::handle:horizontal{ background:rgba(255,255,255,70); border-radius:4px; min-width:40px; margin:3px 2px; }
QScrollBar::handle:horizontal:hover{ background:rgba(255,255,255,130); }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal{ height:0; width:0; }
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{ background:transparent; }

QLineEdit{ background:#242424; border:2px solid transparent; border-radius:8px; padding:10px 12px; selection-background-color:#1db954; selection-color:#000000; }
QLineEdit:hover{ background:#2a2a2a; }
QLineEdit:focus{ border:2px solid #ffffff; background:#2a2a2a; }
QLineEdit[search="true"]{ border-radius:21px; padding:10px 16px 10px 44px; background:#242424; }
QLineEdit[search="true"]:hover{ background:#2c2c2c; border:2px solid #3a3a3a; }
QLineEdit[search="true"]:focus{ border:2px solid #ffffff; }

QFrame[panel="true"]{ background:#121212; border-radius:10px; }
QPushButton{ border:none; background:transparent; }
QPushButton[kind="primary"]{ background:#1db954; color:#000000; border-radius:23px; padding:12px 34px; font-weight:700; }
QPushButton[kind="primary"]:hover{ background:#1ed760; }
QPushButton[kind="primary"]:pressed{ background:#169c46; }
QPushButton[kind="primary"]:disabled{ background:#3a3a3a; color:#7a7a7a; }
QPushButton[kind="ghost"]{ background:transparent; color:#ffffff; border:1px solid #727272; border-radius:18px; padding:7px 18px; font-weight:700; }
QPushButton[kind="ghost"]:hover{ border:1px solid #ffffff; }
QPushButton[kind="ghost"]:pressed{ border:1px solid #b3b3b3; color:#b3b3b3; }
QPushButton[kind="danger"]{ background:#e5484d; color:#ffffff; border-radius:18px; padding:8px 20px; font-weight:700; }
QPushButton[kind="danger"]:hover{ background:#f0585d; }
QPushButton[kind="soft"]{ background:#2a2a2a; color:#ffffff; border-radius:18px; padding:8px 18px; font-weight:600; }
QPushButton[kind="soft"]:hover{ background:#3a3a3a; }
QPushButton[kind="link"]{ background:transparent; color:#b3b3b3; font-weight:700; padding:4px 6px; }
QPushButton[kind="link"]:hover{ color:#ffffff; }
QPushButton[kind="seg"]{ background:transparent; color:#b3b3b3; border-radius:18px; padding:9px 0; font-weight:700; }
QPushButton[kind="seg"]:hover{ color:#ffffff; }
QPushButton[kind="seg"]:checked{ background:#2e2e2e; color:#ffffff; }

QMenu{ background:#282828; border:1px solid #3a3a3a; border-radius:8px; padding:6px; }
QMenu::item{ padding:9px 30px 9px 14px; border-radius:4px; background:transparent; }
QMenu::item:selected{ background:#3e3e3e; }
QMenu::item:disabled{ color:#6a6a6a; }
QMenu::separator{ height:1px; background:#3a3a3a; margin:6px 8px; }

QSlider::groove:horizontal{ height:4px; background:#4d4d4d; border-radius:2px; }
QSlider::sub-page:horizontal{ background:#ffffff; border-radius:2px; }
QSlider::sub-page:horizontal:hover{ background:#1db954; }
QSlider::add-page:horizontal{ background:#4d4d4d; border-radius:2px; }
QSlider::handle:horizontal{ width:0px; height:12px; margin:-4px 0; background:#ffffff; border-radius:6px; }
QSlider::handle:horizontal:hover{ width:12px; }

QComboBox{ background:#242424; border:2px solid transparent; border-radius:8px; padding:8px 12px; min-height:20px; }
QComboBox:hover{ background:#2a2a2a; }
QComboBox:focus{ border:2px solid #ffffff; }
QComboBox::drop-down{ border:none; width:26px; }
QComboBox QAbstractItemView{ background:#282828; border:1px solid #3a3a3a; selection-background-color:#3e3e3e; outline:none; padding:4px; }
QSpinBox{ background:#242424; border:2px solid transparent; border-radius:8px; padding:8px 12px; }
QSpinBox:focus{ border:2px solid #ffffff; }

QDialog{ background:#181818; }
QMessageBox{ background:#181818; }
QInputDialog{ background:#181818; }

QTabWidget::pane{ border:none; }
QTabBar{ background:transparent; }
QTabBar::tab{ background:transparent; color:#b3b3b3; padding:11px 20px; border-bottom:3px solid transparent; font-weight:700; margin-right:4px; }
QTabBar::tab:hover{ color:#ffffff; }
QTabBar::tab:selected{ color:#ffffff; border-bottom:3px solid #1db954; }

QTableWidget{ background:transparent; border:none; gridline-color:transparent; selection-background-color:#2f2f2f; selection-color:#ffffff; outline:none; }
QTableWidget::item{ padding:6px 8px; border-bottom:1px solid #222222; }
QTableWidget::item:hover{ background:#232323; }
QHeaderView{ background:transparent; }
QHeaderView::section{ background:transparent; color:#b3b3b3; border:none; border-bottom:1px solid #2a2a2a; padding:9px 8px; font-weight:700; }
QTableCornerButton::section{ background:transparent; border:none; }
)QSS");
}