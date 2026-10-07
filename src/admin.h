#pragma once
#include <string>
#include <iostream>
#include "user.h"

class Admin : public User{
    private:
        std::string role;
    public:
        Admin();
        Admin(std::string id_user, std::string username, std::string email, std::string password, std::string role = "SUPER_ADMIN");
        Admin(const Admin& other);
        ~Admin() override;
        std::string getRole() const;
        void setRole(const std::string& role);
        void display() const override;
};
inline Admin::Admin() : User(){
    this->role = "ADMIN";
}
inline Admin::Admin(std::string id_user, std::string username, std::string email, std::string password, std::string role)
    : User(id_user, username, email, password){
    this->role = role;
}
inline Admin::Admin(const Admin& other) : User(other){
    this->role = other.role;
}
inline Admin::~Admin(){}
inline std::string Admin::getRole() const{
    return this->role;
}
inline void Admin::setRole(const std::string& role){
    this->role = role;
}
inline void Admin::display() const{
    std::cout << "[Admin] ";
    this->User::display();
    std::cout << " | Role: " << this->role << "\n";
}