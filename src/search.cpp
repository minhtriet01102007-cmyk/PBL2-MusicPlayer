#include "../include/search.h"
#include <string>

int Search::linearSearchById(Song arr[], int size, const std::string& id_song){
    for (int i = 0; i < size; ++i){
        if (arr[i].getIdsong() == id_song){
            return i;
        }
    }
    return -1;
}