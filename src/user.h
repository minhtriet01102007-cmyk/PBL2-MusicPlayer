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
        User(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password);
        virtual ~User();
        std::string getIdUser() const;
        std::string getUsername() const;
        std::string getEmail() const;
        std::string getPassword() const;
        void setIdUser(const std::string& id_user);
        void setUsername(const std::string& username);
        void setEmail(const std::string& email);
        void setPassword(const std::string& password);
        virtual void display() const; 
};

User::User(){
    this->id_user = "";
    this->username = "";
    this->email = "";
    this->password = "";
}

User::User(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password){
    this->id_user = id_user;
    this->username = username;
    this->email = email;
    this->password = password;
}

User::~User() {}

std::string User::getIdUser() const{ return this->id_user; }

std::string User::getUsername() const{ return this->username; }

std::string User::getEmail() const{ return this->email; }

std::string User::getPassword() const{ return this->password; }

void User::setIdUser(const std::string& id_user){ this->id_user = id_user; }

void User::setUsername(const std::string& username){ this->username = username; }

void User::setEmail(const std::string& email){ this->email = email; }

void User::setPassword(const std::string& password){ this->password = password; }

void User::display() const{
    std::cout << "ID: " << this->id_user << " | Username: " << this->username;
}