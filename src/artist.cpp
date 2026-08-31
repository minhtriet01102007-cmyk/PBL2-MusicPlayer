#include "../include/artist.h"
#include <iostream>

Artist::Artist() : id_artist(""), name_artist(""), bio(""), country(""), followers(0)
{}
Artist::Artist(std::string id_artist, std::string name_artist, std::string bio,
               std::string country, long long followers)
    : id_artist(id_artist), name_artist(name_artist), bio(bio), country(country), followers(followers)
{}
std::string Artist::getIdArtist() const{
    return id_artist;
}
std::string Artist::getNameArtist() const{
    return name_artist;
}
std::string Artist::getBio() const{
    return bio;
}
std::string Artist::getCountry() const{
    return country;
}
long long Artist::getFollowers() const{
    return followers;
}
void Artist::setIdArtist(std::string id_artist){
    this->id_artist = id_artist;
}
void Artist::setNameArtist(std::string name_artist){
    this->name_artist = name_artist;
}
void Artist::setBio(std::string bio){
    this->bio = bio;
}
void Artist::setCountry(std::string country){
    this->country = country;
}
void Artist::setFollowers(long long followers){
    this->followers = followers;
}
void Artist::show() const{
    std::cout << "ID Artist: " << id_artist << "\n";
    std::cout << "Ten nghe si: " << name_artist << "\n";
    std::cout << "Tieu su: " << bio << "\n";
    std::cout << "Quoc gia: " << country << "\n";
    std::cout << "Followers: " << followers << "\n";
}
