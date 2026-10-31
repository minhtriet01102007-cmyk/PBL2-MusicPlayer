#pragma once
#include <string>
#include "src/user.h"
#include "src/listener.h"
#include "dal/userrepository.h"

// Nghiệp vụ đăng nhập / phiên làm việc / phân quyền
class AuthService{
    private:
        IUserRepository& users;
        User* current = nullptr;
    public:
        explicit AuthService(IUserRepository& users);
        bool login(const std::string& username, const std::string& password);
        void logout();
        bool isLoggedIn() const;
        bool isAdmin() const;
        std::string currentUsername() const;
        Listener* currentListener() const;      // nullptr nếu chưa đăng nhập hoặc là Admin
};

AuthService::AuthService(IUserRepository& users) : users(users){
}
bool AuthService::login(const std::string& username, const std::string& password){
    User* u = this->users.findByUsername(username);
    if (u == nullptr || !u->checkPassword(password)) return false;
    this->current = u;
    return true;
}
void AuthService::logout(){
    this->current = nullptr;
}
bool AuthService::isLoggedIn() const{
    return this->current != nullptr;
}
bool AuthService::isAdmin() const{
    return this->current != nullptr && this->current->isAdmin();
}
std::string AuthService::currentUsername() const{
    return this->current != nullptr ? this->current->getUsername() : "";
}
Listener* AuthService::currentListener() const{
    return dynamic_cast<Listener*>(this->current);
}
