#pragma once
#include <string>
using namespace std;

class User{
    protected:
        string id_user;
        string name_user;
        string password;
        string email;
        string phone_number;
    public:
        User(string id_user, string name_user, string password, string email, string phone_number);
        User(const User& u);
        virtual ~User();
        virtual bool checkPassword(string password) const;
        virtual bool checkEmail(string email) const;
        virtual bool checkPhone(string phone_number) const;
        virtual void show() const;
};