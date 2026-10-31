#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/playlist.h"
#include "src/listener.h"
#include "dsa/dlinkedlist/dlistutil.h"
#include "userrepository.h"
#include "songrepository.h"
#include "textutil.h"

class IPlaylistRepository{
    public:
        virtual ~IPlaylistRepository(){}
        virtual void load() = 0;                                    // nạp playlist và gắn vào đúng Listener
        virtual void save() = 0;                                    // ghi toàn bộ playlist của mọi Listener
        virtual std::string generateId() const = 0;
};

class TextPlaylistRepository : public IPlaylistRepository{
    private:
        std::string filePath;
        IUserRepository& users;
        ISongRepository& songs;
    public:
        TextPlaylistRepository(const std::string& root, IUserRepository& users, ISongRepository& songs);
        void load() override;
        void save() override;
        std::string generateId() const override;
};

TextPlaylistRepository::TextPlaylistRepository(const std::string& root, IUserRepository& users, ISongRepository& songs)
    : users(users), songs(songs){
    this->filePath = root + "/data/playlists.txt";
}
void TextPlaylistRepository::load(){
    using namespace textutil;
    for (const auto& l : readLines(this->filePath)){
        char d = detectDelim(l);
        auto p = split(l, d);
        if (p.size() < 3) continue;
        Listener* ls = dynamic_cast<Listener*>(this->users.findById(p[2]));
        if (ls == nullptr) continue;
        ls->addPlaylist(Playlist(p[0], p[1], p[2]));        // thêm playlist rỗng rồi mới thêm bài (tránh sao chép playlist đầy)
        Playlist* pl = ls->getPlaylistById(p[0]);
        if (p.size() > 3 && pl != nullptr)
            for (const auto& sid : split(p[3], d == ';' ? ',' : ';'))
                if (Song* s = this->songs.findById(sid)) pl->addSong(s);
    }
}
void TextPlaylistRepository::save(){
    std::ofstream out(this->filePath);
    for (User* u : this->users.getAll()){
        Listener* ls = dynamic_cast<Listener*>(u);
        if (ls == nullptr) continue;
        std::vector<std::string> ids;
        for (const auto& pl : ls->getPlaylists()) ids.push_back(pl.getIdPlaylist());
        for (const auto& id : ids){
            Playlist* pl = ls->getPlaylistById(id);
            out << pl->getIdPlaylist() << '|' << pl->getNamePlaylist() << '|' << pl->getIdUser() << '|';
            bool first = true;
            for (Song* s : dlist::toVector(pl->getQueue())){
                out << (first ? "" : ";") << s->getIdSong();
                first = false;
            }
            out << '\n';
        }
    }
}
std::string TextPlaylistRepository::generateId() const{
    std::vector<std::string> ids;
    for (User* u : this->users.getAll())
        if (Listener* ls = dynamic_cast<Listener*>(u))
            for (const auto& pl : ls->getPlaylists()) ids.push_back(pl.getIdPlaylist());
    return textutil::nextId(ids, "PL");
}
