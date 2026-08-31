#pragma once
#include "song.h"

class Playlist{
    private:
        struct Node{
            Song song;
            Node* next;
            Node* prev;
            Node(const Song& song): song(song), next(nullptr), prev(nullptr){}
        };
        Node* head;
        Node* tail;
        int size;
    public:
        Playlist();
        ~Playlist();
        void add(const Song& song);
        void remove(int index);
        void display() const;
        bool empty() const;
        int getSize() const;
};