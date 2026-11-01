#pragma once
#include <string>
#include <iostream>
#include "user.h"

class Admin : public User{
    private:
        std::string role;
    public:
        Admin();
        Admin(const std::string& id_user, const std::string& username, const std::string& email, 
              const std::string& password, const std::string& role);
        Admin(const Admin& other);
        ~Admin() override;
        std::string getRole() const override;
        bool isAdmin() const override;
        void setRole(const std::string& role);
        void display() const override;
};
Admin::Admin() : User(){
    this->role = "ADMIN";
}
Admin::Admin(const std::string& id_user, const std::string& username, const std::string& email, 
             const std::string& password, const std::string& role) : User(id_user, username, email, password){
    this->role = role;
}
Admin::Admin(const Admin& other) : User(other){
    this->role = other.role;
}
Admin::~Admin(){}
std::string Admin::getRole() const{
    return this->role;
}
bool Admin::isAdmin() const{
    return true;
}
void Admin::setRole(const std::string& role){
    this->role = role;
}
void Admin::display() const{
    std::cout << "[Admin] ";
    this->User::display();
    std::cout << " | Role: " << this->role << std::endl;
}