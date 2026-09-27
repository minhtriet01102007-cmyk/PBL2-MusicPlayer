#pragma once
#include <iostream>
#include "node.h"

class DoublyLinkedList{
    private:
        Node* head;
        Node* tail;
        Node* current;
        int size;
    public:
        DoublyLinkedList();
        ~DoublyLinkedList();
        void pushBack(int value);
        void pushFront(int value);
        bool remove(int value);
        bool nextTrack();
        bool prevTrack();
        int getCurrent() const;
        void resetCurrent();
        bool isEmpty() const;
        int getSize() const;
        void clear();
        void displayForward() const;
        void displayBackward() const;
};
DoublyLinkedList::DoublyLinkedList(){
    this->head = nullptr;
    this->tail = nullptr;
    this->current = nullptr;
    this->size = 0;
}
DoublyLinkedList::~DoublyLinkedList(){
    this->clear();
}
bool DoublyLinkedList::isEmpty() const{
    return this->head == nullptr;
}
int DoublyLinkedList::getSize() const{
    return this->size;
}
void DoublyLinkedList::pushBack(int value){ // Thêm cuối
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = this->tail;
    if (this->isEmpty()){
        this->head = newNode;
        this->tail = newNode;
        this->current = newNode;
    } else{
        this->tail->next = newNode;
        this->tail = newNode;
    }
    this->size++;
}
void DoublyLinkedList::pushFront(int value){ // Thêm đầu
    Node* newNode = new Node;
    newNode->data = value;
    newNode->prev = nullptr;
    newNode->next = this->head;
    if (this->isEmpty()){
        this->head = newNode;
        this->tail = newNode;
        this->current = newNode;
    } else{
        this->head->prev = newNode;
        this->head = newNode;
    }
    this->size++;
}
bool DoublyLinkedList::remove(int value){ // Xóa node có data = value
    Node* curr = this->head;
    while (curr != nullptr && curr->data != value){
        curr = curr->next;
    }
    if (curr == nullptr) return false;
    if (curr == this->current){
        this->current = curr->next ? curr->next : curr->prev;
    }
    if (curr == this->head){
        this->head = curr->next;
        if (this->head) this->head->prev = nullptr;
        else this->tail = nullptr; 
    } else if (curr == this->tail){
        this->tail = curr->prev;
        if (this->tail) this->tail->next = nullptr;
        else this->head = nullptr;
    } else{
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }
    delete curr;
    this->size--;
    return true;
}
bool DoublyLinkedList::nextTrack(){
    if (this->current != nullptr && this->current->next != nullptr){
        this->current = this->current->next;
        return true;
    }
    return false;
}
bool DoublyLinkedList::prevTrack(){
    if (this->current != nullptr && this->current->prev != nullptr){
        this->current = this->current->prev;
        return true;
    }
    return false;
}
int DoublyLinkedList::getCurrent() const{
    if (this->current == nullptr) return -1;
    return this->current->data;
}
void DoublyLinkedList::resetCurrent(){
    this->current = this->head;
}
void DoublyLinkedList::clear(){
    Node* curr = this->head;
    while (curr != nullptr){
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    this->head = nullptr;
    this->tail = nullptr;
    this->current = nullptr;
    this->size = 0;
}
void DoublyLinkedList::displayForward() const{
    Node* temp = this->head;
    std::cout << "List (Forward): ";
    while (temp != nullptr) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << "\n";
}
void DoublyLinkedList::displayBackward() const{
    Node* temp = this->tail;
    std::cout << "List (Backward): ";
    while (temp != nullptr) {
        std::cout << temp->data << " ";
        temp = temp->prev;
    }
    std::cout << "\n";
}
