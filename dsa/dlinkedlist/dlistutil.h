#pragma once
#include <exception>
#include <vector>
#include "dlinkedlist.h"

namespace dlist{
template <typename T>
bool seekTo(DoublyLinkedList<T>& l, const T& target){ // Đưa con trỏ phát (current) tới phần tử target
    if (l.isEmpty()) return false;
    l.resetPlayback();
    for (int i = 0; i < l.getSize(); ++i){
        if (l.getCurrent() == target) return true;
        if (!l.hasNext()) break;
        l.next();
    }
    return false;
}

template <typename T>
std::vector<T> toVector(DoublyLinkedList<T>& l){ // Liệt kê toàn bộ phần tử ra vector, giữ nguyên vị trí đang phát
    std::vector<T> v;
    if (l.isEmpty()) return v;
    bool hasCur = false;
    T cur = T();
    try{
        cur = l.getCurrent();
        hasCur = true;
    }
    catch (const std::exception&){
    }
    l.resetPlayback();
    for (int i = 0; i < l.getSize(); ++i){
        v.push_back(l.getCurrent());
        if (i + 1 < l.getSize()) l.next();
    }
    if (hasCur) seekTo(l, cur);
    return v;
}
}
