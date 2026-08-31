#include "../include/user.h"
#include <string>

User::User()
    : id_user(""), name_user(""), password(""), email(""), phone_number(""),
      display_name(""), avatar(""), is_active(true)
{}
User::User(std::string id_user, std::string name_user, std::string password,
           std::string email, std::string phone_number, std::string display_name,
           std::string avatar, bool is_active)
    : id_user(id_user), name_user(name_user), password(password), email(email),
      phone_number(phone_number), display_name(display_name), avatar(avatar), is_active(is_active)
{}
User::~User()
{}
std::string User::getIdUser() const{
    return id_user;
}
std::string User::getNameuser() const{
    return name_user;
}
std::string User::getEmail() const{
    return email;
}
std::string User::getPhoneNumber() const{
    return phone_number;
}
std::string User::getDisplayName() const{
    return display_name;
}
std::string User::getAvatar() const{
    return avatar;
}
bool User::getIsActive() const{
    return is_active;
}
void User::setIdname(std::string id_user){
    this->id_user = id_user;
}
void User::setUsername(std::string name_user){
    this->name_user = name_user;
}
void User::setPassword(std::string password){
    this->password = password;
}
void User::setEmail(std::string email){
    this->email = email;
}
void User::setPhoneNumber(std::string phone_number){
    this->phone_number = phone_number;
}
void User::setDisplayName(std::string display_name){
    this->display_name = display_name;
}
void User::setAvatar(std::string avatar){
    this->avatar = avatar;
}
void User::setIsActive(bool is_active){
    this->is_active = is_active;
}
bool User::checkPassword(std::string password) const{
    return this->password == password;
}