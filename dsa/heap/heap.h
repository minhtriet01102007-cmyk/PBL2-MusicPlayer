#pragma once

struct MaxHeap
{
private:
    int* data;
    int size;
    int capacity;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MaxHeap();
    MaxHeap(int capacity);
    ~MaxHeap();

    void insert(int value);
    void removeMax();

    int getMax() const;

    bool empty() const;
    bool full() const;

    int getSize() const;

    void display() const;
};