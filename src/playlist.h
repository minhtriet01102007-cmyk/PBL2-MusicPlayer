#pragma once
#include <string>
#include <iostream>
#include "song.h"
#include "dsa/dlinkedlist/dlinkedlist.h"

class Playlist{
    private:
        std::string id_playlist;
        std::string name_playlist;
        std::string id_user;
        DoublyLinkedList<Song*> songs; 
    public:
        Playlist();
        Playlist(std::string id_playlist, std::string name_playlist, std::string id_user);
        std::string getIdPlaylist() const;
        std::string getNamePlaylist() const;
        std::string getIdUser() const;
        int getSongCount() const;   
        void setIdPlaylist(const std::string& id_playlist);
        void setNamePlaylist(const std::string& name_playlist);
        void setIdUser(const std::string& id_user);
        void addSong(Song* song);
        bool removeSong(Song* song);
        DoublyLinkedList<Song*>& getQueue();
        void display() const;
};

inline Playlist::Playlist(){
    this->id_playlist = "";
    this->name_playlist = "";
    this->id_user = "";
}
inline Playlist::Playlist(std::string id_playlist, std::string name_playlist, std::string id_user){
    this->id_playlist = id_playlist;
    this->name_playlist = name_playlist;
    this->id_user = id_user;
}
inline std::string Playlist::getIdPlaylist() const{ return this->id_playlist; }
inline std::string Playlist::getNamePlaylist() const{ return this->name_playlist; }
inline std::string Playlist::getIdUser() const{ return this->id_user; }
inline int Playlist::getSongCount() const{ return this->songs.getSize(); }

inline void Playlist::setIdPlaylist(const std::string& id_playlist){ this->id_playlist = id_playlist; }
inline void Playlist::setNamePlaylist(const std::string& name_playlist){ this->name_playlist = name_playlist; }
inline void Playlist::setIdUser(const std::string& id_user){ this->id_user = id_user; }

inline void Playlist::addSong(Song* song){
    if (song != nullptr) this->songs.pushBack(song);
}
inline bool Playlist::removeSong(Song* song){
    return this->songs.remove(song);
}
inline DoublyLinkedList<Song*>& Playlist::getQueue(){
    return this->songs;
}
inline void Playlist::display() const{
    std::cout << "Playlist: " << this->name_playlist << " (Total: " << this->getSongCount() << " songs)\n";
}