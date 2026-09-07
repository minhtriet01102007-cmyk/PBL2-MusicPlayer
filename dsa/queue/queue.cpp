#include "queue.h"
#include <iostream>

Queue::Queue(){
    capacity = 10;
    front = 0;
    rear = -1;
    size = 0;
    data = new int[capacity];
}
Queue::Queue(int capacity){
    if (capacity <= 0){
        capacity = 10;
    }
    this->capacity = capacity;
    front = 0;
    rear = -1;
    size = 0;
    data = new int[this->capacity];
}
Queue::~Queue(){
    delete[] data;
}
void Queue::enqueue(int value){ //Thêm vào queue
    if (IsFull()){
        std::cout << "Queue da day" << std::endl;
        return;
    }
    rear = (rear + 1) % capacity;
    data[rear] = value;
    size++;
}
void Queue::dequeue(){
    if (IsEmpty()){
        std::cout << "Queue rong" << std::endl;
        return;
    }
    front = (front + 1) % capacity;
    size--;
}
int Queue::getFront() const{
    if (IsEmpty()) return -1;
    return data[front];
}
bool Queue::IsEmpty() const{
    return size == 0;
}
bool Queue::IsFull() const{
    return size == capacity;
}
int Queue::getSize() const{
    return this->size;
}
void Queue::display() const{
    if (IsEmpty()){
        std::cout << "Queue rong!" << std::endl;
        return;
    }
    for (int i=0; i<size; i++){
        int index = (front + i) % capacity;
        std::cout << data[index] << " ";
    }
    std::cout << std::endl;
}