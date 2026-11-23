#pragma once
#include <string>

class UserInfo{
    public:
        std::string id, username, email, role;
        bool active = true;
        int totalPlayed = 0;
        int playlistCount = 0;
};

class StatsInfo{
    public:
        int songs = 0, artists = 0, albums = 0, users = 0, listeners = 0;
        long long totalPlays = 0;
};