#include "hashtable.h"
#include <iostream>

HashTable::HashTable()
{
    capacity = 10;
    size = 0;

    table = new HashNode[capacity];

    for (int i = 0; i < capacity; i++)
    {
        table[i].occupied = false;
    }
}

HashTable::HashTable(int capacity)
{
    if (capacity <= 0)
    {
        capacity = 10;
    }

    this->capacity = capacity;
    size = 0;

    table = new HashNode[this->capacity];

    for (int i = 0; i < this->capacity; i++)
    {
        table[i].occupied = false;
    }
}

HashTable::~HashTable()
{
    delete[] table;
}

int HashTable::hashFunction(int key) const
{
    return key % capacity;
}

void HashTable::insert(int key, int value)
{
    if (size == capacity)
    {
        return;
    }

    int index = hashFunction(key);

    while (table[index].occupied)
    {
        if (table[index].key == key)
        {
            table[index].value = value;
            return;
        }

        index = (index + 1) % capacity;
    }

    table[index].key = key;
    table[index].value = value;
    table[index].occupied = true;

    size++;
}

bool HashTable::search(int key, int& value) const
{
    int index = hashFunction(key);

    for (int i = 0; i < capacity; i++)
    {
        if (!table[index].occupied)
        {
            return false;
        }

        if (table[index].key == key)
        {
            value = table[index].value;
            return true;
        }

        index = (index + 1) % capacity;
    }

    return false;
}

void HashTable::remove(int key)
{
    int index = hashFunction(key);

    for (int i = 0; i < capacity; i++)
    {
        if (!table[index].occupied)
        {
            return;
        }

        if (table[index].key == key)
        {
            table[index].occupied = false;
            size--;

            return;
        }

        index = (index + 1) % capacity;
    }
}

bool HashTable::empty() const
{
    return size == 0;
}

int HashTable::getSize() const
{
    return size;
}

void HashTable::display() const
{
    for (int i = 0; i < capacity; i++)
    {
        if (table[i].occupied)
        {
            std::cout << "[" << i << "] "
                      << table[i].key
                      << " : "
                      << table[i].value
                      << std::endl;
        }
    }
}