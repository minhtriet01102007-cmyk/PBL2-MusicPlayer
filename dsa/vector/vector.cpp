#include "vector.h"
#include <iostream>

Vector::Vector(){
    capacity = 10;
    size = 0;
    data = new int[capacity];
}
Vector::Vector(int capacity){
    if (capacity <= 0){
        capacity = 10;
    }
    this->capacity = capacity;
    size = 0;
    data = new int[this->capacity];
}
Vector::~Vector(){
    delete[] data;
}
void Vector::push_back(int value){
    if (size == capacity){
        capacity *= 2;
        int* newData = new int[capacity];
        for (int i=0; i<size; i++){
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    data[size] = value;
    size++;
}
void Vector::pop_back(){
    if (size == 0){
        return;
    }
    size--;
}
\
void Vector::insert(int index, int value){
    if (index < 0 || index > size){
        return;
    }
    if (size == capacity){
        capacity *= 2;
        int* newData = new int[capacity];
        for (int i=0; i<size; i++){
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    for (int i=size; i>index; i--){
        data[i] = data[i-1];
    }
    data[index] = value;
    size++;
}
void Vector::erase(int index){
    if (index < 0 || index >= size){
        return;
    }

    for (int i = index; i < size - 1; i++)
    {
        data[i] = data[i + 1];
    }

    size--;
}

int Vector::get(int index) const
{
    if (index < 0 || index >= size)
    {
        return -1;
    }

    return data[index];
}

int Vector::getSize() const
{
    return size;
}

int Vector::getCapacity() const
{
    return capacity;
}

bool Vector::empty() const
{
    return size == 0;
}

void Vector::clear()
{
    size = 0;
}

void Vector::display() const
{
    for (int i = 0; i < size; i++)
    {
        std::cout << data[i] << " ";
    }

    std::cout << std::endl;
}