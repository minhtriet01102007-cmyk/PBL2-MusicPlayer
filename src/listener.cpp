#include "../include/listener.h"
#include <iostream>

Listener::Listener(){
    this->followers = 0;
    this->total_songs_played = 0;
}

Listener::Listener(std::string id_user,
                   std::string username,
                   std::string email,
                   std::string phone_number,
                   std::string password,
                   long long followers,
                   int total_songs_played)
    : User(id_user, username, email, phone_number, password){
    this->followers = followers;
    this->total_songs_played = total_songs_played;
}

Listener::Listener(const Listener& listener)
    : User(listener){
    this->followers = listener.followers;
    this->total_songs_played = listener.total_songs_played;
    this->playlists = listener.playlists;
}

Listener::~Listener(){
}

long long Listener::getFollowers() const{
    return this->followers;
}

int Listener::getTotalSongsPlayed() const{
    return this->total_songs_played;
}

int Listener::getPlaylistCount() const{
    return this->playlists.size();
}

void Listener::setFollowers(long long followers){
    this->followers = followers;
}

void Listener::setTotalSongsPlayed(int total_songs_played){
    this->total_songs_played = total_songs_played;
}

void Listener::playSong(){
    this->total_songs_played++;
}

void Listener::addPlaylist(const Playlist& playlist){
    this->playlists.push_back(playlist);
}

void Listener::removePlaylist(std::string id_playlist){
    for (int i = 0; i < this->playlists.size(); i++){
        if (this->playlists[i].getIdPlaylist() == id_playlist){
            this->playlists.erase(this->playlists.begin() + i);
            return;
        }
    }
}

const std::vector<Playlist>& Listener::getPlaylists() const{
    return this->playlists;
}

void Listener::display() const{
    std::cout << "User ID: " << this->id_user << '\n';
    std::cout << "Username: " << this->username << '\n';
    std::cout << "Email: " << this->email << '\n';
    std::cout << "Phone number: " << this->phone_number << '\n';
    std::cout << "Followers: " << this->followers << '\n';
    std::cout << "Songs played: " << this->total_songs_played << '\n';
    std::cout << "Playlist count: " << this->playlists.size() << '\n';

    for (const Playlist& playlist : this->playlists){
        std::cout << "--------------------\n";
        playlist.display();
    }
}