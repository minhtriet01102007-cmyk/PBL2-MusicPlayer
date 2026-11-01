#pragma once
#include "musicsystem.h"
#include "uihelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QWidget>

class AdminPage : public QWidget{
    Q_OBJECT
public:
    explicit AdminPage(MusicSystem* sys, QWidget* parent = nullptr);
signals:
    void songsChanged();
private slots:
    void onBrowse();
    void onAdd();
    void onDelete();
private:
    void reload();
    MusicSystem* sys;
    QTableWidget* table;
    QLineEdit *name, *artist, *album, *type, *date, *file;
    QSpinBox* duration;
    QString pickedPath;
};

AdminPage::AdminPage(MusicSystem* s, QWidget* parent) : QWidget(parent), sys(s){
    table = new QTableWidget(0, 7);
    table->setHorizontalHeaderLabels({"ID", "Ten bai hat", "Ca si", "Album", "The loai", "Thoi luong (s)", "File"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setShowGrid(false);
    table->verticalHeader()->hide();
    table->verticalHeader()->setDefaultSectionSize(36);
    table->horizontalHeader()->setHighlightSections(false);

    name = new QLineEdit; artist = new QLineEdit; album = new QLineEdit; type = new QLineEdit;
    date = new QLineEdit; date->setPlaceholderText("vd: 2024-10-18");
    duration = new QSpinBox; duration->setRange(0, 36000); duration->setSuffix(" giay");
    file = new QLineEdit; file->setReadOnly(true);
    auto* btnBrowse = new QPushButton("Chon file mp3...");
    auto* btnAdd = new QPushButton("Them bai hat");
    auto* btnDel = new QPushButton("Xoa bai dang chon");
    btnAdd->setObjectName("primary");
    btnDel->setObjectName("danger");

    auto* fileRow = new QHBoxLayout;
    fileRow->addWidget(file, 1); fileRow->addWidget(btnBrowse);

    auto* form = new QFormLayout;
    form->addRow("Ten bai hat:", name);
    form->addRow("Ca si:", artist);
    form->addRow("Album:", album);
    form->addRow("The loai:", type);
    form->addRow("Ngay phat hanh:", date);
    form->addRow("Thoi luong:", duration);
    form->addRow("File:", fileRow);

    auto* btns = new QHBoxLayout;
    btns->addWidget(btnAdd); btns->addWidget(btnDel); btns->addStretch();

    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(16, 12, 16, 16);
    lay->setSpacing(10);
    lay->addWidget(table, 1);
    lay->addLayout(form);
    lay->addLayout(btns);

    connect(btnBrowse, &QPushButton::clicked, this, &AdminPage::onBrowse);
    connect(btnAdd, &QPushButton::clicked, this, &AdminPage::onAdd);
    connect(btnDel, &QPushButton::clicked, this, &AdminPage::onDelete);
    reload();
}

void AdminPage::reload(){
    auto v = sys->allSongs();
    table->setRowCount((int)v.size());
    for (int i = 0; i < (int)v.size(); ++i){
        const SongInfo& s = v[i];
        table->setItem(i, 0, new QTableWidgetItem(q(s.id)));
        table->setItem(i, 1, new QTableWidgetItem(q(s.name)));
        table->setItem(i, 2, new QTableWidgetItem(q(s.artist)));
        table->setItem(i, 3, new QTableWidgetItem(q(s.album)));
        table->setItem(i, 4, new QTableWidgetItem(q(s.type)));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(s.duration)));
        table->setItem(i, 6, new QTableWidgetItem(s.path.empty() ? "(khong tim thay file)" : QFileInfo(q(s.path)).fileName()));
    }
}

void AdminPage::onBrowse(){
    QString p = QFileDialog::getOpenFileName(this, "Chon file nhac", QString(), "Audio (*.mp3 *.wav *.flac)");
    if (p.isEmpty()) return;
    pickedPath = p;
    file->setText(QFileInfo(p).fileName());
}

void AdminPage::onAdd(){
    if (name->text().trimmed().isEmpty() || pickedPath.isEmpty()){
        QMessageBox::warning(this, "Thieu thong tin", "Can it nhat ten bai hat va file nhac.");
        return;
    }
    if (!sys->addSong(name->text().toStdString(), artist->text().toStdString(), album->text().toStdString(),
                      type->text().toStdString(), duration->value(), date->text().toStdString(),
                      pickedPath.toStdString())){
        QMessageBox::warning(this, "Khong them duoc", "Khong the them bai hat (can quyen Admin va file nhac hop le).");
        return;
    }
    name->clear(); artist->clear(); album->clear(); type->clear(); date->clear(); file->clear();
    duration->setValue(0); pickedPath.clear();
    reload();
    emit songsChanged();
}

void AdminPage::onDelete(){
    int r = table->currentRow();
    if (r < 0) return;
    std::string id = table->item(r, 0)->text().toStdString();
    if (QMessageBox::question(this, "Xac nhan", "Xoa bai hat nay?") != QMessageBox::Yes) return;
    sys->removeSong(id);
    reload();
    emit songsChanged();
}
