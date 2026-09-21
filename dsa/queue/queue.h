#pragma once
#include <iostream>

class Queue{
    private:
        int* data;
        int capacity;
        int front;
        int rear;
        int size;
    public:
        Queue();
        Queue(int cap);
        ~Queue();
        bool enqueue(int value);
        bool dequeue();
        int peek() const;
        bool isEmpty() const;
        bool isFull() const;
        int getSize() const;
        void clear();
        void display() const;
};
inline Queue::Queue(){
    this->capacity = 10;
    this->data = new int[this->capacity];
    this->front = 0;
    this->rear = -1;
    this->size = 0;
}

inline Queue::Queue(int cap){
    this->capacity = (cap > 0) ? cap : 10;
    this->data = new int[this->capacity];
    this->front = 0;
    this->rear = -1;
    this->size = 0;
}
inline Queue::~Queue(){
    delete[] this->data;
    this->data = nullptr;
}
inline bool Queue::isEmpty() const{
    return this->size == 0;
}
inline bool Queue::isFull() const{
    return this->size == this->capacity;
}
inline int Queue::getSize() const{
    return this->size;
}
inline bool Queue::enqueue(int value){
    if (this->isFull()){
        std::cout << "Queue da day, khong the them\n";
        return false;
    }
    this->rear = (this->rear + 1) % this->capacity;
    this->data[this->rear] = value;
    this->size++;
    return true;
}
inline bool Queue::dequeue(){
    if (this->isEmpty()) {
        std::cout << "Queue rong, khong the lay phan tu\n";
        return false;
    }
    this->front = (this->front + 1) % this->capacity;
    this->size--;
    return true;
}
inline int Queue::peek() const{
    if (this->isEmpty()) {
        std::cout << "Queue rong\n";
        return -1;
    }
    return this->data[this->front];
}
inline void Queue::clear(){
    this->front = 0;
    this->rear = -1;
    this->size = 0;
}
inline void Queue::display() const{
    if (this->isEmpty()) {
        std::cout << "Queue rong\n";
        return;
    }
    std::cout << "Queue: ";
    for (int i = 0; i < this->size; i++){
        int index = (this->front + i) % this->capacity;
        std::cout << this->data[index] << " ";
    }
    std::cout << "\n";
}