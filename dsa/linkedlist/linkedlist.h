#pragma once
#include "node.h"
#include <iostream>

class DoublyLinkedList{
    private:
        Node* head;
        Node* tail;
        int size;
    public:
        DoublyLinkedList();
        ~DoublyLinkedList();
        void push_front(int value);
        void push_back(int value);
        void insert(int index, int value);
        void pop_front();
        void pop_back();
        void remove(int index);
        void clear();
        bool IsEmpty() const;
        int getSize() const;
        void displayForward() const;
        void displayBackward() const;
};
inline DoublyLinkedList::DoublyLinkedList(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}
inline DoublyLinkedList::~DoublyLinkedList(){
    this->clear();
}
inline void DoublyLinkedList::push_front(int value){
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = head;
    newNode->prev = nullptr;
    if (IsEmpty()){
        head = tail = newNode;
    }
    else{
        head->prev = newNode;
        head = newNode;
    }
    size++;
}
inline void DoublyLinkedList::push_back(int value){
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = tail;
    if (IsEmpty()){
        head = tail = newNode;
    }
    else{
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}
inline void DoublyLinkedList::insert(int index, int value){
    if (index < 0 || index > size) return;
    else if (index == 0){
        push_front(value);
        return;
    }
    else if (index == size){
        push_back(value);
        return;
    }
    Node* current = head;
    for (int i = 0; i < index; i++){
        current = current->next;
    }
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    size++;
}
inline void DoublyLinkedList::pop_front(){
    if (IsEmpty()) return;
    Node* temp = head;
    if (head == tail) head = tail = nullptr;
    else{
        head = head->next;
        head->prev = nullptr;
    }
    delete temp;
    size--;
}
inline void DoublyLinkedList::pop_back(){
    if (IsEmpty()) return;
    Node* temp = tail;
    if (head == tail) head = tail = nullptr;
    else{
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete temp;
    size--;
}
inline void DoublyLinkedList::remove(int index){
    if (index < 0 || index >= size || IsEmpty()) return;
    else if (index == 0){
        pop_front();
        return;
    }
    else if (index == size-1){
        pop_back();
        return;
    }
    Node* curr = nullptr;
    if (index < size/2){
        curr = head;
        for (int i=0; i<index; i++) curr = curr->next;
    } 
    else{
        curr = tail;
        for (int i= size-1; i>index; i--) curr = curr->prev;   
    }
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
    size--;
}
inline void DoublyLinkedList::clear(){
    Node* curr = head;
    while (curr != nullptr){
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}
inline bool DoublyLinkedList::IsEmpty() const{
    return this->size == 0; 
}
inline int DoublyLinkedList::getSize() const{
    return this->size;
}
inline void DoublyLinkedList::displayForward() const{
    Node* current = head;
    while (current != nullptr){
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}
inline void DoublyLinkedList::displayBackward() const{
    Node* current = tail;
    while (current != nullptr){
        std::cout << current->data << " ";
        current = current->prev;
    }
    std::cout << std::endl;
}
