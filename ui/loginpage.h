#pragma once
#include <cmath>
#include <functional>
#include <QAbstractButton>
#include <QElapsedTimer>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>
#include <QWidget>
#include "bll/musicsystem.h"
#include "icon.h"
#include "theme.h"
#include "widgets.h"

// Ô nhập mật khẩu có nút "con mắt" để hiện / ẩn
class PasswordField : public QWidget{
    private:
        QLineEdit* line;
        IconButton* eye;
    protected:
        void resizeEvent(QResizeEvent* e) override;
    public:
        explicit PasswordField(const QString& placeholder, QWidget* parent = nullptr);
        QLineEdit* edit() const;
};

// Màn hình đăng nhập / đăng ký: nền động (các đốm sáng trôi + thanh equalizer), thẻ trượt vào, rung khi sai mật khẩu.
class LoginPage : public QWidget{
    private:
        MusicSystem& ms;
        QElapsedTimer clock;
        QTimer* frameTimer;
        QFrame* card;
        QGraphicsOpacityEffect* cardFx;
        QPushButton* loginTab;
        QPushButton* registerTab;
        FadeStack* forms;
        QWidget* loginForm;
        QWidget* registerForm;
        QLineEdit* loginUser;
        PasswordField* loginPass;
        QLabel* loginMsg;
        QLineEdit* regUser;
        QLineEdit* regEmail;
        PasswordField* regPass;
        PasswordField* regPass2;
        QLabel* regMsg;
        qreal introOffset;
        qreal shakeDx;
        void layoutCard();
        void setMode(bool registerMode);
        void showMessage(QLabel* label, const QString& text, bool isError);
        void submitLogin();
        void submitRegister();
        QLineEdit* makeEdit(const QString& placeholder);
    protected:
        void paintEvent(QPaintEvent* e) override;
        void resizeEvent(QResizeEvent* e) override;
        void showEvent(QShowEvent* e) override;
    public:
        std::function<void()> onLoggedIn;
        explicit LoginPage(MusicSystem& system, QWidget* parent = nullptr);
        void reset();        // xóa ô nhập, quay về tab đăng nhập
        void playIntro();    // thẻ đăng nhập trượt vào
        void shake();        // rung khi nhập sai
};

PasswordField::PasswordField(const QString& placeholder, QWidget* parent) : QWidget(parent){
    this->setFixedHeight(46);
    this->line = new QLineEdit(this);
    this->line->setPlaceholderText(placeholder);
    this->line->setEchoMode(QLineEdit::Password);
    this->line->setStyleSheet("padding-right:44px;");
    this->eye = new IconButton(Icon::Eye, 18, this);
    this->eye->setCheckable(true);
    this->eye->setColors(theme::kSubText, theme::kText, theme::kText);
    QObject::connect(this->eye, &QAbstractButton::clicked, this, [this](){
        bool show = this->eye->isChecked();
        this->line->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
        this->eye->setGlyph(show ? Icon::EyeOff : Icon::Eye);
    });
}
QLineEdit* PasswordField::edit() const{
    return this->line;
}
void PasswordField::resizeEvent(QResizeEvent* e){
    QWidget::resizeEvent(e);
    this->line->setGeometry(0, 0, this->width(), this->height());
    this->eye->move(this->width() - this->eye->width() - 6, (this->height() - this->eye->height()) / 2);
    this->eye->raise();
}

QLineEdit* LoginPage::makeEdit(const QString& placeholder){
    QLineEdit* e = new QLineEdit;
    e->setPlaceholderText(placeholder);
    e->setFixedHeight(46);
    return e;
}

LoginPage::LoginPage(MusicSystem& system, QWidget* parent) : QWidget(parent), ms(system){
    this->introOffset = 0.0;
    this->shakeDx = 0.0;
    this->clock.start();
    this->frameTimer = new QTimer(this);
    this->frameTimer->setInterval(33);
    QObject::connect(this->frameTimer, &QTimer::timeout, this, [this](){ this->update(); });

    this->card = new QFrame(this);
    this->card->setStyleSheet("QFrame#loginCard{ background:rgba(24,24,24,235); border:1px solid #2c2c2c; border-radius:18px; }");
    this->card->setObjectName("loginCard");
    this->card->setFixedWidth(430);
    this->cardFx = new QGraphicsOpacityEffect(this->card);
    this->cardFx->setOpacity(1.0);
    this->card->setGraphicsEffect(this->cardFx);

    QVBoxLayout* cl = new QVBoxLayout(this->card);
    cl->setContentsMargins(38, 34, 38, 30);
    cl->setSpacing(14);

    CoverWidget* logo = new CoverWidget(64, 32, this->card);
    logo->setCircular(true);
    logo->setSpecial(Icon::Note, theme::kAccentHover, QColor(14, 120, 56));
    QHBoxLayout* logoRow = new QHBoxLayout;
    logoRow->addStretch();
    logoRow->addWidget(logo);
    logoRow->addStretch();
    cl->addLayout(logoRow);
    QLabel* title = makeLabel("Melodia", 30, true, theme::kText, this->card);
    title->setAlignment(Qt::AlignCenter);
    QLabel* sub = makeLabel("Am nhac cua ban", 13, false, theme::kSubText, this->card);
    sub->setAlignment(Qt::AlignCenter);
    cl->addWidget(title);
    cl->addWidget(sub);
    cl->addSpacing(6);

    QFrame* seg = new QFrame(this->card);
    seg->setStyleSheet("QFrame{ background:#202020; border-radius:20px; }");
    QHBoxLayout* sl = new QHBoxLayout(seg);
    sl->setContentsMargins(4, 4, 4, 4);
    sl->setSpacing(4);
    this->loginTab = makeButton("Dang nhap", "seg", seg);
    this->registerTab = makeButton("Dang ky", "seg", seg);
    this->loginTab->setCheckable(true);
    this->registerTab->setCheckable(true);
    this->loginTab->setChecked(true);
    sl->addWidget(this->loginTab);
    sl->addWidget(this->registerTab);
    cl->addWidget(seg);

    this->forms = new FadeStack(this->card);
    // --- Form đăng nhập ---
    this->loginForm = new QWidget;
    QVBoxLayout* lf = new QVBoxLayout(this->loginForm);
    lf->setContentsMargins(0, 6, 0, 0);
    lf->setSpacing(12);
    this->loginUser = this->makeEdit("Ten dang nhap");
    this->loginPass = new PasswordField("Mat khau");
    this->loginMsg = makeLabel("", 13, false, theme::kDanger);
    this->loginMsg->setWordWrap(true);
    this->loginMsg->hide();
    QPushButton* loginBtn = makeButton("Dang nhap", "primary");
    loginBtn->setMinimumHeight(46);
    QLabel* hint = makeLabel("Luu y: nhan Ctrl+L bat cu luc nao de dang xuat ngay", 12, false, theme::kSubText);
    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);
    lf->addWidget(this->loginUser);
    lf->addWidget(this->loginPass);
    lf->addWidget(this->loginMsg);
    lf->addWidget(loginBtn);
    lf->addWidget(hint);
    // --- Form đăng ký ---
    this->registerForm = new QWidget;
    QVBoxLayout* rf = new QVBoxLayout(this->registerForm);
    rf->setContentsMargins(0, 6, 0, 0);
    rf->setSpacing(12);
    this->regUser = this->makeEdit("Ten dang nhap (3-20 ky tu, khong dau cach)");
    this->regEmail = this->makeEdit("Email");
    this->regPass = new PasswordField("Mat khau (it nhat 4 ký tu)");
    this->regPass2 = new PasswordField("Nhap lai mat khau");
    this->regMsg = makeLabel("", 13, false, theme::kDanger);
    this->regMsg->setWordWrap(true);
    this->regMsg->hide();
    QPushButton* regBtn = makeButton("Tao tai khoan", "primary");
    regBtn->setMinimumHeight(46);
    rf->addWidget(this->regUser);
    rf->addWidget(this->regEmail);
    rf->addWidget(this->regPass);
    rf->addWidget(this->regPass2);
    rf->addWidget(this->regMsg);
    rf->addWidget(regBtn);
    this->forms->addWidget(this->loginForm);
    this->forms->addWidget(this->registerForm);
    cl->addWidget(this->forms);

    QObject::connect(this->loginTab, &QAbstractButton::clicked, this, [this](){ this->setMode(false); });
    QObject::connect(this->registerTab, &QAbstractButton::clicked, this, [this](){ this->setMode(true); });
    QObject::connect(loginBtn, &QAbstractButton::clicked, this, [this](){ this->submitLogin(); });
    QObject::connect(this->loginUser, &QLineEdit::returnPressed, this, [this](){ this->loginPass->edit()->setFocus(); });
    QObject::connect(this->loginPass->edit(), &QLineEdit::returnPressed, this, [this](){ this->submitLogin(); });
    QObject::connect(regBtn, &QAbstractButton::clicked, this, [this](){ this->submitRegister(); });
    QObject::connect(this->regPass2->edit(), &QLineEdit::returnPressed, this, [this](){ this->submitRegister(); });
    this->setMode(false);
}

void LoginPage::setMode(bool registerMode){
    this->loginTab->setChecked(!registerMode);
    this->registerTab->setChecked(registerMode);
    QWidget* target = registerMode ? this->registerForm : this->loginForm;
    this->forms->setCurrentWidget(target);
    fadeIn(target, 240);
    this->forms->setFixedHeight(target->sizeHint().height() + 8);
    this->card->adjustSize();
    this->layoutCard();
    (registerMode ? this->regUser : this->loginUser)->setFocus();
}
void LoginPage::showMessage(QLabel* label, const QString& text, bool isError){
    label->setStyleSheet(QString("color:%1; font-size:13px; background:transparent;").arg(isError ? "#ff6b6b" : "#1ed760"));
    label->setText(text);
    label->setVisible(!text.isEmpty());
    this->forms->setFixedHeight(this->forms->currentWidget()->sizeHint().height() + 8);
    this->card->adjustSize();
    this->layoutCard();
}
void LoginPage::submitLogin(){
    QString user = this->loginUser->text().trimmed();
    QString pass = this->loginPass->edit()->text();
    if (user.isEmpty() || pass.isEmpty()){
        this->showMessage(this->loginMsg, "Vui long nhap ten dang nhap và mat khau", true);
        this->shake();
        return;
    }
    AuthService::LoginResult r = this->ms.login(theme::ss(user), theme::ss(pass));
    if (r == AuthService::LoginOk){
        this->showMessage(this->loginMsg, "", false);
        if (this->onLoggedIn) this->onLoggedIn();
        return;
    }
    this->showMessage(this->loginMsg, r == AuthService::LoginLocked ? "Tai khoan da bi khoa, hay lien he Admin" : "Sai ten dang nhap hoac mat khau", true);
    this->loginPass->edit()->selectAll();
    this->loginPass->edit()->setFocus();
    this->shake();
}
void LoginPage::submitRegister(){
    if (this->regPass->edit()->text() != this->regPass2->edit()->text()){
        this->showMessage(this->regMsg, "Mat khau nhap lai khong khop", true);
        this->shake();
        return;
    }
    std::string err = this->ms.registerListener(theme::ss(this->regUser->text()), theme::ss(this->regEmail->text()),
                                                theme::ss(this->regPass->edit()->text()));
    if (!err.empty()){
        this->showMessage(this->regMsg, theme::qs(err), true);
        this->shake();
        return;
    }
    QString name = this->regUser->text().trimmed();
    this->regUser->clear();
    this->regEmail->clear();
    this->regPass->edit()->clear();
    this->regPass2->edit()->clear();
    this->showMessage(this->regMsg, "", false);
    this->setMode(false);
    this->loginUser->setText(name);
    this->loginPass->edit()->clear();
    this->loginPass->edit()->setFocus();
    this->showMessage(this->loginMsg, "Tao tai khoan thanh cong, hay dang nhap", false);
}
void LoginPage::reset(){
    this->loginUser->clear();
    this->loginPass->edit()->clear();
    this->regUser->clear();
    this->regEmail->clear();
    this->regPass->edit()->clear();
    this->regPass2->edit()->clear();
    this->loginMsg->hide();
    this->regMsg->hide();
    this->setMode(false);
}
void LoginPage::layoutCard(){
    int x = (int)((this->width() - this->card->width()) / 2 + this->shakeDx);
    int y = (int)((this->height() - this->card->height()) / 2 + this->introOffset);
    this->card->move(x, y);
}
void LoginPage::playIntro(){
    QVariantAnimation* a = new QVariantAnimation(this);
    a->setDuration(700);
    a->setStartValue(0.0);
    a->setEndValue(1.0);
    a->setEasingCurve(QEasingCurve::OutCubic);
    this->cardFx->setEnabled(true);
    QObject::connect(a, &QVariantAnimation::valueChanged, this, [this](const QVariant& v){
        qreal t = v.toReal();
        this->introOffset = (1.0 - t) * 46.0;
        this->cardFx->setOpacity(t);
        this->layoutCard();
    });
    QObject::connect(a, &QVariantAnimation::finished, this, [this](){
        this->introOffset = 0.0;
        this->cardFx->setEnabled(false);
        this->layoutCard();
    });
    a->start(QAbstractAnimation::DeleteWhenStopped);
}
void LoginPage::shake(){
    QVariantAnimation* a = new QVariantAnimation(this);
    a->setDuration(420);
    a->setStartValue(0.0);
    a->setEndValue(1.0);
    QObject::connect(a, &QVariantAnimation::valueChanged, this, [this](const QVariant& v){
        qreal t = v.toReal();
        this->shakeDx = std::sin(t * 3.14159265 * 7.0) * 14.0 * (1.0 - t);
        this->layoutCard();
    });
    QObject::connect(a, &QVariantAnimation::finished, this, [this](){
        this->shakeDx = 0.0;
        this->layoutCard();
    });
    a->start(QAbstractAnimation::DeleteWhenStopped);
}
void LoginPage::showEvent(QShowEvent* e){
    QWidget::showEvent(e);
    this->frameTimer->start();
    this->loginUser->setFocus();
}
void LoginPage::resizeEvent(QResizeEvent* e){
    QWidget::resizeEvent(e);
    this->layoutCard();
}
void LoginPage::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    qreal w = this->width();
    qreal h = this->height();
    qreal t = this->clock.elapsed() / 1000.0;
    QLinearGradient bg(0, 0, w, h);
    bg.setColorAt(0.0, QColor(8, 10, 9));
    bg.setColorAt(1.0, QColor(20, 24, 22));
    p.fillRect(this->rect(), bg);
    auto blob = [&](qreal cx, qreal cy, qreal radius, QColor c){
        QRadialGradient g(QPointF(cx, cy), radius);
        c.setAlpha(120);
        g.setColorAt(0.0, c);
        c.setAlpha(0);
        g.setColorAt(1.0, c);
        p.setPen(Qt::NoPen);
        p.setBrush(g);
        p.drawEllipse(QPointF(cx, cy), radius, radius);
    };
    qreal m = std::min(w, h);
    blob(w * (0.22 + 0.07 * std::sin(t * 0.55)), h * (0.30 + 0.09 * std::cos(t * 0.47)), m * 0.62, QColor(29, 185, 84));
    blob(w * (0.80 + 0.06 * std::cos(t * 0.40)), h * (0.25 + 0.08 * std::sin(t * 0.62)), m * 0.55, QColor(88, 62, 214));
    blob(w * (0.60 + 0.08 * std::sin(t * 0.35)), h * (0.85 + 0.05 * std::cos(t * 0.5)), m * 0.60, QColor(0, 150, 190));
    // Thanh equalizer mờ chạy dọc đáy màn hình
    int bars = std::max(8, (int)(w / 26));
    qreal bw = w / bars;
    for (int i = 0; i < bars; ++i){
        qreal level = 0.5 + 0.5 * std::sin(t * (1.1 + (i % 5) * 0.23) + i * 0.7);
        qreal bh = 18 + level * (h * 0.16);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 14));
        p.drawRoundedRect(QRectF(i * bw + bw * 0.22, h - bh, bw * 0.56, bh), 4, 4);
    }
}