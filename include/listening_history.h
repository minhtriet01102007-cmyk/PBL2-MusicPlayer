#pragma once
#include "song.h"

class Listening_History{
    private:
        Song* history;
        int n; // n là số phần tử
        int top;
    public:
        Listening_History();
        Listening_History(int n);
        ~Listening_History();
        void push(const Song& song);
        void pop();
        Song* getTop() const;
        bool empty() const;
        int getSize() const;
};