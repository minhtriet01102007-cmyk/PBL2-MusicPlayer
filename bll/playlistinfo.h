#pragma once
#include <string>

class PlaylistInfo{
    public:
        std::string id, name;
        int songCount = 0;
        int totalSeconds = 0;
};