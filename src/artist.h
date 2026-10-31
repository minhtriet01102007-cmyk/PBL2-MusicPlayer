#pragma once
#include <string>
#include <iostream>

class Artist{
    private:
        std::string id_artist;
        std::string name_artist;
        std::string country;
        long long followers;
    public:
        Artist();
        Artist(const std::string& id_artist, const std::string& name_artist, const std::string& country, long long followers = 0);
        Artist(const Artist& other);
        ~Artist();
        
        std::string getIdArtist() const;
        std::string getNameArtist() const;
        std::string getCountry() const;
        long long getFollowers() const;
        
        void setIdArtist(const std::string& id_artist);
        void setNameArtist(const std::string& name_artist);
        void setCountry(const std::string& country);
        void setFollowers(long long followers);
        
        void increaseFollowers();
        void display() const;
};

Artist::Artist(){
    this->id_artist = "";
    this->name_artist = "";
    this->country = "";
    this->followers = 0;
}
Artist::Artist(const std::string& id_artist, const std::string& name_artist, const std::string& country, long long followers){
    this->id_artist = id_artist;
    this->name_artist = name_artist;
    this->country = country;
    this->followers = followers;
}
Artist::Artist(const Artist& other){
    this->id_artist = other.id_artist;
    this->name_artist = other.name_artist;
    this->country = other.country;
    this->followers = other.followers;
}
Artist::~Artist(){}
std::string Artist::getIdArtist() const{
    return this->id_artist;
}
std::string Artist::getNameArtist() const{
    return this->name_artist;
}
std::string Artist::getCountry() const{
    return this->country;
}
long long Artist::getFollowers() const{
    return this->followers;
}
void Artist::setIdArtist(const std::string& id_artist){
    this->id_artist = id_artist;
}
void Artist::setNameArtist(const std::string& name_artist){
    this->name_artist = name_artist;
}
void Artist::setCountry(const std::string& country){
    this->country = country;
}
void Artist::setFollowers(long long followers){
    this->followers = followers;
}
void Artist::increaseFollowers(){
    this->followers++;
}
void Artist::display() const{
    std::cout << "[Artist] " << this->name_artist << " (" << this->country << ") | Followers: " << this->followers << "\n";
}