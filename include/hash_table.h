#pragma once
#include "song.h"
#include "search.h"

class HashTable{
    private:
        Song* id_song;
        Song* name;
    public:
        HashTable();
        HashTable();
        ~HashTable();
};