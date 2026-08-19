#include <string>

class Song{
    private:
       std::string id_song;
       std::string name;
       std::string artist;
       std::string album;
       std::string type;
       int duration;
       std::string date_release;
       int count_playsong;
    public:
        Song();
        Song(std::string id_song, std::string name, std::string artist, 
            std::string album, std::string type, int duration, 
            std::string date_release, int count_playsong);
        std::string getIdsong() const;
        std::string getName() const;
        std::string getArtist() const;
        std::string getAlbum() const;
        std::string getTypesong() const;
        int getDuration() const;
        std::string getDateRelease() const;
        int getCount() const;
        void setIdsong(std::string id_song);
        void setName(std::string name);
        void setArtist(std::string artist);
        void setAlbum(std::string album);
        void setTypesong(std::string type);
        void setDuration(int duration);
        void setDateRelease(std::string date_release);
        void setCount(int count_playsong);
        void increasePlayCount();
        void resetPlayCount();
        void display() const;
};