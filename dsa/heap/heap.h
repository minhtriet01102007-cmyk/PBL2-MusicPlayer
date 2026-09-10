#pragma once
#include <iostream>

class MaxHeap{
    private:
        int* data;
        int capacity;
        int size;
        int parent(int i) const;
        int leftChild(int i) const;
        int rightChild(int i) const;
        void heapifyUp(int index);
        void heapifyDown(int index);
        void swap(int& a, int& b);
    public:
        MaxHeap();
        MaxHeap(int capacity);
        ~MaxHeap();
        void insert(int value);
        int extractMax();        // Lấy và xóa phần tử lớn nhất (gốc)
        int getMax() const;      // Chỉ xem phần tử lớn nhất
        bool IsEmpty() const;
        bool IsFull() const;
        int getSize() const;
        void display() const;
        void clear();
};
inline MaxHeap::MaxHeap(){
    this->capacity = 10;
    this->size = 0;
    this->data = new int[this->capacity];
}
inline MaxHeap::MaxHeap(int capacity){
    this->capacity = (capacity > 0) ? capacity : 10;
    this->size = 0;
    this->data = new int[this->capacity];
}
inline MaxHeap::~MaxHeap(){
    delete[] this->data;
    this->data = nullptr;
}
inline int MaxHeap::parent(int i) const{
    return (i - 1) / 2;
}
inline int MaxHeap::leftChild(int i) const{
    return 2 * i + 1;
}
inline int MaxHeap::rightChild(int i) const{
    return 2 * i + 2;
}
inline void MaxHeap::swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}
inline bool MaxHeap::IsEmpty() const{
    return this->size == 0;
}
inline bool MaxHeap::IsFull() const{
    return this->size == this->capacity;
}
inline int MaxHeap::getSize() const{
    return this->size;
}
inline void MaxHeap::heapifyUp(int index){
    while (index > 0 && this->data[parent(index)] < this->data[index]){
        swap(this->data[parent(index)], this->data[index]);
        index = parent(index);
    }
}
inline void MaxHeap::insert(int value) {
    if (IsFull()){
        std::cout << "Heap da day" << std::endl;
        return;
    }
    this->data[this->size] = value;
    this->size++;
    heapifyUp(this->size - 1);
}
inline void MaxHeap::heapifyDown(int index){
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    if (left < this->size && this->data[left] > this->data[largest]){
        largest = left;
    }
    if (right < this->size && this->data[right] > this->data[largest]){
        largest = right;
    }
    if (largest != index){
        swap(this->data[index], this->data[largest]);
        heapifyDown(largest);
    }
}
inline int MaxHeap::getMax() const{
    if (IsEmpty()){
        std::cout << "Heap rong" << std::endl;
        return -1;
    }
    return this->data[0];
}
inline int MaxHeap::extractMax(){
    if (IsEmpty()){
        std::cout << "Heap rong" << std::endl;
        return -1;
    }
    int maxValue = this->data[0];
    this->data[0] = this->data[this->size - 1];
    this->size--;
    heapifyDown(0);
    return maxValue;
}
inline void MaxHeap::clear(){
    this->size = 0;
}
inline void MaxHeap::display() const{
    if (IsEmpty()){
        std::cout << "Heap rong" << std::endl;
        return;
    }
    std::cout << "Max-Heap: ";
    for (int i=0; i<this->size; i++){
        std::cout << this->data[i] << " ";
    }
    std::cout << std::endl;
}
