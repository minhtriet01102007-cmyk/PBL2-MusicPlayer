#pragma once
#include <string>

class ArtistInfo{
    public:
        std::string id, name, country;
        long long followers = 0;
        int songCount = 0;
        bool followed = false;   // người dùng hiện tại đang theo dõi chưa
};