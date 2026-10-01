#pragma once
#include <string>
#include <vector>
#include "song.h"

class Playlist{
private:
    std::string id_playlist;
    std::string name_playlist;
    std::string id_user;
    std::vector<Song> songs;

public:
    Playlist();

    Playlist(std::string id_playlist,
             std::string name_playlist,
             std::string id_user);

    Playlist(const Playlist& playlist);

    ~Playlist();

    std::string getIdPlaylist() const;
    std::string getNamePlaylist() const;
    std::string getIdUser() const;
    int getSongCount() const;

    void setIdPlaylist(std::string id_playlist);
    void setNamePlaylist(std::string name_playlist);
    void setIdUser(std::string id_user);

    void addSong(const Song& song);
    void removeSong(std::string id_song);

    const std::vector<Song>& getSongs() const;

    void display() const;
};