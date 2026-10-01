#pragma once

#include "user.h"
#include <string>

class Admin : public User{
private:
    std::string role;

public:
    Admin();

    Admin(std::string id_user,
          std::string username,
          std::string email,
          std::string phone_number,
          std::string password,
          std::string role);

    Admin(const Admin& admin);

    ~Admin() override;

    std::string getRole() const;
    void setRole(std::string role);

    void display() const override;
};