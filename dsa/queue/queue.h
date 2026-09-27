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
Queue::Queue(int cap){
    this->capacity = (cap > 0) ? cap : 10;
    this->data = new int[this->capacity];
    this->front = 0;
    this->rear = -1;
    this->size = 0;
}
Queue::~Queue(){
    delete[] this->data;
    this->data = nullptr;
}
bool Queue::isEmpty() const{
    return this->size == 0;
}
bool Queue::isFull() const{
    return this->size == this->capacity;
}
int Queue::getSize() const{
    return this->size;
}
bool Queue::enqueue(int value){ //Thêm vào cuối 
    if (this->isFull()){
        std::cout << "Queue da day, khong the them\n";
        return false;
    }
    this->rear = (this->rear + 1) % this->capacity;
    this->data[this->rear] = value;
    this->size++;
    return true;
}
bool Queue::dequeue(){ //Lấy đầu
    if (this->isEmpty()){
        std::cout << "Queue rong, khong the lay phan tu\n";
        return false;
    }
    this->front = (this->front + 1) % this->capacity;
    this->size--;
    return true;
}
int Queue::peek() const{ //xem phần tử đầu Queue nhưng k xóa nó
    if (this->isEmpty()){
        std::cout << "Queue rong\n";
        return -1;
    }
    return this->data[this->front];
}
void Queue::clear(){
    this->front = 0;
    this->rear = -1;
    this->size = 0;
}
void Queue::display() const{
    if (this->isEmpty()){
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
