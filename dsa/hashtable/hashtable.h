#pragma once

struct HashNode
{
    int key;
    int value;
    bool occupied;
};

struct HashTable
{
private:
    HashNode* table;
    int capacity;
    int size;

    int hashFunction(int key) const;

public:
    HashTable();
    HashTable(int capacity);
    ~HashTable();

    void insert(int key, int value);
    bool search(int key, int& value) const;
    void remove(int key);

    bool empty() const;
    int getSize() const;

    void display() const;
};