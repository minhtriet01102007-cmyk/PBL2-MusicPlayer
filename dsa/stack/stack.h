#pragma once

struct Stack{
    private:
        int* data;
        int top;
        int capacity;
    public:
        Stack();
        Stack(int capacity);
        ~Stack();
        void push(int value);
        void pop();
        int peek() const;
        bool IsEmpty() const;
        bool IsFull() const;
        int getSize() const;    
        void display() const;
};