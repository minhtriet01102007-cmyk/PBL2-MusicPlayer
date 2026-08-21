#include <string>

class User{
    protected:
        std::string id_user;
        std::string name_user;
        std::string password;
        std::string email;
        std::string phone_number;
        std::string display_name;
        std::string avatar;
        bool is_active;
    public:
        User();
        User(std::string id_user, std::string name_user, std::string password,
         std::string email, std::string phone_number, std::string display_name,
         std::string avatar, bool is_active);
        virtual ~User();
        std::string getIdUser() const;
        std::string getNameuser() const;
        std::string getEmail() const;
        std::string getPhoneNumber() const;
        std::string getDisplayName() const;
        std::string getAvatar() const;
        bool getIsActive() const;
        void setIdname(std::string id_user);    
        void setUsername(std::string name_user);
        void setPassword(std::string password);
        void setEmail(std::string email);
        void setPhoneNumber(std::string phone_number);
        void setDisplayName(std::string display_name);
        void setAvatar(std::string avatar);
        void setIsActive(bool is_active);
        bool checkPassword(std::string password) const;
        virtual std::string getRole() const = 0;
        virtual void display() const = 0;
};