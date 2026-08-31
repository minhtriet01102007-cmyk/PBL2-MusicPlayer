#include "../include/listening_history.h"
#include <iostream>

Listening_History::Listening_History(){
    n = 50;
    top = -1;
    history = new Song[n];
}
Listening_History::Listening_History(int n){
    this->n = n;
    top = -1;
    history = new Song[this->n];
}
Listening_History::~Listening_History(){
    delete[] history;
}
void Listening_History::push(const Song& song){
    if (top >= n - 1){
        std::cout << "Danh sach da day" << std::endl;
        return;
    }
    top++;
    history[top] = song;
}
void Listening_History::pop(){
    if (!empty()){
        top--;
    }
    else{
        std::cout << "Lich su nghe dang rong" << std::endl;
    }
}
Song* Listening_History::getTop() const{
    if (empty()){
        return nullptr;
    }
    return &history[top];
}

bool Listening_History::empty() const{
    return top == -1;
}
int Listening_History::getSize() const{
    return top + 1;
}

