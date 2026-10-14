#pragma once
#include <string>

template <typename K, typename V>
class HashTable{
    private:
        static const int TABLE_SIZE = 101; 
        HashNode<K, V>* table[TABLE_SIZE]; 
        int count;
        int hashFunction(const std::string& key) const;
    public:
        HashTable();
        ~HashTable();
        void insert(const K& key, const V& value);
        bool search(const K& key, V& outValue) const;
        bool remove(const K& key);
        int getCount() const;
        bool isEmpty() const;
        void clear();
};

template <typename K, typename V>
HashTable<K, V>::HashTable(){
    this->count = 0;
    for (int i = 0; i < TABLE_SIZE; i++){
        this->table[i] = nullptr;
    }
}

template <typename K, typename V>
HashTable<K, V>::~HashTable(){
    this->clear();
}

template <typename K, typename V>
int HashTable<K, V>::hashFunction(const std::string& key) const{
    int sum = 0;
    for (char c : key) sum = (sum * 31 + c) % TABLE_SIZE; // Nhân 31 giúp phân tán chuỗi đều hơn
    return (sum < 0) ? (sum + TABLE_SIZE) : sum;
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value){
    int index = this->hashFunction(key);
    HashNode<K, V>* curr = this->table[index];
    while (curr != nullptr){
        if (curr->key == key){
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
    newNode->next = this->table[index];
    this->table[index] = newNode;
    this->count++;
}

template <typename K, typename V>
bool HashTable<K, V>::search(const K& key, V& outValue) const{
    int index = this->hashFunction(key);
    HashNode<K, V>* curr = this->table[index];
    while (curr != nullptr){
        if (curr->key == key){
            outValue = curr->value;
            return true;
        }
        curr = curr->next;
    }
    return false; // Không tìm thấy
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key){
    int index = this->hashFunction(key);
    HashNode<K, V>* curr = this->table[index];
    HashNode<K, V>* prev = nullptr;
    while (curr != nullptr){
        if (curr->key == key){
            if (prev == nullptr){
                this->table[index] = curr->next;
            } else{
                prev->next = curr->next;
            }
            delete curr;
            this->count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

template <typename K, typename V>
int HashTable<K, V>::getCount() const{
    return this->count;
}

template <typename K, typename V>
bool HashTable<K, V>::isEmpty() const{
    return this->count == 0;
}

template <typename K, typename V>
void HashTable<K, V>::clear(){
    for (int i = 0; i < TABLE_SIZE; i++){
        HashNode<K, V>* curr = this->table[i];
        while (curr != nullptr){
            HashNode<K, V>* temp = curr;
            curr = curr->next;
            delete temp;
        }
        this->table[i] = nullptr;
    }
    this->count = 0;
}
