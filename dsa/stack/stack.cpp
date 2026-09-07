#include "stack.h"
#include <iostream>

Stack::Stack(){
    capacity = 10;
    top = -1;
    data = new int[capacity];
}
Stack::Stack(int capacity){
    if (capacity <= 0){
        capacity = 10;
    }
    this->capacity = capacity;
    top = -1;
    data = new int[this->capacity];
}
Stack::~Stack(){
    delete[] data;
}
void Stack::push(int value){
    if (IsFull()){
        std::cout << "Stack da day" << std::endl;
        return;
    }
    top++;
    data[top] = value;
}
void Stack::pop(){
    if (IsEmpty()){
        std::cout << "Stack rong" << std::endl;
        return;
    }
    top--;
}
int Stack::peek() const{ //xem phần tử trên cùng nhưng không xóa
    if (IsEmpty()) return -1;
    return data[top];
}
bool Stack::IsEmpty() const{
    return top == -1;
}
bool Stack::IsFull() const{
    return top == capacity - 1;
}
int Stack::getSize() const{
    return top + 1;
}
void Stack::display() const{
    for (int i=top; i>=0; i--){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}