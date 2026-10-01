#include "../include/song.h"
#include <iostream>

Song::Song(){
    this->id_song = "";
    this->name = "";
    this->artist = "";
    this->album = "";
    this->type = "";
    this->duration = 0;
    this->date_release = "";
    this->count_playsong = 0;
    this->liked = false;
}
Song::Song(std::string id_song, std::string name, std::string artist, std::string album, std::string type,
           int duration, std::string date_release, int count_playsong){
    this->id_song = id_song;
    this->name = name;
    this->artist = artist;
    this->album = album;
    this->type = type;
    this->duration = duration;
    this->date_release = date_release;
    this->count_playsong = count_playsong;
    this->liked = false;
}
std::string Song::getIdsong() const{
    return this->id_song;
}
std::string Song::getName() const{
    return this->name;
}
std::string Song::getArtist() const{
    return this->artist;
}
std::string Song::getAlbum() const{
    return this->album;
}
std::string Song::getTypesong() const{
    return this->type;
}
int Song::getDuration() const{
    return this->duration;
}
std::string Song::getDateRelease() const{
    return this->date_release;
}
int Song::getCount() const{
    return this->count_playsong;
}
bool Song::isLiked() const{
    return this->liked;
}
void Song::setIdsong(std::string id_song){
    this->id_song = id_song;
}
void Song::setName(std::string name){
    this->name = name;
}
void Song::setArtist(std::string artist){
    this->artist = artist;
}
void Song::setAlbum(std::string album){
    this->album = album;
}
void Song::setTypesong(std::string type){
    this->type = type;
}
void Song::setDuration(int duration){
    this->duration = duration;
}
void Song::setDateRelease(std::string date_release){
    this->date_release = date_release;
}
void Song::setCount(int count_playsong){
    this->count_playsong = count_playsong;
}
void Song::increasePlayCount(){
    this->count_playsong++;
}
void Song::resetPlayCount(){
    this->count_playsong = 0;
}
void Song::like(){
    this->liked = true;
}
void Song::unlike(){
    this->liked = false;
}
void Song::display() const{
    std::cout << "ID: " << this->id_song << '\n';
    std::cout << "Name: " << this->name << '\n';
    std::cout << "Artist: " << this->artist << '\n';
    std::cout << "Album: " << this->album << '\n';
    std::cout << "Type: " << this->type << '\n';
    std::cout << "Duration: " << this->duration << " seconds\n";
    std::cout << "Release date: " << this->date_release << '\n';
    std::cout << "Play count: " << this->count_playsong << '\n';
    std::cout << "Liked: " << (this->liked ? "Yes" : "No") << '\n';
}