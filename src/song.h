#pragma once
#include <string>
#include <iostream>

class Song{
    private:
        std::string id_song;
        std::string name;
        std::string artist;
        std::string album;
        std::string type;
        int duration;          // Đơn vị: giây
        std::string date_release;
        int count_playsong;
        std::string file_path;  // Đường dẫn file nhạc (.mp3 / .wav)
        long long plays; // số lượt nghe
    public:
        Song();
        Song(const std::string& id_song, const std::string& name, const std::string& artist, const std::string& album, const std::string& type, 
             const int& duration, const std::string& date_release, const std::string& file_path, const int& count_playsong = 0, const long long& plays = 0);   
        std::string getIdSong() const;
        std::string getName() const;
        std::string getArtist() const;
        std::string getAlbum() const;
        std::string getTypeSong() const;
        int getDuration() const;
        std::string getDateRelease() const;
        int getCount() const;
        std::string getFilePath() const;
        long long getPlays() const;
        void setIdSong(const std::string& id_song);
        void setName(const std::string& name);
        void setArtist(const std::string& artist);
        void setAlbum(const std::string& album);
        void setTypeSong(const std::string& type);
        void setDuration(int duration);
        void setDateRelease(const std::string& date_release);
        void setCount(int count_playsong);
        void setFilePath(const std::string& file_path);
        void setPlays(const long long& plays);
        void increasePlayCount();
        void display() const;
        bool operator==(const Song& other) const;
};
Song::Song(){
    this->id_song = "";
    this->name = "";
    this->artist = "";
    this->album = "";
    this->type = "";
    this->duration = 0;
    this->date_release = "";
    this->file_path = "";
    this->count_playsong = 0;
    this->plays = 0;
}
Song::Song(const std::string& id_song, const std::string& name, const std::string& artist, const std::string& album, const std::string& type, 
             const int& duration, const std::string& date_release, const std::string& file_path, const int& count_playsong, const long long& plays){
    this->id_song = id_song;
    this->name = name;
    this->artist = artist;
    this->album = album;
    this->type = type;
    this->duration = duration;
    this->date_release = date_release;
    this->file_path = file_path;
    this->count_playsong = count_playsong;
    this->plays = plays;
}
std::string Song::getIdSong() const{ return this->id_song; }
std::string Song::getName() const{ return this->name; }
std::string Song::getArtist() const{ return this->artist; }
std::string Song::getAlbum() const{ return this->album; }
std::string Song::getTypeSong() const{ return this->type; }
int Song::getDuration() const{ return this->duration; }
std::string Song::getDateRelease() const{ return this->date_release; }
int Song::getCount() const{ return this->count_playsong; }
std::string Song::getFilePath() const{ return this->file_path; }
long long Song::getPlays() const{ return this->plays; }

void Song::setIdSong(const std::string& id_song){ this->id_song = id_song; }
void Song::setName(const std::string& name){ this->name = name; }
void Song::setArtist(const std::string& artist){ this->artist = artist; }
void Song::setAlbum(const std::string& album){ this->album = album; }
void Song::setTypeSong(const std::string& type){ this->type = type; }
void Song::setDuration(int duration){ this->duration = duration; }
void Song::setDateRelease(const std::string& date_release){ this->date_release = date_release; }
void Song::setCount(int count_playsong){ this->count_playsong = count_playsong; }
void Song::setFilePath(const std::string& file_path){ this->file_path = file_path; }
void Song::setPlays(const long long& plays){ this->plays = plays; }

void Song::increasePlayCount(){ this->count_playsong++; }
void Song::display() const{
    std::cout << "[" << this->id_song << "] " << this->name << " - " << this->artist 
              << " (" << this->duration / 60 << ":" << (this->duration % 60 < 10 ? "0" : "") << this->duration % 60 << ")"
              << " | Plays: " << this->count_playsong << std::endl;
}
bool Song::operator==(const Song& other) const{
    return this->id_song == other.id_song;
}