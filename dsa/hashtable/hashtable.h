#pragma once
#include <string>

template <typename K, typename V>
class HashTable{
    private:
        static const int TABLE_SIZE = 101; // Số lượng vị trí trong bảng băm
        HashNode<K, V>* table[TABLE_SIZE]; // mảng các con trỏ đến HashNode
        int count;
        int hashFunction(const std::string& key) const; // Chuyển chuỗi key thành một chỉ số từ 0 đến TABLE_SIZE - 1
    public:
        HashTable();
        ~HashTable();
        void insert(const K& key, const V& value); // Thêm một cặp (key, value) vào bảng băm
        bool search(const K& key, V& outValue) const; // Tìm kiếm value theo key
        bool remove(const K& key); // Xóa phần tử có key tương ứng
        int getCount() const;
        bool isEmpty() const;
        void clear();
};

template <typename K, typename V>
HashTable<K, V>::HashTable(){
    this->count = 0;
    for (int i = 0; i < TABLE_SIZE; i++){
        this->table[i] = nullptr; // bảng đang rỗng
    }
}

template <typename K, typename V>
HashTable<K, V>::~HashTable(){
    this->clear();
}

template <typename K, typename V>
int HashTable<K, V>::hashFunction(const std::string& key) const{
    int sum = 0;
    // Duyệt lần lượt từng ký tự c trong chuỗi key
    for (char c : key) sum = (sum * 31 + c) % TABLE_SIZE; 
    return (sum < 0) ? (sum + TABLE_SIZE) : sum;
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value){
    int index = this->hashFunction(key);
    HashNode<K, V>* curr = this->table[index]; // curr trỏ đến node đầu tiên trong bucket
    while (curr != nullptr){ 
        if (curr->key == key){
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    HashNode<K, V>* newNode = new HashNode<K, V>(key, value); // Tạo nút mới nếu khóa chưa tồn tại
    newNode->next = this->table[index]; // cho nút mới trỏ đến nút đầu tiên hiện tại
    this->table[index] = newNode; // cập nhật bucket để nút mới trở thành nút đầu tiên
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
    HashNode<K, V>* curr = this->table[index]; // curr trỏ đến node đầu
    HashNode<K, V>* prev = nullptr; // chưa có node đứng trước curr
    while (curr != nullptr){
        if (curr->key == key){
            if (prev == nullptr){ // không có Node đứng trước curr
                this->table[index] = curr->next; // xóa node đầu 
            } else{ // đã có Node đứng trước curr
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
