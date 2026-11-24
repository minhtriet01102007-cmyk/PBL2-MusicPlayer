#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/user.h"
#include "src/admin.h"
#include "src/listener.h"
#include "textutil.h"

class UserDAO{
    private:
        std::string adminPath;
        std::string listenerPath;

    public:
        explicit UserDAO(const std::string& root);
        std::vector<User*> loadAll() const;
        bool saveAll(const std::vector<User*>& users) const;
};
UserDAO::UserDAO(const std::string& root){
    adminPath = root + "/data/admin.txt";
    listenerPath = root + "/data/listener.txt";
}

// Đọc dữ liệu người dùng
std::vector<User*> UserDAO::loadAll() const{
    std::vector<User*> users;

    // Đọc Admin
    for (const std::string& line : textutil::readLines(adminPath)){
        std::vector<std::string> p = textutil::split(line, textutil::detectDelim(line));
        if (p.size() < 4) continue;

        users.push_back(new Admin(p[0], p[1], p[2], p[3]));
    }

    // Đọc Listener
    for (const std::string& line : textutil::readLines(listenerPath)){
        std::vector<std::string> p = textutil::split(line, textutil::detectDelim(line));
        if (p.size() < 4) continue;

        Listener* l = new Listener(p[0], p[1], p[2], p[3]);

        if (p.size() > 4) l->setTotalSongsPlayed(textutil::toInt(p[4]));
        if (p.size() > 5) l->setActive(p[5] != "0");

        if (p.size() > 6){
            for (const std::string& id : textutil::split(p[6], textutil::listDelim(textutil::detectDelim(line))))
                l->likeSong(id);
        }

        if (p.size() > 7){
            for (const std::string& id : textutil::split(p[7], textutil::listDelim(textutil::detectDelim(line))))
                l->followArtist(id);
        }

        if (p.size() > 8){
            for (const std::string& id : textutil::split(p[8], textutil::listDelim(textutil::detectDelim(line))))
                l->addRecentSong(id);
        }

        users.push_back(l);
    }

    return users;
}

// Lưu dữ liệu người dùng
bool UserDAO::saveAll(const std::vector<User*>& users) const{
    std::ofstream adminFile(adminPath);
    std::ofstream listenerFile(listenerPath);

    if (!adminFile || !listenerFile) return false;

    for (User* u : users){
        if (u == nullptr) continue;

        if (u->isAdmin()){
            adminFile << u->getIdUser() << " | "
                      << u->getUsername() << " | "
                      << u->getEmail() << " | "
                      << u->getPassword() << '\n';
        }
        else{
            Listener* l = static_cast<Listener*>(u);

            listenerFile << l->getIdUser() << " | "
                         << l->getUsername() << " | "
                         << l->getEmail() << " | "
                         << l->getPassword() << " | "
                         << l->getTotalSongsPlayed() << " | "
                         << l->isActive() << " | ";

            const std::vector<std::string>& liked = l->getLikedSongIds();
            const std::vector<std::string>& followed = l->getFollowedArtistIds();
            const std::vector<std::string>& recent = l->getRecentSongIds();

            for (size_t i = 0; i < liked.size(); i++)
                listenerFile << (i > 0 ? ";" : "") << liked[i];

            listenerFile << " | ";

            for (size_t i = 0; i < followed.size(); i++)
                listenerFile << (i > 0 ? ";" : "") << followed[i];

            listenerFile << " | ";

            for (size_t i = 0; i < recent.size(); i++)
                listenerFile << (i > 0 ? ";" : "") << recent[i];

            listenerFile << '\n';
        }
    }

    return adminFile.good() && listenerFile.good();
}
