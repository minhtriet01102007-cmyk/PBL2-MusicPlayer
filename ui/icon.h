#pragma once
#include <QAbstractButton>
#include <QColor>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QRectF>
#include <QVariantAnimation>
#include "theme.h"

// Bộ biểu tượng vẽ bằng QPainter (vector, sắc nét ở mọi kích thước, không cần file ảnh).
enum class Icon{
    Home, Search, Library, Heart, HeartFilled, Play, Pause, Next, Prev, Shuffle, Repeat, RepeatOne,
    Volume, Mute, Queue, Plus, More, Back, Forward, Close, Logout, User, Note, Trash, Edit, Eye, EyeOff,
    Shield, Mic, Album, Disc
};

// Vẽ biểu tượng trong hình chữ nhật r bằng màu color (toạ độ thiết kế 24x24)
void drawIcon(QPainter& p, Icon icon, const QRectF& r, const QColor& color);

// Nút chỉ có biểu tượng: đổi màu mượt khi rê chuột, hỗ trợ trạng thái bật/tắt (checkable) và nền tròn (nút Play)
class IconButton : public QAbstractButton{
    private:
        Icon glyphType;
        int glyph;                 // kích thước biểu tượng
        QColor normalColor;
        QColor hoverColor;
        QColor activeColor;        // màu khi checked
        QColor circleColor;
        bool circle;
        int diameter;
        qreal hoverT;
        QVariantAnimation* anim;
        void animateTo(qreal target);
    protected:
        void paintEvent(QPaintEvent* e) override;
        bool event(QEvent* e) override;
    public:
        explicit IconButton(Icon icon, int glyphSize = 20, QWidget* parent = nullptr);
        void setGlyph(Icon icon);
        void setColors(const QColor& normal, const QColor& hover, const QColor& active);
        void setCircle(const QColor& fill, int circleDiameter);
        QSize sizeHint() const override;
};

void drawIcon(QPainter& p, Icon icon, const QRectF& r, const QColor& color){
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.translate(r.topLeft());
    p.scale(r.width() / 24.0, r.height() / 24.0);
    QPen pen(color, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen thin(color, 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    switch (icon){
        case Icon::Home:{
            QPolygonF poly;
            poly << QPointF(12, 3) << QPointF(2.5, 11.5) << QPointF(5, 11.5) << QPointF(5, 20.5) << QPointF(10, 20.5)
                 << QPointF(10, 14.5) << QPointF(14, 14.5) << QPointF(14, 20.5) << QPointF(19, 20.5) << QPointF(19, 11.5)
                 << QPointF(21.5, 11.5);
            p.drawPolygon(poly);
            break;
        }
        case Icon::Search:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(QPointF(10.5, 10.5), 6.3, 6.3);
            p.drawLine(QPointF(15.3, 15.3), QPointF(21, 21));
            break;
        }
        case Icon::Library:{
            p.drawRoundedRect(QRectF(3.5, 4, 3.2, 16), 1, 1);
            p.drawRoundedRect(QRectF(9.5, 4, 3.2, 16), 1, 1);
            QPolygonF poly;
            poly << QPointF(15.5, 5.5) << QPointF(18.6, 4.7) << QPointF(21.8, 18.5) << QPointF(18.7, 19.3);
            p.drawPolygon(poly);
            break;
        }
        case Icon::Heart:
        case Icon::HeartFilled:{
            QPainterPath path;
            path.moveTo(12, 20.5);
            path.cubicTo(4.5, 14.5, 2.2, 10.5, 4.2, 7.2);
            path.cubicTo(6, 4.4, 10, 4.6, 12, 8);
            path.cubicTo(14, 4.6, 18, 4.4, 19.8, 7.2);
            path.cubicTo(21.8, 10.5, 19.5, 14.5, 12, 20.5);
            path.closeSubpath();
            if (icon == Icon::Heart){
                p.setPen(pen);
                p.setBrush(Qt::NoBrush);
            }
            p.drawPath(path);
            break;
        }
        case Icon::Play:{
            QPolygonF poly;
            poly << QPointF(7, 4) << QPointF(20, 12) << QPointF(7, 20);
            p.drawPolygon(poly);
            break;
        }
        case Icon::Pause:{
            p.drawRoundedRect(QRectF(6, 4, 4, 16), 1.2, 1.2);
            p.drawRoundedRect(QRectF(14, 4, 4, 16), 1.2, 1.2);
            break;
        }
        case Icon::Next:{
            QPolygonF poly;
            poly << QPointF(5.5, 5) << QPointF(16.5, 12) << QPointF(5.5, 19);
            p.drawPolygon(poly);
            p.drawRoundedRect(QRectF(17.2, 5, 2.6, 14), 1, 1);
            break;
        }
        case Icon::Prev:{
            QPolygonF poly;
            poly << QPointF(18.5, 5) << QPointF(7.5, 12) << QPointF(18.5, 19);
            p.drawPolygon(poly);
            p.drawRoundedRect(QRectF(4.2, 5, 2.6, 14), 1, 1);
            break;
        }
        case Icon::Shuffle:{
            p.setPen(thin);
            p.setBrush(Qt::NoBrush);
            QPainterPath a;
            a.moveTo(3, 7);
            a.lineTo(7, 7);
            a.cubicTo(12, 7, 11, 17, 16, 17);
            a.lineTo(19, 17);
            p.drawPath(a);
            QPainterPath b;
            b.moveTo(3, 17);
            b.lineTo(7, 17);
            b.cubicTo(9, 17, 10, 16, 10.8, 14.6);
            p.drawPath(b);
            QPainterPath c;
            c.moveTo(13.2, 9.4);
            c.cubicTo(14, 8, 15, 7, 17, 7);
            c.lineTo(19, 7);
            p.drawPath(c);
            p.setPen(Qt::NoPen);
            p.setBrush(color);
            QPolygonF h1;
            h1 << QPointF(17.5, 3.8) << QPointF(21.5, 7) << QPointF(17.5, 10.2);
            p.drawPolygon(h1);
            QPolygonF h2;
            h2 << QPointF(17.5, 13.8) << QPointF(21.5, 17) << QPointF(17.5, 20.2);
            p.drawPolygon(h2);
            break;
        }
        case Icon::Repeat:
        case Icon::RepeatOne:{
            p.setPen(thin);
            p.setBrush(Qt::NoBrush);
            QPainterPath a;
            a.moveTo(4.5, 13);
            a.lineTo(4.5, 10.5);
            a.quadTo(4.5, 7, 8, 7);
            a.lineTo(17, 7);
            p.drawPath(a);
            QPainterPath b;
            b.moveTo(19.5, 11);
            b.lineTo(19.5, 13.5);
            b.quadTo(19.5, 17, 16, 17);
            b.lineTo(7, 17);
            p.drawPath(b);
            p.setPen(Qt::NoPen);
            p.setBrush(color);
            QPolygonF h1;
            h1 << QPointF(16.5, 3.6) << QPointF(21, 7) << QPointF(16.5, 10.4);
            p.drawPolygon(h1);
            QPolygonF h2;
            h2 << QPointF(7.5, 13.6) << QPointF(3, 17) << QPointF(7.5, 20.4);
            p.drawPolygon(h2);
            if (icon == Icon::RepeatOne){
                QFont f;
                f.setPixelSize(9);
                f.setBold(true);
                p.setFont(f);
                p.drawText(QRectF(8.5, 8.5, 7, 7), Qt::AlignCenter, "1");
            }
            break;
        }
        case Icon::Volume:
        case Icon::Mute:{
            QPolygonF poly;
            poly << QPointF(3, 9.5) << QPointF(7, 9.5) << QPointF(12, 5) << QPointF(12, 19) << QPointF(7, 14.5) << QPointF(3, 14.5);
            p.drawPolygon(poly);
            p.setPen(thin);
            p.setBrush(Qt::NoBrush);
            if (icon == Icon::Volume){
                p.drawArc(QRectF(9, 8, 9, 8), -50 * 16, 100 * 16);
                p.drawArc(QRectF(7, 4.5, 14, 15), -50 * 16, 100 * 16);
            } else{
                p.drawLine(QPointF(15.5, 9.5), QPointF(21, 15));
                p.drawLine(QPointF(21, 9.5), QPointF(15.5, 15));
            }
            break;
        }
        case Icon::Queue:{
            p.setPen(pen);
            p.drawLine(QPointF(3.5, 6), QPointF(15, 6));
            p.drawLine(QPointF(3.5, 12), QPointF(15, 12));
            p.drawLine(QPointF(3.5, 18), QPointF(10, 18));
            p.setPen(Qt::NoPen);
            p.setBrush(color);
            QPolygonF poly;
            poly << QPointF(16.5, 13.5) << QPointF(21.5, 17.5) << QPointF(16.5, 21.5);
            p.drawPolygon(poly);
            break;
        }
        case Icon::Plus:{
            p.setPen(pen);
            p.drawLine(QPointF(12, 5), QPointF(12, 19));
            p.drawLine(QPointF(5, 12), QPointF(19, 12));
            break;
        }
        case Icon::More:{
            p.drawEllipse(QPointF(5, 12), 1.9, 1.9);
            p.drawEllipse(QPointF(12, 12), 1.9, 1.9);
            p.drawEllipse(QPointF(19, 12), 1.9, 1.9);
            break;
        }
        case Icon::Back:{
            p.setPen(pen);
            QPainterPath path;
            path.moveTo(15, 5);
            path.lineTo(8, 12);
            path.lineTo(15, 19);
            p.drawPath(path);
            break;
        }
        case Icon::Forward:{
            p.setPen(pen);
            QPainterPath path;
            path.moveTo(9, 5);
            path.lineTo(16, 12);
            path.lineTo(9, 19);
            p.drawPath(path);
            break;
        }
        case Icon::Close:{
            p.setPen(pen);
            p.drawLine(QPointF(6, 6), QPointF(18, 18));
            p.drawLine(QPointF(18, 6), QPointF(6, 18));
            break;
        }
        case Icon::Logout:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            QPainterPath door;
            door.moveTo(10, 4);
            door.lineTo(5, 4);
            door.lineTo(5, 20);
            door.lineTo(10, 20);
            p.drawPath(door);
            p.drawLine(QPointF(9.5, 12), QPointF(20, 12));
            QPainterPath arrow;
            arrow.moveTo(16, 8);
            arrow.lineTo(20, 12);
            arrow.lineTo(16, 16);
            p.drawPath(arrow);
            break;
        }
        case Icon::User:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(QPointF(12, 8), 4, 4);
            QPainterPath body;
            body.moveTo(4, 21);
            body.cubicTo(4, 16, 8, 14.5, 12, 14.5);
            body.cubicTo(16, 14.5, 20, 16, 20, 21);
            p.drawPath(body);
            break;
        }
        case Icon::Note:{
            p.drawEllipse(QPointF(8, 18), 3.4, 2.8);
            p.setPen(pen);
            p.drawLine(QPointF(11.3, 18), QPointF(11.3, 4.5));
            QPainterPath flag;
            flag.moveTo(11.3, 4.5);
            flag.cubicTo(13, 7.5, 18, 6.5, 18.5, 11);
            p.setBrush(Qt::NoBrush);
            p.drawPath(flag);
            break;
        }
        case Icon::Trash:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawLine(QPointF(4, 7), QPointF(20, 7));
            QPainterPath lid;
            lid.moveTo(9, 7);
            lid.lineTo(9, 4);
            lid.lineTo(15, 4);
            lid.lineTo(15, 7);
            p.drawPath(lid);
            QPainterPath body;
            body.moveTo(6, 7);
            body.lineTo(7, 20);
            body.lineTo(17, 20);
            body.lineTo(18, 7);
            p.drawPath(body);
            break;
        }
        case Icon::Edit:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            QPolygonF poly;
            poly << QPointF(4, 20) << QPointF(4, 16) << QPointF(15, 5) << QPointF(19, 9) << QPointF(8, 20);
            p.drawPolygon(poly);
            break;
        }
        case Icon::Eye:
        case Icon::EyeOff:{
            p.setPen(thin);
            p.setBrush(Qt::NoBrush);
            QPainterPath eye;
            eye.moveTo(2, 12);
            eye.quadTo(12, 3, 22, 12);
            eye.quadTo(12, 21, 2, 12);
            p.drawPath(eye);
            p.drawEllipse(QPointF(12, 12), 3, 3);
            if (icon == Icon::EyeOff) p.drawLine(QPointF(4, 20), QPointF(20, 4));
            break;
        }
        case Icon::Shield:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            QPainterPath path;
            path.moveTo(12, 3);
            path.lineTo(20, 6);
            path.lineTo(20, 12);
            path.cubicTo(20, 17, 16, 20, 12, 21);
            path.cubicTo(8, 20, 4, 17, 4, 12);
            path.lineTo(4, 6);
            path.closeSubpath();
            p.drawPath(path);
            break;
        }
        case Icon::Mic:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(QRectF(9, 3, 6, 12), 3, 3);
            QPainterPath path;
            path.moveTo(5.5, 11);
            path.cubicTo(5.5, 17, 18.5, 17, 18.5, 11);
            p.drawPath(path);
            p.drawLine(QPointF(12, 17.5), QPointF(12, 21));
            break;
        }
        case Icon::Album:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(QRectF(3.5, 3.5, 17, 17), 3, 3);
            p.drawEllipse(QPointF(12, 12), 4, 4);
            break;
        }
        case Icon::Disc:{
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(QPointF(12, 12), 9, 9);
            p.setPen(Qt::NoPen);
            p.setBrush(color);
            p.drawEllipse(QPointF(12, 12), 2.2, 2.2);
            break;
        }
    }
    p.restore();
}

IconButton::IconButton(Icon icon, int glyphSize, QWidget* parent) : QAbstractButton(parent){
    this->glyphType = icon;
    this->glyph = glyphSize;
    this->normalColor = theme::kSubText;
    this->hoverColor = theme::kText;
    this->activeColor = theme::kAccent;
    this->circleColor = QColor(255, 255, 255);
    this->circle = false;
    this->diameter = 0;
    this->hoverT = 0.0;
    this->setCursor(Qt::PointingHandCursor);
    this->setAttribute(Qt::WA_Hover, true);
    this->setFocusPolicy(Qt::NoFocus);
    this->anim = new QVariantAnimation(this);
    this->anim->setDuration(150);
    this->anim->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(this->anim, &QVariantAnimation::valueChanged, this, [this](const QVariant& v){
        this->hoverT = v.toReal();
        this->update();
    });
    this->setFixedSize(this->sizeHint());
}
void IconButton::animateTo(qreal target){
    this->anim->stop();
    this->anim->setStartValue(this->hoverT);
    this->anim->setEndValue(target);
    this->anim->start();
}
void IconButton::setGlyph(Icon icon){
    this->glyphType = icon;
    this->update();
}
void IconButton::setColors(const QColor& normal, const QColor& hover, const QColor& active){
    this->normalColor = normal;
    this->hoverColor = hover;
    this->activeColor = active;
    this->update();
}
void IconButton::setCircle(const QColor& fill, int circleDiameter){
    this->circle = true;
    this->circleColor = fill;
    this->diameter = circleDiameter;
    this->setFixedSize(this->sizeHint());
    this->update();
}
QSize IconButton::sizeHint() const{
    int side = this->circle ? this->diameter + 6 : this->glyph + 12;
    return QSize(side, side);
}
bool IconButton::event(QEvent* e){
    if (e->type() == QEvent::HoverEnter) this->animateTo(1.0);
    else if (e->type() == QEvent::HoverLeave) this->animateTo(0.0);
    return QAbstractButton::event(e);
}
void IconButton::paintEvent(QPaintEvent*){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    qreal scale = 1.0 + (this->circle ? 0.07 * this->hoverT : 0.0) - (this->isDown() ? 0.07 : 0.0);
    QPointF c = QRectF(this->rect()).center();
    QColor col;
    if (this->circle){
        p.setPen(Qt::NoPen);
        p.setBrush(this->circleColor);
        qreal d = this->diameter * scale;
        p.drawEllipse(QRectF(c.x() - d / 2, c.y() - d / 2, d, d));
        col = this->normalColor;
    } else{
        col = this->isChecked() ? this->activeColor : theme::mixColor(this->normalColor, this->hoverColor, this->hoverT);
    }
    if (!this->isEnabled()) col.setAlpha(90);
    qreal g = this->glyph * (this->circle ? 1.0 : scale);
    drawIcon(p, this->glyphType, QRectF(c.x() - g / 2, c.y() - g / 2, g, g), col);
}