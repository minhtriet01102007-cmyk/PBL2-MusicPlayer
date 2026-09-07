#pragma once

struct Node
{
    int data;
    Node* next;
    Node* prev;
};

struct DoublyLinkedList
{
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void push_front(int value);
    void push_back(int value);

    void pop_front();
    void pop_back();

    void insert(int index, int value);
    void remove(int index);

    bool empty() const;
    int getSize() const;

    void displayForward() const;
    void displayBackward() const;
};