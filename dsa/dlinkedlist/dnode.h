#pragma once

template <typename T>
struct Node{
    T data;
    Node* prev;
    Node* next;
    Node(const T& val);
};

template <typename T>
Node<T>::Node(const T& val): data(val), prev(nullptr), next(nullptr){}