#include <string>

class Artist{
    private:
        std::string id_artist;
        std::string name_artist;
        std::string bio;
        std::string country;
        long long followers;
    public:
        Artist();
        Artist(std::string id_artist, std::string name_artist, std::string bio,
               std::string country, long long followers);
        std::string getIdArtist() const;
        std::string getNameArtist() const;
        std::string getBio() const;
        std::string getCountry() const;
        long long getFollowers() const;
        void setIdArtist(std::string id_artist);
        void setNameArtist(std::string name_artist);
        void setBio(std::string bio);
        void setCountry(std::string country);
        void setFollowers(long long followers);
        void show() const;
};