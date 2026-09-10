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
        Queue(int capacity);
        ~Queue();
        void enqueue(int value);
        void dequeue();
        bool IsEmpty() const;
        bool IsFull() const;
        int getFront() const;
        int getSize() const;
        void display() const;
};
inline Queue::Queue(){
    capacity = 10;
    front = 0;
    rear = -1;
    size = 0;
    data = new int[capacity];
}
inline Queue::Queue(int capacity){
    this->capacity = (capacity > 0) ? capacity : 10;
    this->front = 0;
    this->rear = -1;
    this->size = 0;
    this->data = new int[this->capacity];
}
inline Queue::~Queue(){
    delete[] data;
    data = nullptr;
}
inline void Queue::enqueue(int value){
    if (IsFull()){
        std::cout << "Queue da day, khong the them" << std::endl;
        return;
    }
    rear = (rear + 1) % capacity;
    data[rear] = value;
    size++;
}
inline void Queue::dequeue(){
    if (IsEmpty()){
        std::cout << "Queue rong, khong the xoa" << std::endl;
        return;
    }
    front = (front + 1) % capacity;
    size--;
}
inline bool Queue::IsEmpty() const{
    return this->size == 0;
}
inline bool Queue::IsFull() const{
    return this->size == capacity;
}
inline int Queue::getFront() const{
    if (IsEmpty()){
        std::cout << "Queue rong" << std::endl;
        return -1;
    }
    return data[front];
}
inline int Queue::getSize() const{
    return this->size;
}
inline void Queue::display() const{
    if (IsEmpty()){
        std::cout << "Queue rong" << std::endl;
        return;
    }
    for (int i=0; i<size; i++){
        int index = (front + i) % capacity;
        std::cout << data[index] << " ";
    }
    std::cout << std::endl;
}


