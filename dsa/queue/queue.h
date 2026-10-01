#pragma once
#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> data;
    int head;
    int count;

    void grow();

public:
    Queue();
    void enqueue(const T& x);
    T dequeue();
    const T& front() const;
    T& front();
    bool empty() const;
    int size() const;
    void clear();
};

template <typename T>
Queue<T>::Queue() : data(8), head(0), count(0) {}

template <typename T>
void Queue<T>::grow() {
    int cap = static_cast<int>(data.size());
    std::vector<T> bigger(cap * 2);
    for (int i = 0; i < count; i++) {
        bigger[i] = data[(head + i) % cap];
    }
    data = bigger;
    head = 0;
}

template <typename T>
void Queue<T>::enqueue(const T& x) {
    if (count == static_cast<int>(data.size())) {
        grow();
    }
    data[(head + count) % data.size()] = x;
    count++;
}

template <typename T>
T Queue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    T res = data[head];
    head = (head + 1) % data.size();
    count--;
    return res;
}

template <typename T>
const T& Queue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    return data[head];
}

template <typename T>
T& Queue<T>::front() {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    return data[head];
}

template <typename T>
bool Queue<T>::empty() const {
    return count == 0;
}

template <typename T>
int Queue<T>::size() const {
    return count;
}

template <typename T>
void Queue<T>::clear() {
    head = 0;
    count = 0;
}
