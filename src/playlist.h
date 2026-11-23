#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "dsa/dlinkedlist/dlinkedlist.h"
#include "dsa/dlinkedlist/dlistutil.h"

class Playlist{
    private:
        std::string id_playlist;
        std::string name_playlist;
        std::string id_user; 
        DoublyLinkedList<std::string> songs; // composition
    public:
        Playlist();
        Playlist(const std::string& id_playlist, const std::string& name_playlist, const std::string& id_user);
        std::string getIdPlaylist() const;
        std::string getNamePlaylist() const;
        std::string getIdUser() const;
        int getSongCount() const;
        void setIdPlaylist(const std::string& id_playlist);
        void setNamePlaylist(const std::string& name_playlist);
        void setIdUser(const std::string& id_user);
        bool addSong(const std::string& id_song);    
        bool removeSong(const std::string& id_song);
        bool containsSong(const std::string& id_song) const;
        void clearSongs();
        std::vector<std::string> getSongIds() const;                  // theo thứ tự phát
        const DoublyLinkedList<std::string>& getSongs() const;        
        void display() const;
};

Playlist::Playlist(){
    this->id_playlist = "";
    this->name_playlist = "";
    this->id_user = "";
}
Playlist::Playlist(const std::string& id_playlist, const std::string& name_playlist, const std::string& id_user){
    this->id_playlist = id_playlist;
    this->name_playlist = name_playlist;
    this->id_user = id_user;
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
    return this->songs.getSize(); 
}
void Playlist::setIdPlaylist(const std::string& id_playlist){ 
    this->id_playlist = id_playlist; 
}
void Playlist::setNamePlaylist(const std::string& name_playlist){ 
    this->name_playlist = name_playlist; 
}
void Playlist::setIdUser(const std::string& id_user){ 
    this->id_user = id_user; 
}
bool Playlist::addSong(const std::string& id_song){
    if (id_song.empty() || this->songs.contains(id_song)) return false;
    this->songs.pushBack(id_song);
    return true;
}
bool Playlist::removeSong(const std::string& id_song){ 
    return this->songs.remove(id_song); 
}
bool Playlist::containsSong(const std::string& id_song) const{ 
    return this->songs.contains(id_song); 
}
void Playlist::clearSongs(){ 
    this->songs.clear(); 
}
std::vector<std::string> Playlist::getSongIds() const{
    DoublyLinkedList<std::string> copy = this->songs; // sao chép để không làm đổi vị trí đang phát của playlist
    return dlist::toVector(copy);
}
const DoublyLinkedList<std::string>& Playlist::getSongs() const{ 
    return this->songs; 
}
void Playlist::display() const{
    std::cout << "Playlist: " << this->name_playlist << " (Total: " << this->getSongCount() << " songs)" << std::endl;
}