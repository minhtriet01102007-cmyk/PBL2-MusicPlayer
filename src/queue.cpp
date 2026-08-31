#include "queue.h"
#include <iostream>

Queue::Queue(){
    n = 100;
    front = 0;
    rear = -1;
    size = 0;
    queue = new Song[n];
}
Queue::Queue(int n, int front, int rear, int size){
    this->n = n;
    front = 0;
    rear = -1;
    size = 0;
    queue = new Song[this->n];
}
Queue::~Queue(){
    delete[] queue;
}
void Queue::enqueue(const Song& song){
    if (full()){
        std::cout << "Hang doi da day" << std::endl;
        return;
    }
    rear = (rear + 1) % n;
    queue[rear] = song;
    size++;
}
void Queue::dequeue(){
    if (empty()){
        std::cout << "Hang cho dang rong" << std::endl;
        return;
    }
    front = (front + 1) % n;
    size--;
}
Song* Queue::getFront() const{
    if (empty()) return nullptr;
    else return &queue[front];
}
bool Queue::empty() const{
    return size == 0;
}
bool Queue::full() const{
    return size == n;
}
int Queue::getSize() const{
    return size;
}