#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "user.h"
#include "playlist.h"

class Listener : public User{
    private:
        int total_songs_played; //tổng số bài hát người nghe đã phát
        std::vector<Playlist> playlists; //lưu nhiều đối tượng Playlist của một người nghe
    public:
        Listener();
        Listener(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password);
        Listener(const Listener& other);
        ~Listener() override;
        int getTotalSongsPlayed() const; //Trả về tổng số bài hát người đó đã phát
        int getPlaylistCount() const; // Trả về số lượng playlist của người nghe
        const std::vector<Playlist>& getPlaylists() const;
        void setTotalSongsPlayed(int total_songs_played);
        void increasePlayedCount();
        void addPlaylist(const Playlist& playlist);
        bool removePlaylist(const std::string& id_playlist);
        Playlist* getPlaylistById(const std::string& id_playlist);
        void display() const override;
};
inline Listener::Listener() : User(){
    this->total_songs_played = 0;
}
inline Listener::Listener(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password)
    : User(id_user, username, email, password){
    this->total_songs_played = 0;
}
inline Listener::Listener(const Listener& other) : User(other){
    this->total_songs_played = other.total_songs_played;
    this->playlists = other.playlists;
}
inline Listener::~Listener(){}
inline int Listener::getTotalSongsPlayed() const{
    return this->total_songs_played;
}
inline int Listener::getPlaylistCount() const{
    return (int)this->playlists.size();
}
inline const std::vector<Playlist>& Listener::getPlaylists() const{
    return this->playlists;
}
inline void Listener::setTotalSongsPlayed(int total_songs_played){
    this->total_songs_played = total_songs_played;
}
inline void Listener::increasePlayedCount(){
    this->total_songs_played++;
}
inline void Listener::addPlaylist(const Playlist& playlist){
    this->playlists.push_back(playlist);
}
inline bool Listener::removePlaylist(const std::string& id_playlist){
    for (auto it = this->playlists.begin(); it != this->playlists.end(); ++it){
        if (it->getIdPlaylist() == id_playlist){
            this->playlists.erase(it);
            return true;
        }
    }
    return false;
}
inline Playlist* Listener::getPlaylistById(const std::string& id_playlist){
    for (auto& pl : this->playlists){
        if (pl.getIdPlaylist() == id_playlist) return &pl; // trả về địa chỉ
    }
    return nullptr;
}
inline void Listener::display() const{
    std::cout << "[Listener] ";
    this->User::display(); 
    std::cout << " | Played: " << this->total_songs_played << " | Playlists: " << this->playlists.size() << "\n";
}