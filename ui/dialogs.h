#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <QComboBox>
#include <QDate>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMediaPlayer>
#include <QPushButton>
#include <QSpinBox>
#include <QUrl>
#include <QVBoxLayout>
#include "bll/musicsystem.h"
#include "theme.h"
#include "widgets.h"

// Hộp thoại nhập liệu chung: tiêu đề, các dòng nhập, dòng báo lỗi, nút Hủy / OK.
// onSubmit trả về chuỗi lỗi; rỗng nghĩa là thành công và hộp thoại đóng lại.
class FormDialog : public QDialog{
    private:
        QFormLayout* form;
        QLabel* errorLabel;
    public:
        std::function<std::string()> onSubmit;
        FormDialog(const QString& title, const QString& okText, QWidget* parent);
        QFormLayout* formLayout() const;
        void setError(const QString& text);
};

bool confirmDialog(QWidget* parent, const QString& title, const QString& text, const QString& okText = "Xóa");
bool textPromptDialog(QWidget* parent, const QString& title, const QString& label, QString& text, const QString& initial = QString());
bool songDialog(QWidget* parent, MusicSystem& ms, const SongInfo* editing);   // editing == nullptr: thêm mới
bool artistDialog(QWidget* parent, MusicSystem& ms);
bool albumDialog(QWidget* parent, MusicSystem& ms);
bool passwordDialog(QWidget* parent, MusicSystem& ms);

FormDialog::FormDialog(const QString& title, const QString& okText, QWidget* parent) : QDialog(parent){
    this->setWindowTitle(title);
    this->setMinimumWidth(460);
    this->setModal(true);
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(28, 24, 28, 22);
    lay->setSpacing(14);
    lay->addWidget(makeLabel(title, 22, true));
    this->form = new QFormLayout;
    this->form->setSpacing(12);
    this->form->setLabelAlignment(Qt::AlignLeft);
    lay->addLayout(this->form);
    this->errorLabel = makeLabel("", 13, false, QColor(255, 107, 107));
    this->errorLabel->setWordWrap(true);
    this->errorLabel->hide();
    lay->addWidget(this->errorLabel);
    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addStretch();
    QPushButton* cancel = makeButton("Hủy", "ghost");
    QPushButton* ok = makeButton(okText, "primary");
    ok->setDefault(true);
    buttons->addWidget(cancel);
    buttons->addWidget(ok);
    lay->addLayout(buttons);
    QObject::connect(cancel, &QAbstractButton::clicked, this, [this](){ this->reject(); });
    QObject::connect(ok, &QAbstractButton::clicked, this, [this](){
        std::string err = this->onSubmit ? this->onSubmit() : std::string();
        if (err.empty()) this->accept();
        else this->setError(theme::qs(err));
    });
}
QFormLayout* FormDialog::formLayout() const{
    return this->form;
}
void FormDialog::setError(const QString& text){
    this->errorLabel->setText(text);
    this->errorLabel->setVisible(!text.isEmpty());
}

bool confirmDialog(QWidget* parent, const QString& title, const QString& text, const QString& okText){
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(400);
    QVBoxLayout* lay = new QVBoxLayout(&dlg);
    lay->setContentsMargins(28, 24, 28, 22);
    lay->setSpacing(16);
    lay->addWidget(makeLabel(title, 20, true));
    QLabel* body = makeLabel(text, 14, false, theme::kSubText);
    body->setWordWrap(true);
    lay->addWidget(body);
    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addStretch();
    QPushButton* cancel = makeButton("Hủy", "ghost");
    QPushButton* ok = makeButton(okText, "danger");
    buttons->addWidget(cancel);
    buttons->addWidget(ok);
    lay->addLayout(buttons);
    QObject::connect(cancel, &QAbstractButton::clicked, &dlg, &QDialog::reject);
    QObject::connect(ok, &QAbstractButton::clicked, &dlg, &QDialog::accept);
    return dlg.exec() == QDialog::Accepted;
}

bool textPromptDialog(QWidget* parent, const QString& title, const QString& label, QString& text, const QString& initial){
    FormDialog dlg(title, "Lưu", parent);
    QLineEdit* edit = new QLineEdit(initial);
    dlg.formLayout()->addRow(label, edit);
    edit->selectAll();
    dlg.onSubmit = [&](){
        if (edit->text().trimmed().isEmpty()) return std::string("Không được để trống");
        return std::string();
    };
    if (dlg.exec() != QDialog::Accepted) return false;
    text = edit->text().trimmed();
    return true;
}

bool songDialog(QWidget* parent, MusicSystem& ms, const SongInfo* editing){
    FormDialog dlg(editing ? "Sua bai hat" : "Them bai hat", editing ? "Luu" : "Them", parent);
    QLineEdit* name = new QLineEdit;
    QComboBox* artist = new QComboBox;
    for (const ArtistInfo& a : ms.allArtists()) artist->addItem(theme::qs(a.name), theme::qs(a.id));
    QComboBox* album = new QComboBox;
    album->addItem("— Khong thuoc album nao —", QString());
    for (const AlbumInfo& al : ms.allAlbums()) album->addItem(theme::qs(al.name + " — " + al.artist), theme::qs(al.id));
    QComboBox* genre = new QComboBox;
    genre->setEditable(true);
    QStringList genres;
    genres << "Pop" << "Rock" << "Ballad" << "Hip-hop" << "EDM" << "Indie" << "R&B" << "Jazz" << "Nhac tre";
    for (const std::string& g : ms.genres()){
        if (!genres.contains(theme::qs(g))) genres << theme::qs(g);
    }
    genre->addItems(genres);
    QSpinBox* duration = new QSpinBox;
    duration->setRange(1, 7200);
    duration->setValue(200);
    duration->setSuffix(" giay");
    QLineEdit* date = new QLineEdit(QDate::currentDate().toString("yyyy-MM-dd"));
    date->setPlaceholderText("yyyy-mm-dd");
    QLineEdit* file = new QLineEdit;
    file->setReadOnly(true);
    file->setPlaceholderText("chon file nhac .mp3 / .wav ...");
    QPushButton* browse = makeButton("Chon file…", "soft");
    QHBoxLayout* fileRow = new QHBoxLayout;
    fileRow->addWidget(file, 1);
    fileRow->addWidget(browse);

    dlg.formLayout()->addRow("Ten bai hat", name);
    dlg.formLayout()->addRow("Nghe si", artist);
    dlg.formLayout()->addRow("Album", album);
    dlg.formLayout()->addRow("The loai", genre);
    dlg.formLayout()->addRow("Thoi luong", duration);
    dlg.formLayout()->addRow("Ngay phat hanh", date);
    if (editing == nullptr) dlg.formLayout()->addRow("File nhac", fileRow);

    if (editing != nullptr){
        name->setText(theme::qs(editing->name));
        artist->setCurrentIndex(std::max(0, artist->findData(theme::qs(editing->idArtist))));
        album->setCurrentIndex(std::max(0, album->findData(theme::qs(editing->idAlbum))));
        genre->setCurrentText(theme::qs(editing->type));
        duration->setValue(std::max(1, editing->duration));
        date->setText(theme::qs(editing->date));
    }
    // Chọn file xong thì tự đọc thời lượng từ file
    QMediaPlayer* probe = new QMediaPlayer(&dlg);
    QObject::connect(probe, &QMediaPlayer::durationChanged, &dlg, [duration](qint64 d){
        if (d > 0) duration->setValue((int)(d / 1000));
    });
    QObject::connect(browse, &QAbstractButton::clicked, &dlg, [&dlg, file, probe, name](){
        QString path = QFileDialog::getOpenFileName(&dlg, "Chon file nhac", QString(), "Am thanh (*.mp3)");
        if (path.isEmpty()) return;
        file->setText(path);
        probe->setSource(QUrl::fromLocalFile(path));
        if (name->text().trimmed().isEmpty()) name->setText(QFileInfo(path).completeBaseName());
    });
    dlg.onSubmit = [&](){
        std::string idArtist = theme::ss(artist->currentData().toString());
        std::string idAlbum = theme::ss(album->currentData().toString());
        if (editing != nullptr){
            return ms.updateSong(editing->id, theme::ss(name->text()), idArtist, idAlbum, theme::ss(genre->currentText()),
                                 duration->value(), theme::ss(date->text()));
        }
        return ms.addSong(theme::ss(name->text()), idArtist, idAlbum, theme::ss(genre->currentText()),
                          duration->value(), theme::ss(date->text()), theme::ss(file->text()));
    };
    return dlg.exec() == QDialog::Accepted;
}

bool artistDialog(QWidget* parent, MusicSystem& ms){
    FormDialog dlg("Them nghe si", "Them", parent);
    QLineEdit* name = new QLineEdit;
    QLineEdit* country = new QLineEdit("Viet Nam");
    dlg.formLayout()->addRow("Ten nghe si", name);
    dlg.formLayout()->addRow("Quoc gia", country);
    dlg.onSubmit = [&](){ return ms.addArtist(theme::ss(name->text()), theme::ss(country->text())); };
    return dlg.exec() == QDialog::Accepted;
}

bool albumDialog(QWidget* parent, MusicSystem& ms){
    FormDialog dlg("Them album", "Them", parent);
    QLineEdit* name = new QLineEdit;
    QComboBox* artist = new QComboBox;
    for (const ArtistInfo& a : ms.allArtists()) artist->addItem(theme::qs(a.name), theme::qs(a.id));
    QLineEdit* date = new QLineEdit(QDate::currentDate().toString("yyyy-MM-dd"));
    dlg.formLayout()->addRow("Ten album", name);
    dlg.formLayout()->addRow("Nghe si", artist);
    dlg.formLayout()->addRow("Ngay phat hanh", date);
    dlg.onSubmit = [&](){
        return ms.addAlbum(theme::ss(name->text()), theme::ss(artist->currentData().toString()), theme::ss(date->text()));
    };
    return dlg.exec() == QDialog::Accepted;
}

bool passwordDialog(QWidget* parent, MusicSystem& ms){
    FormDialog dlg("Doi mat khau", "Doi mat khau", parent);
    QLineEdit* oldPass = new QLineEdit;
    QLineEdit* newPass = new QLineEdit;
    QLineEdit* newPass2 = new QLineEdit;
    oldPass->setEchoMode(QLineEdit::Password);
    newPass->setEchoMode(QLineEdit::Password);
    newPass2->setEchoMode(QLineEdit::Password);
    dlg.formLayout()->addRow("Mat khau cu", oldPass);
    dlg.formLayout()->addRow("Mat khau moi", newPass);
    dlg.formLayout()->addRow("Nhap lai", newPass2);
    dlg.onSubmit = [&](){
        if (newPass->text() != newPass2->text()) return std::string("Mat khau nhap lai khong khop");
        return ms.changePassword(theme::ss(oldPass->text()), theme::ss(newPass->text()));
    };
    return dlg.exec() == QDialog::Accepted;
}