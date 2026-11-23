#pragma once
#include <string>

class SongInfo{
    public:
        std::string id, name, artist, album, idArtist, idAlbum, type, date;
        std::string path;        // đường dẫn file nhạc thật, chỉ được điền khi bài đang được phát
        int duration = 0;        // giây
        long long plays = 0;     // tổng lượt nghe
        bool liked = false;      // người dùng hiện tại đã thích chưa
};