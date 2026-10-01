#pragma once
#include <vector>
#include <stdexcept>
#include <utility> // std::swap

template <typename T>
class MaxHeap {
private:
    std::vector<T> data;
    bool (*isHigher)(const T&, const T&); // true nếu a có độ ưu tiên cao hơn b
    void siftUp(int i);
    void siftDown(int i);
    static bool defaultCmp(const T& a, const T& b){
        return a > b;
    }

public:
    MaxHeap(bool (*cmp)(const T&, const T&) = nullptr);
    void push(const T& x);
    T pop();
    const T& top() const;
    bool empty() const;
    int size() const;
    void clear();
};

template <typename T>
MaxHeap<T>::MaxHeap(bool (*cmp)(const T&, const T&)) {
    isHigher = (cmp != nullptr) ? cmp : defaultCmp;
}

template <typename T>
void MaxHeap<T>::siftUp(int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!isHigher(data[i], data[p])) break;
        std::swap(data[i], data[p]);
        i = p;
    }
}

template <typename T>
void MaxHeap<T>::siftDown(int i) {
    int n = static_cast<int>(data.size());
    while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int best = i;

        if (l < n && isHigher(data[l], data[best])) best = l;
        if (r < n && isHigher(data[r], data[best])) best = r;

        if (best == i) break;
        std::swap(data[i], data[best]);
        i = best;
    }
}

template <typename T>
void MaxHeap<T>::push(const T& x) {
    data.push_back(x);
    siftUp(static_cast<int>(data.size()) - 1);
}

template <typename T>
T MaxHeap<T>::pop() {
    if (empty()) {
        throw std::out_of_range("MaxHeap is empty");
    }
    T res = data[0];
    data[0] = data.back();
    data.pop_back();
    if (!data.empty()) {
        siftDown(0);
    }
    return res;
}

template <typename T>
const T& MaxHeap<T>::top() const {
    if (empty()) {
        throw std::out_of_range("MaxHeap is empty");
    }
    return data[0];
}

template <typename T>
bool MaxHeap<T>::empty() const {
    return data.empty();
}

template <typename T>
int MaxHeap<T>::size() const {
    return static_cast<int>(data.size());
}

template <typename T>
void MaxHeap<T>::clear() {
    data.clear();
}
