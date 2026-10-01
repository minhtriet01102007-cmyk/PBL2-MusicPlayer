#include "../include/playlist.h"
#include <iostream>

Playlist::Playlist(){
    this->id_playlist = "";
    this->name_playlist = "";
    this->id_user = "";
}

Playlist::Playlist(std::string id_playlist,
                   std::string name_playlist,
                   std::string id_user){
    this->id_playlist = id_playlist;
    this->name_playlist = name_playlist;
    this->id_user = id_user;
}

Playlist::Playlist(const Playlist& playlist){
    this->id_playlist = playlist.id_playlist;
    this->name_playlist = playlist.name_playlist;
    this->id_user = playlist.id_user;
    this->songs = playlist.songs;
}

Playlist::~Playlist(){
}

std::string Playlist::getIdPlaylist() const{
    return this->id_playlist;
}

std::string Playlist::getNamePlaylist() const{
    return this->name_playlist;
}

std::string Playlist::getIdUser() const{
    return this->id_user;
}

int Playlist::getSongCount() const{
    return this->songs.size();
}

void Playlist::setIdPlaylist(std::string id_playlist){
    this->id_playlist = id_playlist;
}

void Playlist::setNamePlaylist(std::string name_playlist){
    this->name_playlist = name_playlist;
}

void Playlist::setIdUser(std::string id_user){
    this->id_user = id_user;
}

void Playlist::addSong(const Song& song){
    this->songs.push_back(song);
}

void Playlist::removeSong(std::string id_song){
    for (int i = 0; i < this->songs.size(); i++){
        if (this->songs[i].getIdsong() == id_song){
            this->songs.erase(this->songs.begin() + i);
            return;
        }
    }
}

const std::vector<Song>& Playlist::getSongs() const{
    return this->songs;
}

void Playlist::display() const{
    std::cout << "Playlist ID: " << this->id_playlist << '\n';
    std::cout << "Playlist: " << this->name_playlist << '\n';
    std::cout << "User ID: " << this->id_user << '\n';
    std::cout << "Song count: " << this->songs.size() << '\n';

    for (const Song& song : this->songs){
        std::cout << "--------------------\n";
        song.display();
    }
}