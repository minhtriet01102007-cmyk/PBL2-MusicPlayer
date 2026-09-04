#pragma once
#include "song.h"
#include <string>

class Search{
    public:
        static int linearSearchById(Song arr[], int size, const std::string& id_song);
};