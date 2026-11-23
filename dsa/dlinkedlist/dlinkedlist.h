#pragma once
#include <iostream>
#include <stdexcept>
#include "dnode.h"

template <typename T>
class DoublyLinkedList{
    private:
        Node<T>* head;
        Node<T>* tail;
        Node<T>* current; 
        int size;
        void copyFrom(const DoublyLinkedList& other); // sao chép từng node của other sang danh sách này
    public:
        DoublyLinkedList();
        DoublyLinkedList(const DoublyLinkedList& other); // sao chép sâu (cần khi Playlist bị sao chép)
        DoublyLinkedList& operator=(const DoublyLinkedList& other);
        ~DoublyLinkedList();
        void pushBack(const T& val); // Thêm phần tử vào cuối danh sách
        bool remove(const T& val); // Xóa phần tử có giá trị bằng val 
        bool contains(const T& val) const; // Kiểm tra val có trong danh sách không (không đổi vị trí đang phát)
        T getCurrent() const; // Trả về phần tử đang được chọn hiện tại
        bool hasNext() const; // Kiểm tra xem có node phía sau current
        bool hasPrev() const; // Kiểm tra xem có node phía trước current
        T next(); // Di chuyển current sang node tiếp theo
        T prev(); // Di chuyển current lùi lại node trước
        void resetPlayback(); // Đặt current về đầu danh sách(dùng khi phát nhạc từ đầu lại)
        int getSize() const;
        bool isEmpty() const;
        void clear(); // Xóa toàn bộ danh sách
};
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr), current(nullptr), size(0){}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), current(nullptr), size(0){
        this->copyFrom(other);
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other){
        if (this != &other){
            this->clear();
            this->copyFrom(other);
        }
        return *this;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList(){
        this->clear();
}

template <typename T>
void DoublyLinkedList<T>::copyFrom(const DoublyLinkedList& other){
        for (Node<T>* p = other.head; p != nullptr; p = p->next){
            this->pushBack(p->data);
            if (p == other.current) this->current = this->tail; // giữ nguyên vị trí đang phát
        }
        if (other.current == nullptr) this->current = nullptr;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(const T& val){
        Node<T>* newNode = new Node<T>(val);
        if (this->head == nullptr) this->head = this->tail = this->current = newNode; // danh sách rỗng
        else{
            this->tail->next = newNode;
            newNode->prev = this->tail;
            this->tail = newNode;
        }
        this->size++;
}

template <typename T>
bool DoublyLinkedList<T>::remove(const T& val){
        Node<T>* curr = this->head;
        while (curr != nullptr){ //Duyệt từ đầu đến cuối
            if (curr->data == val){ // Nếu gặp node cần xóa
                if (curr == this->head) this->head = this->head->next; //Xóa đầu
                if (curr == this->tail) this->tail = this->tail->prev; // Xóa cuối
                if (curr->prev != nullptr) curr->prev->next = curr->next; // Nối next
                if (curr->next != nullptr) curr->next->prev = curr->prev; // Nối prev
                if (this->current == curr){
                    this->current = (curr->next != nullptr) ? curr->next : curr->prev; // cập nhật current (hết node sau thì lùi về node trước)
                }
                delete curr;
                this->size--;
                return true;
            }
            curr = curr->next;
        }
        return false;
}

template <typename T>
T DoublyLinkedList<T>::getCurrent() const{
        if (this->current == nullptr) throw std::runtime_error("Rong");
        else return this->current->data;
}

template <typename T>
bool DoublyLinkedList<T>::hasNext() const{
        return this->current != nullptr && this->current->next != nullptr;
}

template <typename T>
bool DoublyLinkedList<T>::hasPrev() const{
        return this->current != nullptr && this->current->prev != nullptr;
}

template <typename T>
T DoublyLinkedList<T>::next(){
        if (this->hasNext()){
            this->current = this->current->next;
            return this->current->data;
        }
        return (this->current != nullptr) ? this->current->data : T(); 
        // T(): giá trị mặc định của kiểu dữ liệu T
}

template <typename T>
T DoublyLinkedList<T>::prev(){
    if (this->hasPrev()){
        this->current = this->current->prev;
        return this->current->data;
    }
    return (this->current != nullptr) ? this->current->data : T();
}

template <typename T>
void DoublyLinkedList<T>::resetPlayback(){
        this->current = this->head;
}

template <typename T>
int DoublyLinkedList<T>::getSize() const{
        return this->size;
}

template <typename T>
bool DoublyLinkedList<T>::isEmpty() const{
        return this->size == 0;
}

template <typename T>
void DoublyLinkedList<T>::clear(){
        Node<T>* curr = this->head;
        while (curr != nullptr){
            Node<T>* temp = curr;
            curr = curr->next;
            delete temp;
        }
        this->head = nullptr;
        this->tail = nullptr;
        this->current = nullptr;
        this->size = 0;
}

template <typename T>
bool DoublyLinkedList<T>::contains(const T& val) const{
        for (Node<T>* p = this->head; p != nullptr; p = p->next){
            if (p->data == val) return true;
        }
        return false;
}