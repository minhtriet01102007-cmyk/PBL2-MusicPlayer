#pragma once
#include <vector> // dùng std::vector<T> để lưu các phần tử Heap.
#include <algorithm> // cung cấp std::swap() để đổi chỗ hai phần tử
#include <stdexcept>

template <typename T, typename Comparator> // Comparator: kiểu đối tượng dùng để so sánh hai phần tử
class MaxHeap{
    private:
        std::vector<T> heap; // mảng lưu các phần tử
        Comparator compare; // đối tượng so sánh hai phần tử
        void heapifyUp(int index);
        void heapifyDown(int index);
    public:
        MaxHeap(Comparator comp = Comparator()); //k truyền đối số => đối tượng Comparator mặc định
        void insert(const T& val);
        T extractMax(); // Lấy và xóa phần tử có ưu tiên cao nhất
        bool isEmpty() const;
        int size() const;
};

template <typename T, typename Comparator>
MaxHeap<T, Comparator>::MaxHeap(Comparator comp){
    this->compare = comp;
}

template <typename T, typename Comparator>
void MaxHeap<T, Comparator>::heapifyUp(int index){
    while (index > 0){
        int parent = (index - 1) / 2;
        if (this->compare(this->heap[index], this->heap[parent])){ // Kiểm tra phần tử hiện tại có ưu tiên cao hơn Node cha không
            std::swap(this->heap[index], this->heap[parent]);
            index = parent;
        } else break;
    }
}

template <typename T, typename Comparator>
void MaxHeap<T, Comparator>::heapifyDown(int index){
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;
    if (left < (int)this->heap.size() && this->compare(this->heap[left], this->heap[largest])){
        largest = left;
    }
    if (right < (int)this->heap.size() && this->compare(this->heap[right], this->heap[largest])){
        largest = right;
    }
    if (largest != index){
        std::swap(this->heap[index], this->heap[largest]);
        this->heapifyDown(largest);
    }
}

template <typename T, typename Comparator>
void MaxHeap<T, Comparator>::insert(const T& val){
    this->heap.push_back(val);
    this->heapifyUp((int)this->heap.size() - 1);
}

template <typename T, typename Comparator>
T MaxHeap<T, Comparator>::extractMax(){
    if (this->heap.empty()){
        throw std::runtime_error("Heap rỗng");
    }
    T maxVal = this->heap[0]; // lưu phần tử lớn nhất
    this->heap[0] = this->heap.back(); // lấy phần tử cuối, gán phần tử cuối vào vị trí gốc
    this->heap.pop_back(); // xóa phần tử cuối cùng
    if (!this->heap.empty()){
        this->heapifyDown(0);
    }
    return maxVal;
}

template <typename T, typename Comparator>
bool MaxHeap<T, Comparator>::isEmpty() const{
    return this->heap.empty();
}

template <typename T, typename Comparator>
int MaxHeap<T, Comparator>::size() const{
    return (int)this->heap.size();
}