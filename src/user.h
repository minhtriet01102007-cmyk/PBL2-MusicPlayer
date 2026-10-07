#pragma once
#include <string>
#include <iostream>

class User{
    protected:
        std::string id_user;
        std::string username;
        std::string email;
        std::string password;
    public:
        User();
        User(std::string id_user, std::string username, std::string email, std::string password);
        virtual ~User();
        std::string getIdUser() const;
        std::string getUsername() const;
        std::string getEmail() const;
        std::string getPassword() const;
        void setIdUser(const std::string& id_user);
        void setUsername(const std::string& username);
        void setEmail(const std::string& email);
        void setPassword(const std::string& password);
        virtual void display() const = 0; 
};
inline User::User(){
    this->id_user = "";
    this->username = "";
    this->email = "";
    this->password = "";
}
inline User::User(std::string id_user, std::string username, std::string email, std::string password){
    this->id_user = id_user;
    this->username = username;
    this->email = email;
    this->password = password;
}
inline User::~User() {}
inline std::string User::getIdUser() const{ return this->id_user; }
inline std::string User::getUsername() const{ return this->username; }
inline std::string User::getEmail() const{ return this->email; }
inline std::string User::getPassword() const{ return this->password; }
inline void User::setIdUser(const std::string& id_user){ this->id_user = id_user; }
inline void User::setUsername(const std::string& username){ this->username = username; }
inline void User::setEmail(const std::string& email){ this->email = email; }
inline void User::setPassword(const std::string& password){ this->password = password; }
inline void User::display() const{
    std::cout << "ID: " << this->id_user << " | Username: " << this->username;
}