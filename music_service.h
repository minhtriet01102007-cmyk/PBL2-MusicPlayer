#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "comparesongs.h"
#include "src/song.h"
#include "src/user.h"
#include "src/listener.h"
#include "src/admin.h"
#include "src/album.h"
#include "src/artist.h"
#include "src/playlist.h"
#include "dsa/dlinkedlist/dlinkedlist.h"
#include "dsa/hashtable/hashtable.h"
#include "dsa/maxheap/maxheap.h"

class MusicService{
    private:
        DoublyLinkedList<Song> playlist;                // Danh sách phát chính
        HashTable<std::string, Song> songTable;         // Bảng băm tra cứu bài hát theo id_song
        MaxHeap<Song, CompareSongByPlays> trendingHeap; // Max Heap quản lý xu hướng bài hát
        std::vector<Song> allSongs;                     // Danh sách toàn bộ bài hát để nạp lên giao diện
        std::vector<Listener> listeners;                // Danh sách người nghe
        std::vector<Album> albums;                      // Danh sách album
        std::vector<Artist> artists;                    // Danh sách nghệ sĩ
    public:
        MusicService();
        ~MusicService();
        bool loadSongsFromFile(const std::string& filePath);
        bool loadUsersFromFile(const std::string& filePath);
        bool loadAlbumsFromFile(const std::string& filePath);
        bool loadArtistsFromFile(const std::string& filePath);
        void loadListeners();
        std::vector<Song> getAllSongs() const;
        bool searchSongById(const std::string& id, Song& outSong);
        void addSongToPlaylist(const Song& song);
        bool checkLogin(const std::string& username, const std::string& password, User*& outUser);
        bool checkLoginAndRole(const std::string& username, const std::string& password, std::string& outRole);
        void addListeningHistory(const std::string& username, const std::string& songId);
        bool registerListener(std::string username, std::string password, std::string name);
        void savePlaylistsToFile();
};
inline MusicService::MusicService(){
    loadSongsFromFile("data/songs.txt");
    loadUsersFromFile("data/users.txt");
    loadAlbumsFromFile("data/albums.txt");
    loadArtistsFromFile("data/artists.txt");
}
inline MusicService::~MusicService(){
}
inline bool MusicService::loadSongsFromFile(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()){
        std::cout << "Khong the mo file: " << filePath << std::endl;
        return false;
    }
    allSongs.clear();
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id_song, name, artist, album, type, date_release, file_path;
        int duration = 0;
        int count_playsong = 0;
        long long plays = 0;
        std::string segment;
        if (std::getline(ss, id_song, '|') && std::getline(ss, name, '|') && std::getline(ss, artist, '|') &&
            std::getline(ss, album, '|') && std::getline(ss, type, '|')){
            if (std::getline(ss, segment, '|')) duration = std::stoi(segment); // stoi: string to interger
            std::getline(ss, date_release, '|');
            if (std::getline(ss, segment, '|')) count_playsong = std::stoi(segment);
            std::getline(ss, file_path);

            plays = count_playsong; // Đồng bộ lượt nghe
            Song song(id_song, name, artist, album, type, duration, date_release, file_path, count_playsong, plays);
            allSongs.push_back(song);
            playlist.pushBack(song);
            songTable.insert(id_song, song);
            trendingHeap.insert(song);
        }
    }
    file.close();
    playlist.resetPlayback();
    return true;
}
inline bool MusicService::loadUsersFromFile(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    listeners.clear();
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, username, email, password, role;
        if (std::getline(ss, id, '|') && std::getline(ss, username, '|') && std::getline(ss, email, '|') && std::getline(ss, password, '|')){
            Listener listener(id, username, email, password);
            listeners.push_back(listener);
        }
    }
    file.close();
    return true;
}
inline bool MusicService::loadAlbumsFromFile(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    albums.clear();
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, name, artist, date;
        if (std::getline(ss, id, '|') && std::getline(ss, name, '|') && std::getline(ss, artist, '|') && std::getline(ss, date, '|')){
            albums.emplace_back(id, name, artist, date);
        }
    }
    file.close();
    return true;
}
inline bool MusicService::loadArtistsFromFile(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    artists.clear();
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, name, country;
        long long followers = 0;
        std::string seg;
        if (std::getline(ss, id, '|') && std::getline(ss, name, '|') && std::getline(ss, country, '|')){
            if (std::getline(ss, seg, '|')) followers = std::stoll(seg); // stoll: chuyển string sang long long 
            artists.emplace_back(id, name, country, followers);
        }
    }
    file.close();
    return true;
}
inline void MusicService::loadListeners(){
    std::ifstream file("data/listener.txt");
    if (!file.is_open()){
        return; 
    }
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, username, password, name;
        std::getline(ss, id, '|');
        std::getline(ss, username, '|');
        std::getline(ss, password, '|');
        std::getline(ss, name, '|');
    }
    file.close();
}
inline std::vector<Song> MusicService::getAllSongs() const{
    return allSongs;
}
inline bool MusicService::searchSongById(const std::string& id, Song& outSong){
    return songTable.search(id, outSong);
}
inline void MusicService::addSongToPlaylist(const Song& song){
    playlist.pushBack(song);
}
inline bool MusicService::checkLogin(const std::string& username, const std::string& password, User*& outUser){
    for (auto& listener : listeners){
        if (listener.getUsername() == username && listener.getPassword() == password){
            outUser = &listener;
            return true;
        }
    }
    return false;
}
inline bool MusicService::checkLoginAndRole(const std::string& username, const std::string& password, std::string& outRole){
    if (username == "admin" && password == "admin123"){
        outRole = "ADMIN";
        return true;
    }
    for (auto& listener : listeners){
        if (listener.getUsername() == username && listener.getPassword() == password){
            outRole = "LISTENER";
            return true;
        }
    }
    return false;
}
inline void MusicService::addListeningHistory(const std::string& username, const std::string& songId){
    std::ofstream file("data/history.txt", std::ios::app); 
    if (file.is_open()){
        file << username << " | " << songId << "\n";
        file.close();
    }
}
inline bool MusicService::registerListener(std::string username, std::string password, std::string name) {
    std::ofstream file("data/listener.txt", std::ios::app);
    if (!file.is_open()) return false;
    std::string newId = "L" + std::to_string(time(0)); 
    file << newId << " | " << username << " | " << password << " | " << name << "\n";
    file.close();
    return true;
}
inline void MusicService::savePlaylistsToFile(){
    std::ofstream file("data/playlists.txt", std::ios::out);
    if (!file.is_open()) return;
    file.close();
}