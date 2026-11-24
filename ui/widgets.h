#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>
#include <QAbstractButton>
#include <QApplication>
#include <QContextMenuEvent>
#include <QCursor>
#include <QEasingCurve>
#include <QFontMetrics>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSlider>
#include <QStackedWidget>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>
#include <QWidget>
#include "icon.h"
#include "theme.h"
#include "bll/songinfo.h"

// Các thành phần giao diện dùng chung. Không dùng Q_OBJECT: mọi "tín hiệu" tự định nghĩa đều là std::function,
// nên không cần moc và header chỉ được biên dịch trong đúng một file .cpp (main.cpp).

// ---------- Hỗ trợ hiệu ứng rê chuột ----------
bool cursorInside(const QWidget* w);

class Hover{
    private:
        QVariantAnimation* anim;
    public:
        qreal t;                                // 0 = bình thường, 1 = đang rê chuột
        std::function<void(qreal)> onValue;     // gọi mỗi khi t thay đổi (để cập nhật thêm thành phần con)
        Hover();
        void init(QWidget* owner, int durationMs = 160);
        void to(qreal target);
        void handle(QEvent* e, QWidget* owner); // gọi trong event() của widget có WA_Hover
};

// ---------- Hàm dựng nhanh ----------
void clearLayout(QLayout* layout);
QLabel* makeLabel(const QString& text, int pixelSize, bool bold = false, const QColor& color = theme::kText, QWidget* parent = nullptr);
QPushButton* makeButton(const QString& text, const char* kind, QWidget* parent = nullptr);

// ---------- Nhãn có thể bấm và tự cắt chữ dài (...) ----------
class LinkLabel : public QLabel{
    private:
        QString fullText;
        bool clickable;
        void updateElide();
    protected:
        bool event(QEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void resizeEvent(QResizeEvent* e) override;
    public:
        std::function<void()> onClick;
        explicit LinkLabel(QWidget* parent = nullptr);
        void setFullText(const QString& text);
        void setClickable(bool on);
};

// ---------- Ảnh bìa vẽ bằng gradient (không cần file ảnh) ----------
class CoverWidget : public QWidget{
    private:
        QString seed;
        QString label;
        qreal radius;
        bool circular;
        bool special;
        Icon specialIcon;
        QColor colorA, colorB;
    protected:
        void paintEvent(QPaintEvent* e) override;
    public:
        CoverWidget(int size, qreal cornerRadius, QWidget* parent = nullptr);
        void setSeed(const QString& newSeed, const QString& newLabel);
        void setCircular(bool on);
        void setSpecial(Icon icon, const QColor& a, const QColor& b);
};

// ---------- Biểu tượng "đang phát" nhảy theo nhạc ----------
class EqualizerWidget : public QWidget{
    private:
        QTimer* timer;
        qreal phase;
        bool playing;
    protected:
        void paintEvent(QPaintEvent* e) override;
    public:
        explicit EqualizerWidget(QWidget* parent = nullptr);
        void setPlaying(bool on);
};

// ---------- Thông báo nổi (toast) ----------
class Toast : public QLabel{
    private:
        QTimer* hideTimer;
        QGraphicsOpacityEffect* fx;
        QPointer<QPropertyAnimation> outAnim;
        int generation;
    public:
        explicit Toast(QWidget* parent);
        void showMessage(const QString& text, bool isError = false, int bottomMargin = 110);
};

// ---------- Chuyển trang có hiệu ứng mờ dần ----------
void fadeIn(QWidget* page, int durationMs = 280); // hiện page từ trong suốt đến rõ nét

class FadeStack : public QStackedWidget{
    public:
        explicit FadeStack(QWidget* parent = nullptr);
        void showPage(QWidget* page, bool animate = true); // trang cũ bị xóa
};

// ---------- Thanh trượt bấm đâu nhảy tới đó ----------
class ClickSlider : public QSlider{
    private:
        bool dragging;
        void updateFromX(int x);
    protected:
        void mousePressEvent(QMouseEvent* e) override;
        void mouseMoveEvent(QMouseEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
    public:
        std::function<void(int)> onScrub;     // đang kéo
        std::function<void(int)> onUserSeek;  // thả chuột
        explicit ClickSlider(Qt::Orientation o = Qt::Horizontal, QWidget* parent = nullptr);
        bool isDragging() const;
};

// ---------- Nền trang có dải màu mờ dần ở trên ----------
class PageCanvas : public QWidget{
    private:
        QColor tint;
    protected:
        void paintEvent(QPaintEvent* e) override;
    public:
        explicit PageCanvas(const QColor& tintColor, QWidget* parent = nullptr);
};
QScrollArea* makePage(const QColor& tint, QVBoxLayout*& layout);

// ---------- Thẻ (bài hát / album / nghệ sĩ) có hiệu ứng nổi và nút Play hiện dần ----------
class Card : public QWidget{
    private:
        CoverWidget* cover;
        QLabel* titleLabel;
        QLabel* subtitleLabel;
        IconButton* playBtn;
        QGraphicsOpacityEffect* playFx;
        Hover hover;
        void placePlayButton();
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
    public:
        std::function<void()> onClick;
        std::function<void()> onPlay;
        Card(const QString& seed, const QString& title, const QString& subtitle, bool circularCover, QWidget* parent = nullptr);
        static int cardWidth();
        static int cardHeight();
};

// ---------- Lưới thẻ tự xuống dòng; maxRows > 0 thì chỉ hiện số hàng đó (các thẻ thừa bị ẩn) ----------
class CardFlow : public QWidget{
    private:
        std::vector<QWidget*> cards;
        int cardW, cardH, gap, maxRows;
        void relayout();
    protected:
        void resizeEvent(QResizeEvent* e) override;
    public:
        CardFlow(int cardWidth, int cardHeight, int maxRowCount, QWidget* parent = nullptr);
        void addCard(QWidget* card);
};

// ---------- Ô truy cập nhanh trang chủ ----------
class QuickTile : public QWidget{
    private:
        CoverWidget* cover;
        QLabel* titleLabel;
        IconButton* playBtn;
        QGraphicsOpacityEffect* playFx;
        Hover hover;
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
        void resizeEvent(QResizeEvent* e) override;
    public:
        std::function<void()> onClick;
        std::function<void()> onPlay;
        QuickTile(const QString& seed, const QString& title, QWidget* parent = nullptr);
        void setSpecial(Icon icon, const QColor& a, const QColor& b);
};

// ---------- Ô thể loại màu sắc ----------
class GenreTile : public QWidget{
    private:
        QString name;
        Hover hover;
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
        void mouseReleaseEvent(QMouseEvent* e) override;
    public:
        std::function<void()> onClick;
        GenreTile(const QString& genre, QWidget* parent = nullptr);
};

// ---------- Một dòng bài hát trong danh sách ----------
class SongRow : public QWidget{
    private:
        SongInfo info;
        bool hovered;
        bool current;
        QWidget* cell;
        QLabel* indexLabel;
        IconButton* playBtn;
        EqualizerWidget* eq;
        LinkLabel* titleLabel;
        LinkLabel* artistLabel;
        LinkLabel* albumLabel;
        IconButton* heartBtn;
        IconButton* moreBtn;
        Hover hover;
        void updateCell();
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
        void mouseDoubleClickEvent(QMouseEvent* e) override;
        void contextMenuEvent(QContextMenuEvent* e) override;
    public:
        std::function<void(const SongInfo&)> onPlay;
        std::function<bool(const SongInfo&)> onToggleLike;   // trả về trạng thái "đã thích" mới
        std::function<void(const SongInfo&, const QPoint&)> onMore;
        std::function<void(const std::string&)> onOpenArtist;
        std::function<void(const std::string&)> onOpenAlbum;
        SongRow(const SongInfo& song, int number, bool showAlbum, bool showCover, QWidget* parent = nullptr);
        const std::string& songId() const;
        void setCurrent(bool isCurrent, bool isPlaying);
        void setLiked(bool liked);
};

// ---------- Mục trong thanh bên: dạng điều hướng (biểu tượng + chữ) hoặc dạng thư viện (bìa + 2 dòng chữ) ----------
class SideItem : public QAbstractButton{
    private:
        bool libraryMode;
        Icon icon;
        QString title;
        QString subtitle;
        QString seed;
        bool special;
        Icon specialIcon;
        QColor colorA, colorB;
        bool active;
        Hover hover;
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
    public:
        SideItem(Icon navIcon, const QString& text, QWidget* parent = nullptr);
        SideItem(const QString& coverSeed, const QString& text, const QString& sub, QWidget* parent = nullptr);
        void setSpecialCover(Icon coverIcon, const QColor& a, const QColor& b);
        void setActive(bool on);
        QSize sizeHint() const override;
};

// =====================================================================================
// Định nghĩa
// =====================================================================================

bool cursorInside(const QWidget* w){
    return w->rect().contains(w->mapFromGlobal(QCursor::pos()));
}

Hover::Hover() : anim(nullptr), t(0.0){
}
void Hover::init(QWidget* owner, int durationMs){
    this->anim = new QVariantAnimation(owner);
    this->anim->setDuration(durationMs);
    this->anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(this->anim, &QVariantAnimation::valueChanged, owner, [this, owner](const QVariant& v){
        this->t = v.toReal();
        if (this->onValue) this->onValue(this->t);
        owner->update();
    });
    owner->setAttribute(Qt::WA_Hover, true);
}
void Hover::to(qreal target){
    if (this->anim == nullptr) return;
    this->anim->stop();
    this->anim->setStartValue(this->t);
    this->anim->setEndValue(target);
    this->anim->start();
}
void Hover::handle(QEvent* e, QWidget* owner){
    if (e->type() == QEvent::HoverEnter){
        this->to(1.0);
    } else if (e->type() == QEvent::HoverLeave){
        QTimer::singleShot(0, owner, [this, owner](){
            if (!cursorInside(owner)) this->to(0.0);
        });
    }
}

void clearLayout(QLayout* layout){
    if (layout == nullptr) return;
    while (QLayoutItem* item = layout->takeAt(0)){
        if (item->widget() != nullptr) item->widget()->deleteLater();
        if (item->layout() != nullptr) clearLayout(item->layout());
        delete item;
    }
}
QLabel* makeLabel(const QString& text, int pixelSize, bool bold, const QColor& color, QWidget* parent){
    QLabel* l = new QLabel(text, parent);
    l->setTextFormat(Qt::PlainText);
    l->setStyleSheet(QString("color:%1; font-size:%2px; font-weight:%3; background:transparent;")
                         .arg(color.name()).arg(pixelSize).arg(bold ? 700 : 400));
    return l;
}
QPushButton* makeButton(const QString& text, const char* kind, QWidget* parent){
    QPushButton* b = new QPushButton(text, parent);
    b->setProperty("kind", kind);
    b->setCursor(Qt::PointingHandCursor);
    b->setFocusPolicy(Qt::NoFocus);
    return b;
}

// ---------- LinkLabel ----------
LinkLabel::LinkLabel(QWidget* parent) : QLabel(parent), clickable(false){
    this->setTextFormat(Qt::PlainText);
    this->setAttribute(Qt::WA_Hover, true);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}
void LinkLabel::updateElide(){
    QFontMetrics fm(this->font());
    QString shown = fm.elidedText(this->fullText, Qt::ElideRight, std::max(10, this->width()));
    QLabel::setText(shown);
    this->setToolTip(shown != this->fullText ? this->fullText : QString());
}
void LinkLabel::setFullText(const QString& text){
    this->fullText = text;
    this->updateElide();
}
void LinkLabel::setClickable(bool on){
    this->clickable = on;
    this->setCursor(on ? Qt::PointingHandCursor : Qt::ArrowCursor);
}
bool LinkLabel::event(QEvent* e){
    if (this->clickable && (e->type() == QEvent::HoverEnter || e->type() == QEvent::HoverLeave)){
        QFont f = this->font();
        f.setUnderline(e->type() == QEvent::HoverEnter);
        this->setFont(f);
    }
    return QLabel::event(e);
}
void LinkLabel::mouseReleaseEvent(QMouseEvent* e){
    if (this->clickable && e->button() == Qt::LeftButton && this->rect().contains(e->position().toPoint()) && this->onClick){
        this->onClick();
        return;
    }
    QLabel::mouseReleaseEvent(e);
}
void LinkLabel::resizeEvent(QResizeEvent* e){
    QLabel::resizeEvent(e);
    this->updateElide();
}

// ---------- CoverWidget ----------
CoverWidget::CoverWidget(int size, qreal cornerRadius, QWidget* parent) : QWidget(parent){
    this->radius = cornerRadius;
    this->circular = false;
    this->special = false;
    this->specialIcon = Icon::Heart;
    this->colorA = theme::kAccent;
    this->colorB = theme::kAccent;
    this->setFixedSize(size, size);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}
void CoverWidget::setSeed(const QString& newSeed, const QString& newLabel){
    this->seed = newSeed;
    this->label = newLabel;
    this->special = false;
    this->colorA = theme::coverColor(newSeed, 0);
    this->colorB = theme::coverColor(newSeed, 55).darker(150);
    this->update();
}
void CoverWidget::setCircular(bool on){
    this->circular = on;
    this->update();
}
void CoverWidget::setSpecial(Icon icon, const QColor& a, const QColor& b){
    this->special = true;
    this->specialIcon = icon;
    this->colorA = a;
    this->colorB = b;
    this->update();
}
void CoverWidget::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRectF r(this->rect());
    QPainterPath path;
    if (this->circular) path.addEllipse(r);
    else path.addRoundedRect(r, this->radius, this->radius);
    p.setClipPath(path);
    QLinearGradient g(r.topLeft(), r.bottomRight());
    g.setColorAt(0.0, this->colorA);
    g.setColorAt(1.0, this->colorB);
    p.fillRect(r, g);
    QRadialGradient glow(r.width() * 0.22, r.height() * 0.12, r.width() * 0.95);
    glow.setColorAt(0.0, QColor(255, 255, 255, 80));
    glow.setColorAt(1.0, QColor(255, 255, 255, 0));
    p.fillRect(r, glow);
    if (this->special){
        qreal s = r.width() * 0.46;
        drawIcon(p, this->specialIcon, QRectF((r.width() - s) / 2, (r.height() - s) / 2, s, s), QColor(255, 255, 255));
    } else{
        QString letter = this->label.trimmed().left(1).toUpper();
        if (letter.isEmpty()) letter = QString::fromUtf8("♪");
        QFont f = p.font();
        f.setPixelSize(int(r.height() * 0.42));
        f.setBold(true);
        p.setFont(f);
        p.setPen(QColor(255, 255, 255, 215));
        p.drawText(r, Qt::AlignCenter, letter);
    }
}

// ---------- EqualizerWidget ----------
EqualizerWidget::EqualizerWidget(QWidget* parent) : QWidget(parent), phase(0.0), playing(false){
    this->setFixedSize(16, 16);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->timer = new QTimer(this);
    this->timer->setInterval(55);
    QObject::connect(this->timer, &QTimer::timeout, this, [this](){
        this->phase += 0.2;
        this->update();
    });
}
void EqualizerWidget::setPlaying(bool on){
    this->playing = on;
    if (on) this->timer->start();
    else this->timer->stop();
    this->update();
}
void EqualizerWidget::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(theme::kAccent);
    const int bars = 4;
    qreal bw = 2.6;
    qreal gap = (this->width() - bars * bw) / (bars - 1);
    for (int i = 0; i < bars; ++i){
        qreal level = this->playing ? 0.25 + 0.75 * std::abs(std::sin(this->phase * (1.3 + i * 0.45) + i * 1.7)) : 0.35;
        qreal h = this->height() * level;
        p.drawRoundedRect(QRectF(i * (bw + gap), this->height() - h, bw, h), 1.2, 1.2);
    }
}

// ---------- Toast ----------
Toast::Toast(QWidget* parent) : QLabel(parent), generation(0){
    this->setAlignment(Qt::AlignCenter);
    this->setTextFormat(Qt::PlainText);
    this->fx = new QGraphicsOpacityEffect(this);
    this->fx->setOpacity(0.0);
    this->setGraphicsEffect(this->fx);
    this->hide();
    this->hideTimer = new QTimer(this);
    this->hideTimer->setSingleShot(true);
    QObject::connect(this->hideTimer, &QTimer::timeout, this, [this](){
        int gen = this->generation;
        QPropertyAnimation* a = new QPropertyAnimation(this->fx, "opacity", this);
        a->setDuration(320);
        a->setStartValue(this->fx->opacity());
        a->setEndValue(0.0);
        QObject::connect(a, &QPropertyAnimation::finished, this, [this, gen](){
            if (gen == this->generation) this->hide();
        });
        this->outAnim = a;
        a->start(QAbstractAnimation::DeleteWhenStopped);
    });
}
void Toast::showMessage(const QString& text, bool isError, int bottomMargin){
    this->generation++;
    if (this->outAnim) this->outAnim->stop();
    this->setText(text);
    this->setStyleSheet(QString("background:#2a2a2a; color:#ffffff; border:1px solid %1; border-radius:22px; padding:12px 26px; font-weight:600; font-size:14px;")
                            .arg(isError ? "#e5484d" : "#1db954"));
    this->adjustSize();
    QWidget* p = this->parentWidget();
    int x = (p->width() - this->width()) / 2;
    int y = p->height() - bottomMargin - this->height();
    this->move(x, y);
    this->raise();
    this->show();
    QPropertyAnimation* fadeAnim = new QPropertyAnimation(this->fx, "opacity", this);
    fadeAnim->setDuration(220);
    fadeAnim->setStartValue(0.0);
    fadeAnim->setEndValue(1.0);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation* slide = new QPropertyAnimation(this, "pos", this);
    slide->setDuration(300);
    slide->setEasingCurve(QEasingCurve::OutCubic);
    slide->setStartValue(QPoint(x, y + 22));
    slide->setEndValue(QPoint(x, y));
    slide->start(QAbstractAnimation::DeleteWhenStopped);
    this->hideTimer->start(2600);
}

// ---------- FadeStack ----------
FadeStack::FadeStack(QWidget* parent) : QStackedWidget(parent){
}
void FadeStack::showPage(QWidget* page, bool animate){
    QWidget* old = this->currentWidget();
    this->addWidget(page);
    this->setCurrentWidget(page);
    if (old != nullptr && old != page){
        this->removeWidget(old);
        old->deleteLater();
    }
    if (animate) fadeIn(page);
}
void fadeIn(QWidget* page, int durationMs){
    QGraphicsOpacityEffect* fx = new QGraphicsOpacityEffect(page);
    fx->setOpacity(0.0);
    page->setGraphicsEffect(fx);
    QPropertyAnimation* a = new QPropertyAnimation(fx, "opacity", page);
    a->setDuration(durationMs);
    a->setStartValue(0.0);
    a->setEndValue(1.0);
    a->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(a, &QPropertyAnimation::finished, page, [page](){
        QTimer::singleShot(0, page, [page](){ page->setGraphicsEffect(nullptr); }); // bỏ hiệu ứng cho nhẹ khi cuộn
    });
    a->start(QAbstractAnimation::DeleteWhenStopped);
}

// ---------- ClickSlider ----------
ClickSlider::ClickSlider(Qt::Orientation o, QWidget* parent) : QSlider(o, parent), dragging(false){
    this->setCursor(Qt::PointingHandCursor);
    this->setFocusPolicy(Qt::NoFocus);
}
bool ClickSlider::isDragging() const{
    return this->dragging;
}
void ClickSlider::updateFromX(int x){
    int v = QStyle::sliderValueFromPosition(this->minimum(), this->maximum(), x, std::max(1, this->width()));
    this->setValue(v);
    if (this->onScrub) this->onScrub(v);
}
void ClickSlider::mousePressEvent(QMouseEvent* e){
    if (e->button() != Qt::LeftButton){
        QSlider::mousePressEvent(e);
        return;
    }
    this->dragging = true;
    this->setSliderDown(true);
    this->updateFromX((int)e->position().x());
    e->accept();
}
void ClickSlider::mouseMoveEvent(QMouseEvent* e){
    if (!this->dragging){
        QSlider::mouseMoveEvent(e);
        return;
    }
    this->updateFromX((int)e->position().x());
    e->accept();
}
void ClickSlider::mouseReleaseEvent(QMouseEvent* e){
    if (!this->dragging){
        QSlider::mouseReleaseEvent(e);
        return;
    }
    this->dragging = false;
    this->setSliderDown(false);
    this->updateFromX((int)e->position().x());
    if (this->onUserSeek) this->onUserSeek(this->value());
    e->accept();
}

// ---------- PageCanvas ----------
PageCanvas::PageCanvas(const QColor& tintColor, QWidget* parent) : QWidget(parent), tint(tintColor){
}
void PageCanvas::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.fillRect(this->rect(), theme::kBg);
    QLinearGradient g(0, 0, 0, 420);
    g.setColorAt(0.0, theme::mixColor(theme::kBg, this->tint, 0.55));
    g.setColorAt(1.0, theme::kBg);
    p.fillRect(QRect(0, 0, this->width(), 420), g);
}
QScrollArea* makePage(const QColor& tint, QVBoxLayout*& layout){
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    PageCanvas* canvas = new PageCanvas(tint);
    layout = new QVBoxLayout(canvas);
    layout->setContentsMargins(28, 22, 28, 36);
    layout->setSpacing(14);
    scroll->setWidget(canvas);
    return scroll;
}

// ---------- Card ----------
int Card::cardWidth(){ return 172; }
int Card::cardHeight(){ return 240; }
Card::Card(const QString& seed, const QString& title, const QString& subtitle, bool circularCover, QWidget* parent) : QWidget(parent){
    this->setFixedSize(cardWidth(), cardHeight());
    this->setCursor(Qt::PointingHandCursor);
    this->cover = new CoverWidget(148, 8, this);
    this->cover->setSeed(seed, title);
    this->cover->setCircular(circularCover);
    this->cover->move(12, 12);
    QFont tf;
    tf.setPixelSize(14);
    tf.setBold(true);
    QFont sf;
    sf.setPixelSize(12);
    this->titleLabel = makeLabel(QFontMetrics(tf).elidedText(title, Qt::ElideRight, 148), 14, true, theme::kText, this);
    this->titleLabel->setGeometry(12, 170, 148, 22);
    this->titleLabel->setToolTip(title);
    this->titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->subtitleLabel = makeLabel(QFontMetrics(sf).elidedText(subtitle, Qt::ElideRight, 296), 12, false, theme::kSubText, this);
    this->subtitleLabel->setWordWrap(true);
    this->subtitleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->subtitleLabel->setGeometry(12, 194, 148, 36);
    this->subtitleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->playBtn = new IconButton(Icon::Play, 18, this);
    this->playBtn->setColors(QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0));
    this->playBtn->setCircle(theme::kAccent, 44);
    this->playFx = new QGraphicsOpacityEffect(this->playBtn);
    this->playFx->setOpacity(0.0);
    this->playBtn->setGraphicsEffect(this->playFx);
    QObject::connect(this->playBtn, &QAbstractButton::clicked, this, [this](){
        if (this->onPlay) this->onPlay();
    });
    this->hover.onValue = [this](qreal){ this->placePlayButton(); };
    this->hover.init(this, 180);
    this->placePlayButton();
}
void Card::placePlayButton(){
    qreal t = this->hover.t;
    this->playFx->setOpacity(t);
    this->playBtn->move(12 + 148 - this->playBtn->width() - 4, int(12 + 148 - this->playBtn->height() - 4 + (1.0 - t) * 12));
    this->playBtn->setVisible(t > 0.01);
}
bool Card::event(QEvent* e){
    this->hover.handle(e, this);
    return QWidget::event(e);
}
void Card::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(theme::mixColor(theme::kPanel, theme::kHover, this->hover.t));
    p.drawRoundedRect(QRectF(this->rect()), 10, 10);
}
void Card::mouseReleaseEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton && this->rect().contains(e->position().toPoint()) && this->onClick) this->onClick();
}

// ---------- CardFlow ----------
CardFlow::CardFlow(int cardWidth, int cardHeight, int maxRowCount, QWidget* parent)
    : QWidget(parent), cardW(cardWidth), cardH(cardHeight), gap(16), maxRows(maxRowCount){
    this->setMinimumHeight(cardHeight);
}
void CardFlow::addCard(QWidget* card){
    card->setParent(this);
    this->cards.push_back(card);
    card->show();
    this->relayout();
}
void CardFlow::relayout(){
    int n = (int)this->cards.size();
    int cols = std::max(1, (this->width() + this->gap) / (this->cardW + this->gap));
    int visible = (this->maxRows > 0) ? std::min(n, cols * this->maxRows) : n;
    for (int i = 0; i < n; ++i){
        if (i < visible){
            this->cards[i]->move((i % cols) * (this->cardW + this->gap), (i / cols) * (this->cardH + this->gap));
            this->cards[i]->show();
        } else{
            this->cards[i]->hide();
        }
    }
    int rows = (visible + cols - 1) / cols;
    int h = rows > 0 ? rows * this->cardH + (rows - 1) * this->gap : 0;
    this->setMinimumHeight(h);
    this->setMaximumHeight(h);
}
void CardFlow::resizeEvent(QResizeEvent* e){
    QWidget::resizeEvent(e);
    this->relayout();
}

// ---------- QuickTile ----------
QuickTile::QuickTile(const QString& seed, const QString& title, QWidget* parent) : QWidget(parent){
    this->setFixedHeight(64);
    this->setCursor(Qt::PointingHandCursor);
    this->cover = new CoverWidget(64, 8, this);
    this->cover->setSeed(seed, title);
    this->cover->move(0, 0);
    this->titleLabel = makeLabel(title, 14, true, theme::kText, this);
    this->titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->titleLabel->setWordWrap(false);
    this->playBtn = new IconButton(Icon::Play, 16, this);
    this->playBtn->setColors(QColor(0, 0, 0), QColor(0, 0, 0), QColor(0, 0, 0));
    this->playBtn->setCircle(theme::kAccent, 38);
    this->playFx = new QGraphicsOpacityEffect(this->playBtn);
    this->playFx->setOpacity(0.0);
    this->playBtn->setGraphicsEffect(this->playFx);
    this->playBtn->hide();
    QObject::connect(this->playBtn, &QAbstractButton::clicked, this, [this](){
        if (this->onPlay) this->onPlay();
    });
    this->hover.onValue = [this](qreal t){
        this->playFx->setOpacity(t);
        this->playBtn->setVisible(t > 0.01);
    };
    this->hover.init(this, 160);
}
void QuickTile::setSpecial(Icon icon, const QColor& a, const QColor& b){
    this->cover->setSpecial(icon, a, b);
}
void QuickTile::resizeEvent(QResizeEvent* e){
    QWidget::resizeEvent(e);
    this->titleLabel->setGeometry(78, 0, std::max(10, this->width() - 78 - 56), this->height());
    this->playBtn->move(this->width() - this->playBtn->width() - 8, (this->height() - this->playBtn->height()) / 2);
}
bool QuickTile::event(QEvent* e){
    this->hover.handle(e, this);
    return QWidget::event(e);
}
void QuickTile::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(theme::mixColor(QColor(255, 255, 255, 20), QColor(255, 255, 255, 48), this->hover.t));
    p.drawRoundedRect(QRectF(this->rect()), 8, 8);
}
void QuickTile::mouseReleaseEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton && this->rect().contains(e->position().toPoint()) && this->onClick) this->onClick();
}

// ---------- GenreTile ----------
GenreTile::GenreTile(const QString& genre, QWidget* parent) : QWidget(parent), name(genre){
    this->setFixedSize(200, 112);
    this->setCursor(Qt::PointingHandCursor);
    this->hover.init(this, 180);
}
bool GenreTile::event(QEvent* e){
    this->hover.handle(e, this);
    return QWidget::event(e);
}
void GenreTile::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRectF r(this->rect());
    QPainterPath clip;
    clip.addRoundedRect(r, 10, 10);
    p.setClipPath(clip);
    QColor a = theme::coverColor(this->name, 0);
    QColor b = theme::coverColor(this->name, 50).darker(140);
    QLinearGradient g(r.topLeft(), r.bottomRight());
    g.setColorAt(0.0, theme::mixColor(a, QColor(255, 255, 255), 0.12 * this->hover.t));
    g.setColorAt(1.0, b);
    p.fillRect(r, g);
    p.save();
    p.translate(r.width() - 34 + 6 * this->hover.t, r.height() - 22 - 4 * this->hover.t);
    p.rotate(25);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 55));
    p.drawRoundedRect(QRectF(-34, -34, 68, 68), 8, 8);
    p.restore();
    QFont f = p.font();
    f.setPixelSize(19);
    f.setBold(true);
    p.setFont(f);
    p.setPen(Qt::white);
    p.drawText(QRectF(14, 12, r.width() - 28, 60), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, this->name);
}
void GenreTile::mouseReleaseEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton && this->rect().contains(e->position().toPoint()) && this->onClick) this->onClick();
}

// ---------- SongRow ----------
SongRow::SongRow(const SongInfo& song, int number, bool showAlbum, bool showCover, QWidget* parent)
    : QWidget(parent), info(song), hovered(false), current(false){
    this->setFixedHeight(showCover ? 58 : 48);
    QHBoxLayout* lay = new QHBoxLayout(this);
    lay->setContentsMargins(12, 4, 12, 4);
    lay->setSpacing(12);

    this->cell = new QWidget(this);
    this->cell->setFixedSize(32, 32);
    this->indexLabel = makeLabel(number > 0 ? QString::number(number) : QString(), 14, false, theme::kSubText, this->cell);
    this->indexLabel->setAlignment(Qt::AlignCenter);
    this->indexLabel->setGeometry(0, 0, 32, 32);
    this->playBtn = new IconButton(Icon::Play, 16, this->cell);
    this->playBtn->setColors(theme::kText, theme::kText, theme::kText);
    this->playBtn->move(2, 2);
    this->playBtn->hide();
    this->eq = new EqualizerWidget(this->cell);
    this->eq->move(8, 8);
    this->eq->hide();
    lay->addWidget(this->cell);

    if (showCover){
        CoverWidget* cover = new CoverWidget(44, 5, this);
        cover->setSeed(theme::qs(song.id), theme::qs(song.name));
        lay->addWidget(cover);
    }
    QVBoxLayout* titles = new QVBoxLayout;
    titles->setSpacing(1);
    titles->setContentsMargins(0, 0, 0, 0);
    this->titleLabel = new LinkLabel(this);
    this->titleLabel->setFullText(theme::qs(song.name));
    this->artistLabel = new LinkLabel(this);
    this->artistLabel->setFullText(theme::qs(song.artist));
    this->artistLabel->setStyleSheet("color:#b3b3b3; font-size:13px;");
    this->artistLabel->setClickable(true);
    this->artistLabel->onClick = [this](){
        if (this->onOpenArtist) this->onOpenArtist(this->info.idArtist);
    };
    titles->addStretch();
    titles->addWidget(this->titleLabel);
    titles->addWidget(this->artistLabel);
    titles->addStretch();
    lay->addLayout(titles, 3);

    this->albumLabel = new LinkLabel(this);
    this->albumLabel->setFullText(theme::qs(song.album));
    this->albumLabel->setStyleSheet("color:#b3b3b3; font-size:13px;");
    if (!song.idAlbum.empty()){
        this->albumLabel->setClickable(true);
        this->albumLabel->onClick = [this](){
            if (this->onOpenAlbum) this->onOpenAlbum(this->info.idAlbum);
        };
    }
    this->albumLabel->setVisible(showAlbum);
    lay->addWidget(this->albumLabel, 2);

    this->heartBtn = new IconButton(song.liked ? Icon::HeartFilled : Icon::Heart, 18, this);
    this->heartBtn->setCheckable(true);
    this->heartBtn->setChecked(song.liked);
    this->heartBtn->setColors(theme::kSubText, theme::kText, theme::kAccent);
    lay->addWidget(this->heartBtn);

    QLabel* dur = makeLabel(theme::formatTime(song.duration), 13, false, theme::kSubText, this);
    dur->setFixedWidth(46);
    dur->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lay->addWidget(dur);

    this->moreBtn = new IconButton(Icon::More, 18, this);
    lay->addWidget(this->moreBtn);

    QObject::connect(this->playBtn, &QAbstractButton::clicked, this, [this](){
        if (this->onPlay) this->onPlay(this->info);
    });
    QObject::connect(this->heartBtn, &QAbstractButton::clicked, this, [this](){
        if (!this->onToggleLike) return;
        this->setLiked(this->onToggleLike(this->info));
    });
    QObject::connect(this->moreBtn, &QAbstractButton::clicked, this, [this](){
        if (this->onMore) this->onMore(this->info, this->moreBtn->mapToGlobal(QPoint(0, this->moreBtn->height())));
    });
    this->setLiked(song.liked);
    this->hover.init(this, 120);
    this->updateCell();
    this->setCurrent(false, false);
}
const std::string& SongRow::songId() const{
    return this->info.id;
}
void SongRow::updateCell(){
    bool hov = this->hovered;
    this->indexLabel->setVisible(!hov && !this->current);
    this->eq->setVisible(!hov && this->current);
    this->playBtn->setVisible(hov);
}
void SongRow::setCurrent(bool isCurrent, bool isPlaying){
    this->current = isCurrent;
    this->eq->setPlaying(isCurrent && isPlaying);
    this->titleLabel->setStyleSheet(isCurrent ? "color:#1db954; font-size:15px; font-weight:600;" : "color:#ffffff; font-size:15px; font-weight:600;");
    this->updateCell();
}
void SongRow::setLiked(bool liked){
    this->info.liked = liked;
    this->heartBtn->setChecked(liked);
    this->heartBtn->setGlyph(liked ? Icon::HeartFilled : Icon::Heart);
}
bool SongRow::event(QEvent* e){
    if (e->type() == QEvent::HoverEnter){
        this->hovered = true;
        this->updateCell();
    } else if (e->type() == QEvent::HoverLeave){
        QTimer::singleShot(0, this, [this](){
            if (!cursorInside(this)){
                this->hovered = false;
                this->updateCell();
            }
        });
    }
    this->hover.handle(e, this);
    return QWidget::event(e);
}
void SongRow::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, int(26 * this->hover.t)));
    p.drawRoundedRect(QRectF(this->rect()), 6, 6);
}
void SongRow::mouseDoubleClickEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton && this->onPlay) this->onPlay(this->info);
}
void SongRow::contextMenuEvent(QContextMenuEvent* e){
    if (this->onMore) this->onMore(this->info, e->globalPos());
}

// ---------- SideItem ----------
SideItem::SideItem(Icon navIcon, const QString& text, QWidget* parent)
    : QAbstractButton(parent), libraryMode(false), icon(navIcon), title(text), special(false), specialIcon(Icon::Heart), active(false){
    this->setCursor(Qt::PointingHandCursor);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedHeight(42);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->hover.init(this, 140);
}
SideItem::SideItem(const QString& coverSeed, const QString& text, const QString& sub, QWidget* parent)
    : QAbstractButton(parent), libraryMode(true), icon(Icon::Note), title(text), subtitle(sub), seed(coverSeed), special(false),
      specialIcon(Icon::Heart), active(false){
    this->setCursor(Qt::PointingHandCursor);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedHeight(62);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->hover.init(this, 140);
}
void SideItem::setSpecialCover(Icon coverIcon, const QColor& a, const QColor& b){
    this->special = true;
    this->specialIcon = coverIcon;
    this->colorA = a;
    this->colorB = b;
    this->update();
}
void SideItem::setActive(bool on){
    this->active = on;
    this->update();
}
QSize SideItem::sizeHint() const{
    return QSize(200, this->libraryMode ? 62 : 42);
}
bool SideItem::event(QEvent* e){
    this->hover.handle(e, this);
    return QAbstractButton::event(e);
}
void SideItem::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRectF r(this->rect());
    QColor bg = this->active ? QColor(255, 255, 255, 30) : QColor(255, 255, 255, int(20 * this->hover.t));
    p.setPen(Qt::NoPen);
    p.setBrush(bg);
    p.drawRoundedRect(r.adjusted(2, 1, -2, -1), 8, 8);
    QFont f = p.font();
    if (!this->libraryMode){
        QColor c = (this->active || this->hover.t > 0.5) ? theme::kText : theme::kSubText;
        drawIcon(p, this->icon, QRectF(16, (r.height() - 22) / 2, 22, 22), c);
        f.setPixelSize(15);
        f.setBold(true);
        p.setFont(f);
        p.setPen(c);
        p.drawText(QRectF(52, 0, r.width() - 60, r.height()), Qt::AlignVCenter | Qt::AlignLeft, this->title);
        return;
    }
    QRectF cov(10, (r.height() - 44) / 2, 44, 44);
    QPainterPath clip;
    clip.addRoundedRect(cov, 6, 6);
    p.save();
    p.setClipPath(clip);
    QColor a = this->special ? this->colorA : theme::coverColor(this->seed, 0);
    QColor b = this->special ? this->colorB : theme::coverColor(this->seed, 55).darker(150);
    QLinearGradient g(cov.topLeft(), cov.bottomRight());
    g.setColorAt(0.0, a);
    g.setColorAt(1.0, b);
    p.fillRect(cov, g);
    if (this->special){
        drawIcon(p, this->specialIcon, QRectF(cov.center().x() - 10, cov.center().y() - 10, 20, 20), Qt::white);
    } else{
        QFont lf = p.font();
        lf.setPixelSize(20);
        lf.setBold(true);
        p.setFont(lf);
        p.setPen(QColor(255, 255, 255, 220));
        p.drawText(cov, Qt::AlignCenter, this->title.trimmed().left(1).toUpper());
    }
    p.restore();
    qreal tx = 66;
    qreal tw = r.width() - tx - 8;
    f.setPixelSize(15);
    f.setBold(true);
    p.setFont(f);
    p.setPen(this->active ? theme::kAccent : theme::kText);
    p.drawText(QRectF(tx, 10, tw, 22), Qt::AlignVCenter | Qt::AlignLeft,
               QFontMetrics(f).elidedText(this->title, Qt::ElideRight, (int)tw));
    QFont sf = p.font();
    sf.setPixelSize(12);
    sf.setBold(false);
    p.setFont(sf);
    p.setPen(theme::kSubText);
    p.drawText(QRectF(tx, 32, tw, 20), Qt::AlignVCenter | Qt::AlignLeft,
               QFontMetrics(sf).elidedText(this->subtitle, Qt::ElideRight, (int)tw));
}