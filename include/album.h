#pragma once
#include <string>

class Album{
    private:
        std::string id_album;
        std::string name_album;
        std::string artist;
        std::string date_release;
        std::string type;
    public:
        Album();
        Album(std::string id_album, std::string name_album, std::string artist,
              std::string date_release, std::string type);
        std::string getIdAlbum() const;
        std::string getNameAlbum() const;
        std::string getArtist() const;
        std::string getDateRelease() const;
        std::string getType() const;
        void setIdAlbum(std::string id_album);
        void setNameAlbum(std::string name_album);
        void setArtist(std::string artist);
        void setDateRelease(std::string date_release);
        void setType(std::string type);
        void display() const;
};
