#pragma once
#include <QString>
#include <initializer_list>
#include <string>

QString q(const std::string& s) { return QString::fromStdString(s); }
QString fmtSec(int s) { return QString("%1:%2").arg(s / 60).arg(s % 60, 2, 10, QChar('0')); }
QString sym(std::initializer_list<char32_t> cps) {
    QString s;
    for (char32_t c : cps) s += QString::fromUcs4(&c, 1);
    return s;
}
