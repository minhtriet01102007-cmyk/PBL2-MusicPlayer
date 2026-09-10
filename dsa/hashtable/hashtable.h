#pragma once
#include "hashtablenode.h"
#include <iostream>

class HashTable{
    private:
        HashNode* table;
        int capacity;
        int size;
        int hashFunction(int key) const;
    public:
        HashTable();
        HashTable(int capacity); 
        ~HashTable();
        void insert(int key, int value);
        bool search(int key, int& outValue) const;
        bool remove(int key);
        bool IsEmpty() const;
        bool IsFull() const;
        int getSize() const;
        void display() const;
};
inline HashTable::HashTable(){
    capacity = 101;
    size = 0;
    table = new HashNode[capacity];
    for (int i=0; i<capacity; i++){
        table[i].state = EMPTY;
    }
}
inline HashTable::HashTable(int capacity){
    this->capacity = (capacity > 0) ? capacity : 101;
    this->size = 0;
    this->table = new HashNode[this->capacity];
    for (int i=0; i<this->capacity; i++){
        this->table[i].state = EMPTY;
    }
}
inline HashTable::~HashTable(){
    delete[] table;
    table = nullptr;
}
inline int HashTable::hashFunction(int key) const{
    return (key % capacity + capacity) % capacity;
}
inline bool HashTable::IsEmpty() const{
    return this->size == 0;
}
inline bool HashTable::IsFull() const{
    return this->size == this->capacity;
}
inline int HashTable::getSize() const{
    return this->size;
}
inline void HashTable::insert(int key, int value){
    if (IsFull()){
        std::cout << "Bang bam da day" << std::endl;
        return;
    }
    int index = hashFunction(key);
    for (int i=0; i<capacity; i++){
        int currIndex = (index+i) % capacity;
        // Nếu key đã có sẵn thì cập nhật giá trị
        if (table[currIndex].state == OCCUPIED && table[currIndex].key == key){
            table[currIndex].value = value;
            return;
        }
        // Tìm thấy ô trống hoặc ô đã bị xóa để chèn vào
        if (table[currIndex].state == EMPTY || table[currIndex].state == DELETED){
            table[currIndex].key = key;
            table[currIndex].value = value;
            table[currIndex].state = OCCUPIED;
            size++;
            return;
        }
    }
}
inline bool HashTable::search(int key, int& outValue) const{
    int index = hashFunction(key);
    for (int i=0; i<capacity; i++){
        int currIndex = (index + i) % capacity;
        if (table[currIndex].state == EMPTY){
            return false; 
        }
        if (table[currIndex].state == OCCUPIED && table[currIndex].key == key){
            outValue = table[currIndex].value;
            return true;
        }
    }
    return false;
}
inline bool HashTable::remove(int key){
    int index = hashFunction(key);
    for (int i=0; i<capacity; i++){
        int currIndex = (index + i) % capacity;
        if (table[currIndex].state == EMPTY){
            return false;
        }
        if (table[currIndex].state == OCCUPIED && table[currIndex].key == key){
            table[currIndex].state = DELETED; // Đánh dấu đã xóa
            size--;
            return true;
        }
    }
    return false;
}
inline void HashTable::display() const{
    for (int i=0; i<capacity; i++){
        if (table[i].state == OCCUPIED){
            std::cout << "[" << i << "]: Key = " << table[i].key << ", Val = " 
                << table[i].value << "\n";
        }
    }
}
