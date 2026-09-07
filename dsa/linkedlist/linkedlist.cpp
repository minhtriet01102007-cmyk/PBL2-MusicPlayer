#include "linkedlist.h"
#include <iostream>

DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
    Node* current = head;

    while (current != nullptr)
    {
        Node* temp = current;
        current = current->next;

        delete temp;
    }
}

void DoublyLinkedList::push_front(int value)
{
    Node* newNode = new Node;

    newNode->data = value;
    newNode->next = head;
    newNode->prev = nullptr;

    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        head->prev = newNode;
        head = newNode;
    }

    size++;
}

void DoublyLinkedList::push_back(int value)
{
    Node* newNode = new Node;

    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = tail;

    if (tail == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

void DoublyLinkedList::pop_front()
{
    if (empty())
    {
        return;
    }

    Node* temp = head;

    head = head->next;

    if (head == nullptr)
    {
        tail = nullptr;
    }
    else
    {
        head->prev = nullptr;
    }

    delete temp;
    size--;
}

void DoublyLinkedList::pop_back()
{
    if (empty())
    {
        return;
    }

    Node* temp = tail;

    tail = tail->prev;

    if (tail == nullptr)
    {
        head = nullptr;
    }
    else
    {
        tail->next = nullptr;
    }

    delete temp;
    size--;
}

void DoublyLinkedList::insert(int index, int value)
{
    if (index < 0 || index > size)
    {
        return;
    }

    if (index == 0)
    {
        push_front(value);
        return;
    }

    if (index == size)
    {
        push_back(value);
        return;
    }

    Node* current = head;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    Node* newNode = new Node;

    newNode->data = value;
    newNode->next = current;
    newNode->prev = current->prev;

    current->prev->next = newNode;
    current->prev = newNode;

    size++;
}

void DoublyLinkedList::remove(int index)
{
    if (index < 0 || index >= size)
    {
        return;
    }

    if (index == 0)
    {
        pop_front();
        return;
    }

    if (index == size - 1)
    {
        pop_back();
        return;
    }

    Node* current = head;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    delete current;
    size--;
}

bool DoublyLinkedList::empty() const
{
    return size == 0;
}

int DoublyLinkedList::getSize() const
{
    return size;
}

void DoublyLinkedList::displayForward() const
{
    Node* current = head;

    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->next;
    }

    std::cout << std::endl;
}

void DoublyLinkedList::displayBackward() const
{
    Node* current = tail;

    while (current != nullptr)
    {
        std::cout << current->data << " ";
        current = current->prev;
    }

    std::cout << std::endl;
}