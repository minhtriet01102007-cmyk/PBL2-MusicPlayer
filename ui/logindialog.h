#pragma once
#include "musicsystem.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDialog>

class LoginDialog : public QDialog{
    Q_OBJECT
public:
    explicit LoginDialog(MusicSystem* sys, QWidget* parent = nullptr);
private slots:
    void onLogin();
private:
    MusicSystem* sys;
    QLineEdit *user, *pass;
    QLabel* error;
};
inline LoginDialog::LoginDialog(MusicSystem* s, QWidget* parent) : QDialog(parent), sys(s){
    setWindowTitle("Log in - Music System");
    setMinimumWidth(380);

    user = new QLineEdit; user->setPlaceholderText("Name log in");
    pass = new QLineEdit; pass->setPlaceholderText("Password");
    pass->setEchoMode(QLineEdit::Password);
    error = new QLabel; error->setObjectName("error");

    auto* btn = new QPushButton("Log in");
    btn->setDefault(true);
    btn->setObjectName("primary");
    btn->setMinimumHeight(38);

    auto* form = new QFormLayout;
    form->addRow("Account:", user);
    form->addRow("Password:", pass);

    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(32, 28, 32, 28);
    lay->setSpacing(14);
    auto* title = new QLabel("Music System");
    title->setObjectName("title");
    title->setAlignment(Qt::AlignCenter);
    lay->addWidget(title);
    lay->addLayout(form);
    lay->addWidget(error);
    lay->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(pass, &QLineEdit::returnPressed, this, &LoginDialog::onLogin);
}

inline void LoginDialog::onLogin(){
    if (sys->login(user->text().toStdString(), pass->text().toStdString()))
        accept();
    else
        error->setText("Sai tai khoan hoac mat khau");
}
