#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "user.h"
#include "playlist.h"

// Listener là một User (kế thừa).
// Sở hữu Playlist (composition): Listener mất thì playlist mất theo.
// Chỉ lưu ID của bài hát / nghệ sĩ (association)
class Listener : public User{
    private:
        int total_songs_played;                       // tổng số bài đã nghe
        std::vector<Playlist> playlists;              // playlist của người này
        std::vector<std::string> liked_song_ids;      // id các bài đã thích
        std::vector<std::string> followed_artist_ids; // id các nghệ sĩ đang theo dõi
        std::vector<std::string> recent_song_ids;     // id bài nghe gần đây, cũ -> mới
        static bool containsId(const std::vector<std::string>& v, const std::string& id);
        static bool eraseId(std::vector<std::string>& v, const std::string& id);
    public:
        Listener();
        Listener(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password);
        Role getRole() const override;
        bool isAdmin() const override;
        bool canManageCatalog() const override;
        bool canManageUsers() const override;
        User* clone() const override;
        void display() const override;

        int getTotalSongsPlayed() const;
        void setTotalSongsPlayed(int total_songs_played);
        void increasePlayedCount();

        const std::vector<Playlist>& getPlaylists() const;
        bool addPlaylist(const Playlist& playlist);                 
        bool removePlaylist(const std::string& id_playlist);
        Playlist* getPlaylistById(const std::string& id_playlist);  

        bool likeSong(const std::string& id_song);                 
        bool unlikeSong(const std::string& id_song);
        bool isLiked(const std::string& id_song) const;
        const std::vector<std::string>& getLikedSongIds() const;

        bool followArtist(const std::string& id_artist);            
        bool unfollowArtist(const std::string& id_artist);
        bool isFollowing(const std::string& id_artist) const;
        const std::vector<std::string>& getFollowedArtistIds() const;

        void addRecentSong(const std::string& id_song);
        const std::vector<std::string>& getRecentSongIds() const;

        void removeSongReferences(const std::string& id_song);
        void removeArtistReference(const std::string& id_artist);
};
Listener::Listener() : User(){
    this->total_songs_played = 0;
}
Listener::Listener(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password)
    : User(id_user, username, email, password){
    this->total_songs_played = 0;
}
bool Listener::containsId(const std::vector<std::string>& v, const std::string& id){
    return std::find(v.begin(), v.end(), id) != v.end();
}
bool Listener::eraseId(std::vector<std::string>& v, const std::string& id){
    auto it = std::find(v.begin(), v.end(), id);
    if (it == v.end()) return false;
    v.erase(it);
    return true;
}
Role Listener::getRole() const{ 
    return Role::LISTENER; 
}
bool Listener::isAdmin() const{ 
    return false; 
}
bool Listener::canManageCatalog() const{ 
    return false; 
}
bool Listener::canManageUsers() const{ 
    return false; 
}
User* Listener::clone() const{ 
    return new Listener(*this); 
}
void Listener::display() const{
    std::cout << "[Listener] ";
    this->User::display();
    std::cout << " | Played: " << this->total_songs_played
              << " | Playlists: " << this->playlists.size()
              << " | Liked: " << this->liked_song_ids.size() << std::endl;
}
int Listener::getTotalSongsPlayed() const{ 
    return this->total_songs_played; 
}
void Listener::setTotalSongsPlayed(int total_songs_played){ 
    this->total_songs_played = total_songs_played; 
}
void Listener::increasePlayedCount(){ 
    this->total_songs_played++; 
}
const std::vector<Playlist>& Listener::getPlaylists() const{ 
    return this->playlists; 
}
bool Listener::addPlaylist(const Playlist& playlist){
    if (this->getPlaylistById(playlist.getIdPlaylist()) != nullptr) return false;
    Playlist p = playlist;
    p.setIdUser(this->getIdUser()); // playlist luôn mang id của chủ sở hữu
    this->playlists.push_back(p);
    return true;
}
bool Listener::removePlaylist(const std::string& id_playlist){
    for (auto it = this->playlists.begin(); it != this->playlists.end(); ++it){
        if (it->getIdPlaylist() == id_playlist){
            this->playlists.erase(it);
            return true;
        }
    }
    return false;
}
Playlist* Listener::getPlaylistById(const std::string& id_playlist){
    for (auto& pl : this->playlists){
        if (pl.getIdPlaylist() == id_playlist) return &pl;
    }
    return nullptr;
}
bool Listener::likeSong(const std::string& id_song){
    if (id_song.empty() || containsId(this->liked_song_ids, id_song)) return false;
    this->liked_song_ids.push_back(id_song);
    return true;
}
bool Listener::unlikeSong(const std::string& id_song){ 
    return eraseId(this->liked_song_ids, id_song); 
}
bool Listener::isLiked(const std::string& id_song) const{ 
    return containsId(this->liked_song_ids, id_song); 
}
const std::vector<std::string>& Listener::getLikedSongIds() const{ 
    return this->liked_song_ids; 
}
bool Listener::followArtist(const std::string& id_artist){
    if (id_artist.empty() || containsId(this->followed_artist_ids, id_artist)) return false;
    this->followed_artist_ids.push_back(id_artist);
    return true;
}
bool Listener::unfollowArtist(const std::string& id_artist){ 
    return eraseId(this->followed_artist_ids, id_artist); 
}
bool Listener::isFollowing(const std::string& id_artist) const{ 
    return containsId(this->followed_artist_ids, id_artist); 
}
const std::vector<std::string>& Listener::getFollowedArtistIds() const{ 
    return this->followed_artist_ids; 
}
void Listener::addRecentSong(const std::string& id_song){
    if (id_song.empty()) return;
    eraseId(this->recent_song_ids, id_song);  // nghe lại thì chuyển lên cuối, không bị lặp
    this->recent_song_ids.push_back(id_song);
}
const std::vector<std::string>& Listener::getRecentSongIds() const{ 
    return this->recent_song_ids; 
}
void Listener::removeSongReferences(const std::string& id_song){
    for (auto& pl : this->playlists) pl.removeSong(id_song);
    eraseId(this->liked_song_ids, id_song);
    eraseId(this->recent_song_ids, id_song);
}
void Listener::removeArtistReference(const std::string& id_artist){
    eraseId(this->followed_artist_ids, id_artist);
}