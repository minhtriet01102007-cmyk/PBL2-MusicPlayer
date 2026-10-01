#pragma once
#include <vector>
using namespace std;

template <typename T>
class Queue{
    private:
        vector<T> data;
        int head;
        int count;
        void grow();
    public:
        Queue();
        void enqueue(const T& x);
        T dequeue();                // chỉ gọi khi !empty()
        const T& front() const;     // chỉ gọi khi !empty()
        bool empty() const;
        int size() const;
};

template <typename T>
Queue<T>::Queue() : data(8), head(0), count(0){
}

template <typename T>
void Queue<T>::grow(){
    int cap = (int)data.size();
    vector<T> bigger(cap * 2);
    for (int i = 0; i < count; i++) bigger[i] = data[(head + i) % cap];
    data = bigger;
    head = 0;
}

template <typename T>
void Queue<T>::enqueue(const T& x){
    if (count == (int)data.size()) grow();
    data[(head + count) % data.size()] = x;
    count++;
}

template <typename T>
T Queue<T>::dequeue(){
    T res = data[head];
    head = (head + 1) % data.size();
    count--;
    return res;
}

template <typename T>
const T& Queue<T>::front() const{ 
    return data[head]; 
}

template <typename T>
bool Queue<T>::empty() const{ 
    return count == 0; 
}

template <typename T>
int Queue<T>::size() const{ 
    return count; 
}
