#include "heap.h"
#include <iostream>

MaxHeap::MaxHeap()
{
    capacity = 10;
    size = 0;

    data = new int[capacity];
}

MaxHeap::MaxHeap(int capacity)
{
    if (capacity <= 0)
    {
        capacity = 10;
    }

    this->capacity = capacity;
    size = 0;

    data = new int[this->capacity];
}

MaxHeap::~MaxHeap()
{
    delete[] data;
}

void MaxHeap::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (data[parent] >= data[index])
        {
            break;
        }

        int temp = data[parent];

        data[parent] = data[index];
        data[index] = temp;

        index = parent;
    }
}

void MaxHeap::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int largest = index;

        if (left < size && data[left] > data[largest])
        {
            largest = left;
        }

        if (right < size && data[right] > data[largest])
        {
            largest = right;
        }

        if (largest == index)
        {
            break;
        }

        int temp = data[index];

        data[index] = data[largest];
        data[largest] = temp;

        index = largest;
    }
}

void MaxHeap::insert(int value)
{
    if (full())
    {
        std::cout << "Heap day!" << std::endl;
        return;
    }

    data[size] = value;
    size++;

    heapifyUp(size - 1);
}

void MaxHeap::removeMax()
{
    if (empty())
    {
        return;
    }

    data[0] = data[size - 1];

    size--;

    if (size > 0)
    {
        heapifyDown(0);
    }
}

int MaxHeap::getMax() const
{
    if (empty())
    {
        return -1;
    }

    return data[0];
}

bool MaxHeap::empty() const
{
    return size == 0;
}

bool MaxHeap::full() const
{
    return size == capacity;
}

int MaxHeap::getSize() const
{
    return size;
}

void MaxHeap::display() const
{
    for (int i = 0; i < size; i++)
    {
        std::cout << data[i] << " ";
    }

    std::cout << std::endl;
}