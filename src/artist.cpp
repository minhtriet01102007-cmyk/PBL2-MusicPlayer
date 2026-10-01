#include "../include/artist.h"
#include <iostream>

Artist::Artist(){
    this->id_artist = "";
    this->name_artist = "";
    this->country = "";
    this->followers = 0;
}
Artist::Artist(std::string id_artist, std::string name_artist, std::string country, long long followers){
    this->id_artist = id_artist;
    this->name_artist = name_artist;
    this->country = country;
    this->followers = followers;
}
std::string Artist::getIdArtist() const{
    return this->id_artist;
}
std::string Artist::getNameArtist() const{
    return this->name_artist;
}
std::string Artist::getCountry() const{
    return this->country;
}
long long Artist::getFollowers() const{
    return this->followers;
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
void Artist::setFollowers(long long followers){
    this->followers = followers;
}
void Artist::show() const{
    std::cout << "Artist ID: " << this->id_artist << '\n';
    std::cout << "Artist: " << this->name_artist << '\n';
    std::cout << "Country: " << this->country << '\n';
    std::cout << "Followers: " << this->followers << '\n';
}