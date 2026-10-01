#pragma once
#include <string>

class User{
    protected:
        std::string id_user;
        std::string username;
        std::string email;
        std::string phone_number;
        std::string password;
    public:
        User();
        User(std::string id_user, std::string username, std::string email, std::string phone_number, std::string password);
        User(const User& user);
        virtual ~User();
        std::string getIdUser() const;
        std::string getUsername() const;
        std::string getEmail() const;
        std::string getPhonenumber() const;
        std::string getPassword() const;
        void setIdUser(std::string id_user);
        void setUsername(std::string username);
        void setEmail(std::string email);
        void setPhonenumber(std::string phone_number);
        void setPassword(std::string password);
        virtual void display() const;
};