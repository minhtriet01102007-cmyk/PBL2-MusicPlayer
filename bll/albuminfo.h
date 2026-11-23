#pragma once
#include <string>

class AlbumInfo{
    public:
        std::string id, name, idArtist, artist, date;
        int trackCount = 0;
        int totalSeconds = 0;
};