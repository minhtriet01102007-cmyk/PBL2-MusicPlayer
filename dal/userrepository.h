#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/user.h"
#include "src/admin.h"
#include "src/listener.h"
#include "dsa/hashtable/hashtable.h"
#include "textutil.h"

class IUserRepository{
    public:
        virtual ~IUserRepository(){}
        virtual void load() = 0;
        virtual void save() = 0;                                    // ghi lại thông tin thay đổi của Listener
        virtual const std::vector<User*>& getAll() const = 0;
        virtual User* findByUsername(const std::string& username) const = 0;
        virtual User* findById(const std::string& id) const = 0;
};

class TextUserRepository : public IUserRepository{
    private:
        std::string adminPath, listenerPath;
        std::vector<User*> users;
        HashTable<std::string, User*> byName;   // đăng nhập: username -> User*
        HashTable<std::string, User*> byId;     // id_user -> User*
        void add(User* u);
    public:
        explicit TextUserRepository(const std::string& root);
        ~TextUserRepository() override;
        TextUserRepository(const TextUserRepository&) = delete;
        TextUserRepository& operator=(const TextUserRepository&) = delete;
        void load() override;
        void save() override;
        const std::vector<User*>& getAll() const override;
        User* findByUsername(const std::string& username) const override;
        User* findById(const std::string& id) const override;
};

TextUserRepository::TextUserRepository(const std::string& root){
    this->adminPath = root + "/data/admin.txt";
    this->listenerPath = root + "/data/listener.txt";
}
TextUserRepository::~TextUserRepository(){
    for (User* u : this->users) delete u;
}
void TextUserRepository::add(User* u){
    User* existed = nullptr;
    if (this->byName.search(u->getUsername(), existed) || this->byId.search(u->getIdUser(), existed)){
        delete u;                       // trùng username hoặc id: bỏ qua tài khoản đến sau
        return;
    }
    this->users.push_back(u);
    this->byName.insert(u->getUsername(), u);
    this->byId.insert(u->getIdUser(), u);
}
void TextUserRepository::load(){
    using namespace textutil;
    for (const auto& l : readLines(this->adminPath)){
        auto p = split(l, detectDelim(l));
        if (p.size() < 4) continue;
        this->add(new Admin(p[0], p[1], p[2], p[3], p.size() > 4 ? p[4] : "ADMIN"));
    }
    for (const auto& l : readLines(this->listenerPath)){
        auto p = split(l, detectDelim(l));
        if (p.size() < 4) continue;
        Listener* ls = new Listener(p[0], p[1], p[2], p[3]);
        if (p.size() > 4) ls->setTotalSongsPlayed(toInt(p[4]));
        this->add(ls);
    }
}
void TextUserRepository::save(){
    // Cập nhật tại chỗ cột total_songs_played của từng Listener, giữ nguyên các cột còn lại (kể cả mật khẩu)
    auto lines = textutil::readLines(this->listenerPath);
    if (lines.empty()) return;
    std::ofstream out(this->listenerPath);
    for (const auto& l : lines){
        char d = textutil::detectDelim(l);
        auto p = textutil::split(l, d);
        User* u = nullptr;
        if (p.size() >= 4 && this->byId.search(p[0], u)){
            Listener* ls = dynamic_cast<Listener*>(u);
            if (ls != nullptr){
                std::string total = std::to_string(ls->getTotalSongsPlayed());
                if (p.size() > 4) p[4] = total;
                else p.push_back(total);
            }
        }
        for (size_t i = 0; i < p.size(); ++i){
            if (i > 0) out << d;
            out << p[i];
        }
        out << '\n';
    }
}
const std::vector<User*>& TextUserRepository::getAll() const{
    return this->users;
}
User* TextUserRepository::findByUsername(const std::string& username) const{
    User* u = nullptr;
    return this->byName.search(username, u) ? u : nullptr;
}
User* TextUserRepository::findById(const std::string& id) const{
    User* u = nullptr;
    return this->byId.search(id, u) ? u : nullptr;
}
