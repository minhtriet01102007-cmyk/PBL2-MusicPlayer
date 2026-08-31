#pragma once
#include <stdexcept>
#include <utility>

template <typename T>
class Vector{
    private:
        T* data;
        int size;
        int capacity;
        void resize();
    public:
        Vector();
        explicit Vector(int initialSize);
        Vector(const Vector<T>& other);
        ~Vector();
        Vector<T>& operator=(const Vector<T>& other);
        T& operator[](int index);
        const T& operator[](int index) const;
        T& at(int index);
        const T& at(int index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        void push_back(const T& element);
        void push_front(const T& element);
        void insert(int index, const T& element);
        void pop_back();
        void pop_front();
        void erase(int index);
        int getSize() const;
        int getCapacity() const;
        bool isEmpty() const;
        int find(const T& element) const;
        bool contains(const T& element) const;  
        void clear();
        void reverse();
};

template <typename T>
void Vector<T>::resize(){
    int newCapacity;
    if (capacity == 0){
        newCapacity = 1;
    }
    else{
        newCapacity = capacity * 2;
    }
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++){
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}
template <typename T>
Vector<T>::Vector() : data(nullptr), size(0), capacity(0)
{}
template <typename T>
Vector<T>::Vector(int initialSize) : size(initialSize), capacity(initialSize){
    if (initialSize < 0){
        throw std::invalid_argument("Initial size cannot be negative");
    }
    data = new T[capacity];
    for (int i = 0; i < size; i++){
        data[i] = T();
    }
}
template <typename T>
Vector<T>::Vector(const Vector<T>& other) : size(other.size), capacity(other.capacity){
    data = new T[capacity];
    for (int i = 0; i < size; i++){
        data[i] = other.data[i];
    }
}
template <typename T>
Vector<T>::~Vector(){
    delete[] data;
}
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other){
    if (this != &other){
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; i++){
            data[i] = other.data[i];
        }
    }
    return *this;
}
template <typename T>
T& Vector<T>::operator[](int index){
    return data[index];
}
template <typename T>
const T& Vector<T>::operator[](int index) const{
    return data[index];
}
template <typename T>
T& Vector<T>::at(int index){
    if (index < 0 || index >= size){
        throw std::out_of_range("Vector index out of range");
    }
    return data[index];
}
template <typename T>
const T& Vector<T>::at(int index) const{
    if (index < 0 || index >= size){
        throw std::out_of_range("Vector index out of range");
    }
    return data[index];
}
template <typename T>
T& Vector<T>::front(){
    if (isEmpty()){
        throw std::out_of_range("Vector is empty");
    }
    return data[0];
}
template <typename T>
const T& Vector<T>::front() const{
    if (isEmpty()){
        throw std::out_of_range("Vector is empty");
    }
    return data[0];
}
template <typename T>
T& Vector<T>::back(){
    if (isEmpty()){
        throw std::out_of_range("Vector is empty");
    }
    return data[size - 1];
}
template <typename T>
const T& Vector<T>::back() const{
    if (isEmpty()){
        throw std::out_of_range("Vector is empty");
    }
    return data[size - 1];
}
template <typename T>
void Vector<T>::push_back(const T& element){
    if (size == capacity){
        resize();
    }
    data[size] = element;
    size++;
}
template <typename T>
void Vector<T>::push_front(const T& element){
    insert(0, element);
}
template <typename T>
void Vector<T>::insert(int index, const T& element){
    if (index < 0 || index > size){
        throw std::out_of_range("Insert index out of range");
    }
    if (size == capacity){
        resize();
    }
    for (int i = size; i > index; i--){
        data[i] = data[i - 1];
    }
    data[index] = element;
    size++;
}
template <typename T>
void Vector<T>::pop_back(){
    if (isEmpty()){
        throw std::out_of_range("Vector is empty");
    }
    size--;
}
template <typename T>
void Vector<T>::pop_front(){
    erase(0);
}
template <typename T>
void Vector<T>::erase(int index){
    if (index < 0 || index >= size){
        throw std::out_of_range("Erase index out of range");
    }
    for (int i = index; i < size - 1; i++){
        data[i] = data[i + 1];
    }
    size--;
}
template <typename T>
int Vector<T>::getSize() const{
    return size;
}
template <typename T>
int Vector<T>::getCapacity() const{
    return capacity;
}
template <typename T>
bool Vector<T>::isEmpty() const{
    return size == 0;
}
template <typename T>
int Vector<T>::find(const T& element) const{
    for (int i = 0; i < size; i++){
        if (data[i] == element){
            return i;
        }
    }
    return -1;
}
template <typename T>
bool Vector<T>::contains(const T& element) const{
    return find(element) != -1;
}
template <typename T>
void Vector<T>::clear(){
    size = 0;
}
template <typename T>
void Vector<T>::reverse(){
    for (int i = 0; i < size / 2; i++){
        std::swap(data[i], data[size - 1 - i]);
    }
}