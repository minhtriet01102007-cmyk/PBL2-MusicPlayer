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
        int duration;           // Đơn vị: giây
        std::string date_release;
        int count_playsong;
        std::string file_path;  // Đường dẫn file nhạc (.mp3 / .wav)
    public:
        Song();
        Song(std::string id_song, std::string name, std::string artist, std::string album, std::string type, 
             int duration, std::string date_release, std::string file_path, int count_playsong = 0);
        std::string getIdSong() const;
        std::string getName() const;
        std::string getArtist() const;
        std::string getAlbum() const;
        std::string getTypeSong() const;
        int getDuration() const;
        std::string getDateRelease() const;
        int getCount() const;
        std::string getFilePath() const;
        void setIdSong(const std::string& id_song);
        void setName(const std::string& name);
        void setArtist(const std::string& artist);
        void setAlbum(const std::string& album);
        void setTypeSong(const std::string& type);
        void setDuration(int duration);
        void setDateRelease(const std::string& date_release);
        void setCount(int count_playsong);
        void setFilePath(const std::string& file_path);
        void increasePlayCount();
        void display() const;
        bool operator==(const Song& other) const;
};

inline Song::Song(){
    this->id_song = "";
    this->name = "";
    this->artist = "";
    this->album = "";
    this->type = "";
    this->duration = 0;
    this->date_release = "";
    this->file_path = "";
    this->count_playsong = 0;
}
inline Song::Song(std::string id_song, std::string name, std::string artist, std::string album, std::string type, 
                  int duration, std::string date_release, std::string file_path, int count_playsong){
    this->id_song = id_song;
    this->name = name;
    this->artist = artist;
    this->album = album;
    this->type = type;
    this->duration = duration;
    this->date_release = date_release;
    this->file_path = file_path;
    this->count_playsong = count_playsong;
}
inline std::string Song::getIdSong() const{ return this->id_song; }
inline std::string Song::getName() const{ return this->name; }
inline std::string Song::getArtist() const{ return this->artist; }
inline std::string Song::getAlbum() const{ return this->album; }
inline std::string Song::getTypeSong() const{ return this->type; }
inline int Song::getDuration() const{ return this->duration; }
inline std::string Song::getDateRelease() const{ return this->date_release; }
inline int Song::getCount() const{ return this->count_playsong; }
inline std::string Song::getFilePath() const{ return this->file_path; }

inline void Song::setIdSong(const std::string& id_song){ this->id_song = id_song; }
inline void Song::setName(const std::string& name){ this->name = name; }
inline void Song::setArtist(const std::string& artist){ this->artist = artist; }
inline void Song::setAlbum(const std::string& album){ this->album = album; }
inline void Song::setTypeSong(const std::string& type){ this->type = type; }
inline void Song::setDuration(int duration){ this->duration = duration; }
inline void Song::setDateRelease(const std::string& date_release){ this->date_release = date_release; }
inline void Song::setCount(int count_playsong){ this->count_playsong = count_playsong; }
inline void Song::setFilePath(const std::string& file_path){ this->file_path = file_path; }

inline void Song::increasePlayCount(){ this->count_playsong++; }
inline void Song::display() const{
    std::cout << "[" << this->id_song << "] " << this->name << " - " << this->artist 
              << " (" << this->duration / 60 << ":" << (this->duration % 60 < 10 ? "0" : "") << this->duration % 60 << ")"
              << " | Plays: " << this->count_playsong << "\n";
}
inline bool Song::operator==(const Song& other) const{
    return this->id_song == other.id_song;
}