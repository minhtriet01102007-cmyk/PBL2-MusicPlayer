#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

class Album{
    private:
        std::string id_album;
        std::string name_album;
        std::string id_artist;               
        std::string date_release;            
        std::vector<std::string> id_songs; // tập hợp các song theo thứ tự track (aggregation bằng id)
    public:
        Album();
        Album(const std::string& id_album, const std::string& name_album, const std::string& id_artist, const std::string& date_release);

        std::string getIdAlbum() const;
        std::string getNameAlbum() const;
        std::string getIdArtist() const;
        std::string getDateRelease() const;
        const std::vector<std::string>& getSongIds() const;
        int getTrackCount() const;

        void setIdAlbum(const std::string& id_album);
        void setNameAlbum(const std::string& name_album);
        void setIdArtist(const std::string& id_artist);
        void setDateRelease(const std::string& date_release);

        bool addSong(const std::string& id_song);    
        bool removeSong(const std::string& id_song);
        bool containsSong(const std::string& id_song) const; // kiểm tra một bài hát có tồn tại trong Album
        void display() const;
};
Album::Album(){
    this->id_album = "";
    this->name_album = "";
    this->id_artist = "";
    this->date_release = "";
}
Album::Album(const std::string& id_album, const std::string& name_album, const std::string& id_artist, const std::string& date_release){
    this->id_album = id_album;
    this->name_album = name_album;
    this->id_artist = id_artist;
    this->date_release = date_release;
}
std::string Album::getIdAlbum() const{ 
    return this->id_album; 
}
std::string Album::getNameAlbum() const{
    return this->name_album; 
}
std::string Album::getIdArtist() const{ 
    return this->id_artist; 
}
std::string Album::getDateRelease() const{ 
    return this->date_release; 
}
const std::vector<std::string>& Album::getSongIds() const{ 
    return this->id_songs; 
}
int Album::getTrackCount() const{ 
    return (int)this->id_songs.size(); 
}
void Album::setIdAlbum(const std::string& id_album){ 
    this->id_album = id_album; 
}
void Album::setNameAlbum(const std::string& name_album){ 
    this->name_album = name_album; 
}
void Album::setIdArtist(const std::string& id_artist){ 
    this->id_artist = id_artist; 
}
void Album::setDateRelease(const std::string& date_release){ 
    this->date_release = date_release; 
}
bool Album::addSong(const std::string& id_song){
    if (id_song.empty() || this->containsSong(id_song)) return false;
    this->id_songs.push_back(id_song);
    return true;
}
bool Album::removeSong(const std::string& id_song){
    auto it = std::find(this->id_songs.begin(), this->id_songs.end(), id_song); // std::find(first, last, value);
    if (it == this->id_songs.end()) return false;
    this->id_songs.erase(it);
    return true;
}
bool Album::containsSong(const std::string& id_song) const{
    return std::find(this->id_songs.begin(), this->id_songs.end(), id_song) != this->id_songs.end();
}
void Album::display() const{
    std::cout << "[Album] " << this->name_album << " (artist: " << this->id_artist << ", " << this->date_release
              << ") | Tracks: " << this->id_songs.size() << std::endl;
}