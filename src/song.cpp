#include "../include/Song.h"
#include <iostream>

Song::Song() : id_song(""), name(""), artist(""), album(""), type(""), 
               duration(0), date_release(""), count_playsong(0)
{}
Song::Song(std::string id_song, std::string name, std::string artist, std::string album,
           std::string type, int duration, std::string date_release, int count_playsong)
    : id_song(id_song), name(name), artist(artist), album(album),
      type(type), duration(duration),
      date_release(date_release), count_playsong(count_playsong)
{}
std::string Song::getIdsong() const{
    return id_song;
}
std::string Song::getName() const{
    return name;
}
std::string Song::getArtist() const{
    return artist;
}

std::string Song::getAlbum() const{
    return album;
}
std::string Song::getTypesong() const{
    return type;
}
int Song::getDuration() const{
    return duration;
}
std::string Song::getDateRelease() const{
    return date_release;
}
int Song::getCount() const{
    return count_playsong;
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
    count_playsong++;
}
void Song::resetPlayCount(){
    count_playsong = 0;
}
void Song::display() const{
    std::cout << "ID: " << id_song << std::endl;
    std::cout << "Ten bai hat: " << name << std::endl;
    std::cout << "Nghe si: " << artist << std::endl;
    std::cout << "Album: " << album << std::endl;
    std::cout << "The loai: " << type << std::endl;
    std::cout << "Thoi luong: " << duration << " giay" << std::endl;
    std::cout << "Ngay phat hanh: " << date_release << std::endl;
    std::cout << "Luot nghe: " << count_playsong << std::endl;
}