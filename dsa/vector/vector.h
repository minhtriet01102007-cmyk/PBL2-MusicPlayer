#pragma once

struct Vector{
    private:
        int* data;
        int size;
        int capacity;
    public:
        Vector();
        Vector(int capacity);
        ~Vector();
        void push_back(int value);
        void pop_back();
        void insert(int index, int value);
        void erase(int index);
        int get(int index) const;
        int getSize() const;
        int getCapacity() const;
        bool empty() const;
        void clear();   
        void display() const;
};