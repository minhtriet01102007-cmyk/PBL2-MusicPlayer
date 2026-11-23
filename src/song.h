#pragma once
#include <string>
#include <iostream>

// Liên kết với Artist và Album bằng ID (association)
class Song{
    private:
        std::string id_song;
        std::string name;
        std::string id_artist;  
        std::string id_album;    
        std::string type;        
        int duration;            // giây
        std::string date_release; 
        std::string file_path;   
        long long plays;         
    public:
        Song();
        Song(const std::string& id_song, const std::string& name, const std::string& id_artist, const std::string& id_album,
             const std::string& type, int duration, const std::string& date_release, const std::string& file_path, long long plays = 0);

        std::string getIdSong() const;
        std::string getName() const;
        std::string getIdArtist() const;
        std::string getIdAlbum() const;
        std::string getTypeSong() const;
        int getDuration() const;
        std::string getDurationText() const; 
        std::string getDateRelease() const;
        std::string getFilePath() const;
        long long getPlays() const;

        void setIdSong(const std::string& id_song);
        void setName(const std::string& name);
        void setIdArtist(const std::string& id_artist);
        void setIdAlbum(const std::string& id_album);
        void setTypeSong(const std::string& type);
        void setDuration(int duration);
        void setDateRelease(const std::string& date_release);
        void setFilePath(const std::string& file_path);
        void setPlays(long long plays);

        void increasePlayCount();
        void display() const;
        bool operator==(const Song& other) const;
};
// so sánh song nhiều lượt nghe hơn đứng trước, hòa thì xếp theo tên
struct SongPlaysComparator{
    bool operator()(const Song* a, const Song* b) const;
};
Song::Song(){
    this->id_song = "";
    this->name = "";
    this->id_artist = "";
    this->id_album = "";
    this->type = "";
    this->duration = 0;
    this->date_release = "";
    this->file_path = "";
    this->plays = 0;
}
Song::Song(const std::string& id_song, const std::string& name, const std::string& id_artist, const std::string& id_album,
           const std::string& type, int duration, const std::string& date_release, const std::string& file_path, long long plays){
    this->id_song = id_song;
    this->name = name;
    this->id_artist = id_artist;
    this->id_album = id_album;
    this->type = type;
    this->duration = (duration < 0) ? 0 : duration;
    this->date_release = date_release;
    this->file_path = file_path;
    this->plays = plays;
}
std::string Song::getIdSong() const{ 
    return this->id_song; 
}
std::string Song::getName() const{ 
    return this->name; 
}
std::string Song::getIdArtist() const{ 
    return this->id_artist; 
}
std::string Song::getIdAlbum() const{ 
    return this->id_album; 
}
std::string Song::getTypeSong() const{ 
    return this->type; 
}
int Song::getDuration() const{ 
    return this->duration; 
}
std::string Song::getDurationText() const{
    int sec = this->duration % 60;
    return std::to_string(this->duration / 60) + ":" + (sec < 10 ? "0" : "") + std::to_string(sec); 
    // to_string: chuyển đổi một giá trị số sang kiểu chuỗi
}
std::string Song::getDateRelease() const{ 
    return this->date_release; 
}
std::string Song::getFilePath() const{ 
    return this->file_path; 
}
long long Song::getPlays() const{ 
    return this->plays; 
}
void Song::setIdSong(const std::string& id_song){
    this->id_song = id_song; 
}
void Song::setName(const std::string& name){ 
    this->name = name; 
}
void Song::setIdArtist(const std::string& id_artist){ 
    this->id_artist = id_artist; 
}
void Song::setIdAlbum(const std::string& id_album){ 
    this->id_album = id_album; 
}
void Song::setTypeSong(const std::string& type){ 
    this->type = type; 
}
void Song::setDuration(int duration){ 
    this->duration = (duration < 0) ? 0 : duration; 
}
void Song::setDateRelease(const std::string& date_release){ 
    this->date_release = date_release; 
}
void Song::setFilePath(const std::string& file_path){ 
    this->file_path = file_path; 
}
void Song::setPlays(long long plays){ 
    this->plays = (plays < 0) ? 0 : plays; 
}
void Song::increasePlayCount(){ 
    this->plays++; 
}
void Song::display() const{
    std::cout << "[" << this->id_song << "] " << this->name << " (artist: " << this->id_artist << ")"
              << " (" << this->getDurationText() << ") | Plays: " << this->plays << std::endl;
}
bool Song::operator==(const Song& other) const{
    return this->id_song == other.id_song;
}
bool SongPlaysComparator::operator()(const Song* a, const Song* b) const{
    if (a->getPlays() != b->getPlays()) return a->getPlays() > b->getPlays();
    else return a->getName() < b->getName(); // Nếu lượt nghe bằng nhau, so sánh tên bài hát theo thứ tự alphabet
}