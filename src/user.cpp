#include "../include/user.h"
#include <iostream>

User::User(){
    this->id_user = "";
    this->username = "";
    this->email = "";
    this->phone_number = "";
    this->password = "";
}

User::User(std::string id_user,
           std::string username,
           std::string email,
           std::string phone_number,
           std::string password){
    this->id_user = id_user;
    this->username = username;
    this->email = email;
    this->phone_number = phone_number;
    this->password = password;
}

User::User(const User& user){
    this->id_user = user.id_user;
    this->username = user.username;
    this->email = user.email;
    this->phone_number = user.phone_number;
    this->password = user.password;
}

User::~User(){
}

std::string User::getIdUser() const{
    return this->id_user;
}

std::string User::getUsername() const{
    return this->username;
}

std::string User::getEmail() const{
    return this->email;
}

std::string User::getPhonenumber() const{
    return this->phone_number;
}

std::string User::getPassword() const{
    return this->password;
}

void User::setIdUser(std::string id_user){
    this->id_user = id_user;
}

void User::setUsername(std::string username){
    this->username = username;
}

void User::setEmail(std::string email){
    this->email = email;
}

void User::setPhonenumber(std::string phone_number){
    this->phone_number = phone_number;
}

void User::setPassword(std::string password){
    this->password = password;
}

void User::display() const{
    std::cout << "User ID: " << this->id_user << '\n';
    std::cout << "Username: " << this->username << '\n';
    std::cout << "Email: " << this->email << '\n';
    std::cout << "Phone number: " << this->phone_number << '\n';
}