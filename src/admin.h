#pragma once
#include <string>
#include <iostream>
#include "user.h"

class Admin : public User{
    public:
        Admin();
        Admin(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password);
        Admin(const Admin& other);
        ~Admin() override;
        Role getRole() const override;
        bool isAdmin() const override;
        bool canManageCatalog() const override;
        bool canManageUsers() const override;
        User* clone() const override;
        void display() const override;
};
Admin::Admin() : User(){}
Admin::Admin(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password)
    : User(id_user, username, email, password){}
Admin::Admin(const Admin& other) : User(other){}
Admin::~Admin(){}
Role Admin::getRole() const{ 
    return Role::ADMIN; 
}
bool Admin::isAdmin() const{ 
    return true; 
}
bool Admin::canManageCatalog() const{ 
    return true; 
}
bool Admin::canManageUsers() const{ 
    return true; 
}
User* Admin::clone() const{ 
    return new Admin(*this); 
}
void Admin::display() const{
    std::cout << "[Admin] ";
    this->User::display();
    std::cout << std::endl;
}