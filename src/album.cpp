#include "../include/album.h"
#include <iostream>

Album::Album(){
    this->id_album = "";
    this->name_album = "";
    this->artist = "";
    this->date_release = "";
}
Album::Album(std::string id_album, std::string name_album, std::string artist, std::string date_release){
    this->id_album = id_album;
    this->name_album = name_album;
    this->artist = artist;
    this->date_release = date_release;
}
Album::Album(const Album& ab){
    this->id_album = ab.id_album;
    this->name_album = ab.name_album;
    this->artist = ab.artist;
    this->date_release = ab.date_release;
}
Album::~Album(){
}
std::string Album::getIdAlbum() const{
    return this->id_album;
}
std::string Album::getNameAlbum() const{
    return this->name_album;
}
std::string Album::getArtist() const{
    return this->artist;
}
std::string Album::getDateRelease() const{
    return this->date_release;
}
void Album::setIdAlbum(std::string id_album){
    this->id_album = id_album;
}
void Album::setNameAlbum(std::string name_album){
    this->name_album = name_album;
}
void Album::setArtist(std::string artist){
    this->artist = artist;
}
void Album::setDateRelease(std::string date_release){
    this->date_release = date_release;
}
void Album::display() const{
    std::cout << "Album ID: " << this->id_album << '\n';
    std::cout << "Album: " << this->name_album << '\n';
    std::cout << "Artist: " << this->artist << '\n';
    std::cout << "Release date: " << this->date_release << '\n';
}