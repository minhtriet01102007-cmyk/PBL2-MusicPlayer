#pragma once
#include <string>
#include <iostream>

enum class Role{ 
    ADMIN, 
    LISTENER 
};
class User{
    protected:
        std::string id_user;
        std::string username;
        std::string email;
        std::string password;
        bool active;              
    public:
        User();
        User(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password);
        User(const User& other);   
        virtual ~User();

        std::string getIdUser() const;
        std::string getUsername() const;
        std::string getEmail() const;
        std::string getPassword() const; 
        bool isActive() const;

        void setIdUser(const std::string& id_user);
        void setUsername(const std::string& username);
        void setEmail(const std::string& email);
        void setPassword(const std::string& password);
        void setActive(bool active);

        bool checkPassword(const std::string& input) const;      
        static std::string roleToString(Role role);
        std::string getRoleName() const;
     
        virtual Role getRole() const = 0;
        virtual bool isAdmin() const = 0;
        virtual bool canManageCatalog() const = 0; // thêm/sửa/xóa bài hát, nghệ sĩ, album
        virtual bool canManageUsers() const = 0;   // khóa/mở khóa, xem danh sách người dùng
        virtual User* clone() const = 0;           // sao chép đa hình 
        virtual void display() const;              
};
User::User(){
    this->id_user = "";
    this->username = "";
    this->email = "";
    this->password = "";
    this->active = true;
}
User::User(const std::string& id_user, const std::string& username, const std::string& email, const std::string& password){
    this->id_user = id_user;
    this->username = username;
    this->email = email;
    this->password = password;
    this->active = true;
}
User::User(const User& other){
    this->id_user = other.id_user;
    this->username = other.username;
    this->email = other.email;
    this->password = other.password;
    this->active = other.active;
}
User::~User(){}
std::string User::getIdUser() const{ 
    return this->id_user; 
}
std::string User::getUsername() const{ 
    return this->username; 
}
std::string User::getEmail() const{ 
    return this->email; 
}
std::string User::getPassword() const{ 
    return this->password; 
}
bool User::isActive() const{ 
    return this->active; 
}
void User::setIdUser(const std::string& id_user){ 
    this->id_user = id_user; 
}
void User::setUsername(const std::string& username){ 
    this->username = username; 
}
void User::setEmail(const std::string& email){ 
    this->email = email; 
}
void User::setPassword(const std::string& password){ 
    this->password = password; 
}
void User::setActive(bool active){ 
    this->active = active; 
}
bool User::checkPassword(const std::string& input) const{
    return this->password == input;
}
std::string User::roleToString(Role role){
    return (role == Role::ADMIN) ? "ADMIN" : "LISTENER";
}
std::string User::getRoleName() const{
    return User::roleToString(this->getRole());
}
void User::display() const{
    std::cout << "ID: " << this->id_user << " | Username: " << this->username
              << " | " << (this->active ? "Active" : "Locked");
}