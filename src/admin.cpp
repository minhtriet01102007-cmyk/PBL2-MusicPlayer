#include "../include/admin.h"
#include <iostream>

Admin::Admin(){
    this->role = "Admin";
}

Admin::Admin(std::string id_user,
             std::string username,
             std::string email,
             std::string phone_number,
             std::string password,
             std::string role)
    : User(id_user, username, email, phone_number, password){
    this->role = role;
}

Admin::Admin(const Admin& admin)
    : User(admin){
    this->role = admin.role;
}

Admin::~Admin(){
}

std::string Admin::getRole() const{
    return this->role;
}

void Admin::setRole(std::string role){
    this->role = role;
}

void Admin::display() const{
    std::cout << "User ID: " << this->id_user << '\n';
    std::cout << "Username: " << this->username << '\n';
    std::cout << "Email: " << this->email << '\n';
    std::cout << "Phone number: " << this->phone_number << '\n';
    std::cout << "Role: " << this->role << '\n';
}