#pragma once
#include <string>

struct SongInfo{
    std::string id, name, artist, album, type, date;
    int duration = 0;         
    long long count = 0;       
    std::string path;          
};
