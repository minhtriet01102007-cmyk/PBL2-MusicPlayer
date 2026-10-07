#pragma once
#include <string>
#include <iostream>

class Album{
    private:
        std::string id_album;
        std::string name_album;
        std::string artist;
        std::string date_release;
    public:
        Album();
        Album(std::string id_album, std::string name_album, std::string artist, std::string date_release);
        Album(const Album& other);
        ~Album();
        std::string getIdAlbum() const;
        std::string getNameAlbum() const;
        std::string getArtist() const;
        std::string getDateRelease() const;
        void setIdAlbum(const std::string& id_album);
        void setNameAlbum(const std::string& name_album);
        void setArtist(const std::string& artist);
        void setDateRelease(const std::string& date_release);
        void display() const;
};
inline Album::Album(){
    this->id_album = "";
    this->name_album = "";
    this->artist = "";
    this->date_release = "";
}
inline Album::Album(std::string id_album, std::string name_album, std::string artist, std::string date_release){
    this->id_album = id_album;
    this->name_album = name_album;
    this->artist = artist;
    this->date_release = date_release;
}
inline Album::Album(const Album& other){
    this->id_album = other.id_album;
    this->name_album = other.name_album;
    this->artist = other.artist;
    this->date_release = other.date_release;
}
inline Album::~Album(){}
inline std::string Album::getIdAlbum() const{
    return this->id_album;
}
inline std::string Album::getNameAlbum() const{
    return this->name_album;
}
inline std::string Album::getArtist() const{
    return this->artist;
}
inline std::string Album::getDateRelease() const{
    return this->date_release;
}
inline void Album::setIdAlbum(const std::string& id_album){
    this->id_album = id_album;
}
inline void Album::setNameAlbum(const std::string& name_album){
    this->name_album = name_album;
}
inline void Album::setArtist(const std::string& artist){
    this->artist = artist;
}
inline void Album::setDateRelease(const std::string& date_release){
    this->date_release = date_release;
}
inline void Album::display() const{
    std::cout << "[Album] " << this->name_album << " by " << this->artist << " (" << this->date_release << ")\n";
}