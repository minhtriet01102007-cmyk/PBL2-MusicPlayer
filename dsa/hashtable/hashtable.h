#pragma once
#include <iostream>
#include "hashtablenode.h"

class HashTable{
    private:
        HashNode* table;
        int capacity;
        int size;
        int hashFunction(int key) const; // Biến key thành index của mảng
    public:
        HashTable();
        HashTable(int capacity);
        ~HashTable();
        bool insert(int key, int value);
        bool search(int key, int& outValue) const;
        bool remove(int key);
        bool isEmpty() const;
        int getSize() const;
        void display() const;
        void clear();
};
inline int HashTable::hashFunction(int key) const{
    int hash = key % this->capacity;
    return (hash < 0) ? (hash + this->capacity) : hash;
}
inline HashTable::HashTable(){
    this->capacity = 101;
    this->size = 0;
    this->table = new HashNode[this->capacity];
    for (int i = 0; i < this->capacity; i++){
        this->table[i].state = EMPTY; // Đánh dấu tất cả ô là trống
    }
}
inline HashTable::HashTable(int capacity){
    this->capacity = (capacity > 0) ? capacity : 101;
    this->size = 0;
    this->table = new HashNode[this->capacity];
    for (int i = 0; i < this->capacity; i++){
        this->table[i].state = EMPTY; // Đánh dấu tất cả ô là trống
    }
}
inline HashTable::~HashTable(){
    delete[] this->table;
    this->table = nullptr; // tránh tình trạng con trỏ lạc
}
inline bool HashTable::isEmpty() const{
    return this->size == 0;
}
inline int HashTable::getSize() const{
    return this->size;
}
inline bool HashTable::insert(int key, int value){
    if (this->size >= this->capacity){
        std::cout << "Bang bam da day" << std::endl;
        return false;
    }
    int index = this->hashFunction(key);
    int firstDeletedIndex = -1; // Biến này dùng để ghi nhớ ô DELETED đầu tiên gặp được, Chưa tìm thấy ô DELETED nào
    for (int i = 0; i < this->capacity; i++){
        int currentIndex = (index + i) % this->capacity; // Linear Probing: Dò tuyến tính
        if (this->table[currentIndex].state == OCCUPIED){
            if (this->table[currentIndex].key == key){
                this->table[currentIndex].value = value; // Cập nhật giá trị nếu trùng key
                return true;
            }
        } else if (this->table[currentIndex].state == DELETED){
            if (firstDeletedIndex == -1){
                firstDeletedIndex = currentIndex; // Lưu lại vị trí DELETED đầu tiên
            }
        } else{ // EMPTY
            int insertIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : currentIndex;
            this->table[insertIndex].key = key;
            this->table[insertIndex].value = value;
            this->table[insertIndex].state = OCCUPIED;
            this->size++;
            return true;
        }
    }
    if (firstDeletedIndex != -1){
        this->table[firstDeletedIndex].key = key;
        this->table[firstDeletedIndex].value = value;
        this->table[firstDeletedIndex].state = OCCUPIED;
        this->size++;
        return true;
    }
    return false; // Không có EMPTY và DELETED => OCCUPIED => Không thể thêm
}
inline bool HashTable::search(int key, int& outValue) const{ // Tìm key và lấy value ra
    int index = this->hashFunction(key);
    for (int i = 0; i < this->capacity; i++){
        int currentIndex = (index + i) % this->capacity;
        if (this->table[currentIndex].state == EMPTY){
            return false;
        }
        if (this->table[currentIndex].state == OCCUPIED && this->table[currentIndex].key == key){
            outValue = this->table[currentIndex].value;
            return true;
        }
    }
    return false; // Không có EMPTY và DELETED => OCCUPIED => Không thể thêm
}
inline bool HashTable::remove(int key){
    int index = this->hashFunction(key);
    for (int i = 0; i < this->capacity; i++){
        int currentIndex = (index + i) % this->capacity;
        if (this->table[currentIndex].state == EMPTY){
            return false;
        }
        if (this->table[currentIndex].state == OCCUPIED && this->table[currentIndex].key == key){
            this->table[currentIndex].state = DELETED;
            this->size--;
            return true;
        }
    }
    return false; // Không có EMPTY và DELETED => OCCUPIED => Không thể thêm
}
inline void HashTable::clear(){
    for (int i = 0; i < this->capacity; i++){
        this->table[i].state = EMPTY;
    }
    this->size = 0;
}
inline void HashTable::display() const{
    std::cout << "HashTable:" << std::endl;
    for (int i = 0; i < this->capacity; i++){
        if (this->table[i].state == OCCUPIED){
            std::cout << "  [" << i << "] Key: " << this->table[i].key
                      << " => Value: " << this->table[i].value << std::endl;
        }
    }
}
