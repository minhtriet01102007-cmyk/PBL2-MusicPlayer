#pragma once

struct Queue{
    private:
        int* data;
        int capacity;
        int front;
        int rear;
        int size;
    public:
        Queue();
        Queue(int capacity);
        ~Queue();
        void enqueue(int value);
        void dequeue();
        int getFront() const;
        bool IsEmpty() const;
        bool IsFull() const;
        int getSize() const;
        void display() const;
};