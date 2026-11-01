#pragma once
#include "songinfo.h"
#include "src/song.h"
#include "dal/songrepository.h"

// Chuyển thực thể Song (tầng dữ liệu) thành SongInfo (gửi lên giao diện)
SongInfo toSongInfo(const Song& s, const ISongRepository& repo){
    SongInfo i;
    i.id = s.getIdSong();
    i.name = s.getName();
    i.artist = s.getArtist();
    i.album = s.getAlbum();
    i.type = s.getTypeSong();
    i.date = s.getDateRelease();
    i.duration = s.getDuration();
    i.count = s.getPlays();
    i.path = repo.resolveAudioPath(s);
    return i;
}
