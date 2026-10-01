#pragma once
#include <vector>
using namespace std;

template <typename T>
bool defaultHigher(const T& a, const T& b);

template <typename T>
class MaxHeap{
    private:
        vector<T> data;
        bool (*isHigher)(const T&, const T&);   // true nếu a ưu tiên hơn b
        void siftUp(int i);
        void siftDown(int i);
    public:
        MaxHeap(bool (*cmp)(const T&, const T&) = nullptr);
        void push(const T& x);
    T    pop();                    // chỉ gọi khi !empty()
        const T& top() const;       // chỉ gọi khi !empty()
        bool empty() const;
        int size() const;
};

template <typename T>
bool defaultHigher(const T& a, const T& b){ 
    return a > b; 
}

template <typename T>
MaxHeap<T>::MaxHeap(bool (*cmp)(const T&, const T&)){
    isHigher = (cmp != nullptr) ? cmp : defaultHigher<T>;
}

template <typename T>
void MaxHeap<T>::siftUp(int i){
    while (i > 0){
        int p = (i - 1) / 2;
        if (!isHigher(data[i], data[p])) break;
        T tmp = data[i]; data[i] = data[p]; data[p] = tmp;
        i = p;
    }
}

template <typename T>
void MaxHeap<T>::siftDown(int i){
    int n = (int)data.size();
    while (true){
        int l = 2 * i + 1, r = 2 * i + 2, best = i;
        if (l < n && isHigher(data[l], data[best])) best = l;
        if (r < n && isHigher(data[r], data[best])) best = r;
        if (best == i) break;
        T tmp = data[i]; data[i] = data[best]; data[best] = tmp;
        i = best;
    }
}

template <typename T>
void MaxHeap<T>::push(const T& x){
    data.push_back(x);
    siftUp((int)data.size() - 1);
}

template <typename T>
T MaxHeap<T>::pop(){
    T res = data[0];
    data[0] = data[data.size() - 1];
    data.pop_back();
    if (!data.empty()) siftDown(0);
    return res;
}

template <typename T>
const T& MaxHeap<T>::top() const{ 
    return data[0]; 
}

template <typename T>
bool MaxHeap<T>::empty() const{ 
    return data.empty(); 
}

template <typename T>
int MaxHeap<T>::size() const{ 
    return (int)data.size(); 
}
