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
        void resize();
    public:
        MaxHeap();
        MaxHeap(int cap);
        ~MaxHeap();
        void insert(int value);
        int extractMax();
        int getMax() const;
        bool empty() const;
        bool isFull() const;
        int getSize() const;
        void display() const;
        void clear();
};
inline MaxHeap::MaxHeap(){
    this->capacity = 10;
    this->size = 0;
    this->data = new int[this->capacity];
}
inline MaxHeap::MaxHeap(int cap){
    this->capacity = (cap > 0) ? cap : 10;
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
inline bool MaxHeap::empty() const{
    return this->size == 0;
}
inline bool MaxHeap::isFull() const{
    return this->size == this->capacity;
}
inline int MaxHeap::getSize() const{
    return this->size;
}
inline void MaxHeap::resize(){
    this->capacity *= 2;
    int* newData = new int[this->capacity];
    for (int i = 0; i < this->size; i++){ // Copy dữ liệu cũ
        newData[i] = this->data[i];
    }
    delete[] this->data;
    this->data = newData;
}
inline void MaxHeap::heapifyUp(int index){ //Sau khi thêm một phần tử, đưa phần tử đó lên trên nếu nó lớn hơn cha
    while (index > 0 && this->data[parent(index)] < this->data[index]){
        this->swap(this->data[parent(index)], this->data[index]);
        index = parent(index);
    }
}
inline void MaxHeap::insert(int value){ 
    if (this->isFull()){
        this->resize(); // Tự động tăng dung lượng khi đầy
    }
    this->data[this->size] = value; // Thêm phần tử vào cuối heap
    this->size++;
    this->heapifyUp(this->size - 1);
}
inline void MaxHeap::heapifyDown(int index){
    int largest = index; // node hiện tại lớn nhất
    int left = this->leftChild(index);
    int right = this->rightChild(index);
    if (left < this->size && this->data[left] > this->data[largest]){
        largest = left;
    }
    if (right < this->size && this->data[right] > this->data[largest]){
        largest = right;
    }
    if (largest != index){
        this->swap(this->data[index], this->data[largest]);
        this->heapifyDown(largest); // tiếp tục kiểm tra ở vị trí mới
    }
}
inline int MaxHeap::getMax() const{
    if (this->empty()){
        std::cout << "Heap rong" << std::endl;
        return -1;
    }
    return this->data[0];
}
inline int MaxHeap::extractMax(){ // Lấy và xóa phần tử lớn nhất
    if (this->empty()){
        std::cout << "Heap rong, khong the lay phan tu" << std::endl;
        return -1;
    }
    int maxValue = this->data[0];
    this->data[0] = this->data[this->size - 1]; // lấy phần tử cuối đưa lên đầu
    this->size--;
    this->heapifyDown(0);
    return maxValue;
}
inline void MaxHeap::clear(){
    this->size = 0;
}
inline void MaxHeap::display() const{
    if (this->empty()){
        std::cout << "Heap rong" << std::endl;
        return;
    }
    std::cout << "Max-Heap: ";
    for (int i = 0; i < this->size; i++){
        std::cout << this->data[i] << " ";
    }
    std::cout << "\n";
}
