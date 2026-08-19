#include <string>

class Artist{
    private:
        std::string id_artist;
        std::string name_artist;
        std::string country;
        std::string type;
    public:
        Artist();
        Artist(std::string id_artist, std::string name_artist, 
               std::string country, std::string type);
        std::string getIdArtist() const;
        std::string getNameArtist() const;
        std::string getCountry() const;
        std::string getTypePlay() const;
        void setIdArtist(std::string id_artist);
        void setNameArtist(std::string name_artist);
        void setCountry(std::string country);
        void setTypePlay(std::string type);
        void show() const;
};