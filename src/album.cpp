#include "../include/album.h"
#include <iostream>

Album::Album() : id_album(""), name_album(""), artist(""), date_release(""), type("")
{}
Album::Album(std::string id_album, std::string name_album, std::string artist, 
             std::string date_release, std::string type) 
        : id_album(id_album), name_album(name_album), artist(artist), 
          date_release(date_release), type(type)
{}
std::string Album::getIdAlbum() const{
    return id_album;
}
std::string Album::getNameAlbum() const{
    return name_album;
}
std::string Album::getArtist() const{
    return artist;
}
std::string Album::getDateRelease() const{
    return date_release;
}
std::string Album::getType() const{
    return type;
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
void Album::setType(std::string type){
    this->type = type;
}
void Album::display() const{
    std::cout << "ID Album: " << id_album << "\n";
    std::cout << "Ten Album: " << name_album << "\n";
    std::cout << "Ten nghe si: " << artist << "\n";
    std::cout << "Ngay phat hanh: " << date_release << "\n";
    std::cout << "The loai: " << type << "\n";
}
