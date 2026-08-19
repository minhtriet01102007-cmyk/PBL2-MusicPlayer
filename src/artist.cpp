#include "../include/artist.h"
#include <iostream>

Artist::Artist() : id_artist(""), name_artist(""), country(""), type_play("")
{}
Artist::Artist(std::string id_artist, std::string name_artist, std::string country, std::string type_play)
    : id_artist(id_artist), name_artist(name_artist), country(country), type_play(type_play)
{}
std::string Artist::getIdArtist() const{
    return id_artist;
}
std::string Artist::getNameArtist() const{
    return name_artist;
}
std::string Artist::getCountry() const{
    return country;
}
std::string Artist::getTypePlay() const{
    return type_play;
}
void Artist::setIdArtist(std::string id_artist){
    this->id_artist = id_artist;
}
void Artist::setNameArtist(std::string name_artist){
    this->name_artist = name_artist;
}
void Artist::setCountry(std::string country){
    this->country = country;
}
void Artist::setTypePlay(std::string type_play){
    this->type_play = type_play;
}
void Artist::show() const{
    std::cout << "ID Artist: " << id_artist << "\n";
    std::cout << "Ten nghe si: " << name_artist << "\n";
    std::cout << "Quoc gia: " << country << "\n";
    std::cout << "The loai choi: " << type_play << "\n";
}
