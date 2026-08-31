#include "playlist.h"
#include <iostream>

Playlist::Playlist(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}
Playlist::~Playlist(){
    Node* current = head;
    while (current != nullptr){
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
void Playlist::add(const Song& song){
    Node* newNode = new Node(song);
    if (head == nullptr){
        head = newNode;
        tail = newNode;
    }
    else{
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}
void Playlist::remove(int index){
    if (index < 0 || index >= size){
        std::cout << "Vi tri khong hop le" << std::endl;
        return;
    }
    Node* current = head;
    for (int i = 0; i < index; i++){
        current = current->next;
    }
    if (head == tail){
        head = nullptr;
        tail = nullptr;
    }
    else if (current == head){
        head = head->next;
        head->prev = nullptr;
    }
    else if (current == tail){
        tail = tail->prev;
        tail->next = nullptr;
    }
    else{
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    delete current;
    size--;
}
void Playlist::display() const{
    if (empty()){
        std::cout << "Playlist dang rong" << std::endl;
        return;
    }
    Node* current = head;
    while (current != nullptr){
        current->song.display();
        current = current->next;
    }
}
bool Playlist::empty() const{
    return size == 0;
}
int Playlist::getSize() const{
    return size;
}