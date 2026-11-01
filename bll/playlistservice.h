#pragma once
#include <functional>
#include <string>
#include <vector>
#include "songinfo.h"
#include "playlistinfo.h"
#include "mapper.h"
#include "authservice.h"
#include "src/playlist.h"
#include "src/listener.h"
#include "dal/playlistrepository.h"
#include "dal/userrepository.h"
#include "dal/songrepository.h"
#include "dal/textutil.h"
#include "dsa/dlinkedlist/dlistutil.h"

// Nghiệp vụ playlist của Listener
class PlaylistService{
    private:
        IPlaylistRepository& repo;
        IUserRepository& users;
        ISongRepository& songs;
        AuthService& auth;
        std::function<void(const std::string&)> afterRemove;    // báo cho tầng khác khi playlist bị xóa
        Playlist* find(const std::string& id) const;
    public:
        PlaylistService(IPlaylistRepository& repo, IUserRepository& users, ISongRepository& songs, AuthService& auth);
        void setAfterRemoveHook(const std::function<void(const std::string&)>& hook);
        std::vector<PlaylistInfo> myPlaylists() const;
        std::vector<SongInfo> songsOf(const std::string& idPlaylist);
        std::string create(const std::string& name);            // trả về id, "" nếu thất bại
        bool remove(const std::string& idPlaylist);
        bool addSong(const std::string& idPlaylist, const std::string& idSong);
        bool removeSong(const std::string& idPlaylist, const std::string& idSong);
        void purgeSong(Song* s);                                // gỡ bài khỏi mọi playlist của mọi Listener
};

PlaylistService::PlaylistService(IPlaylistRepository& repo, IUserRepository& users, ISongRepository& songs, AuthService& auth)
    : repo(repo), users(users), songs(songs), auth(auth){
}
void PlaylistService::setAfterRemoveHook(const std::function<void(const std::string&)>& hook){
    this->afterRemove = hook;
}
Playlist* PlaylistService::find(const std::string& id) const{
    Listener* l = this->auth.currentListener();
    return l != nullptr ? l->getPlaylistById(id) : nullptr;
}
std::vector<PlaylistInfo> PlaylistService::myPlaylists() const{
    std::vector<PlaylistInfo> v;
    Listener* l = this->auth.currentListener();
    if (l == nullptr) return v;
    for (const auto& pl : l->getPlaylists()){
        PlaylistInfo i;
        i.id = pl.getIdPlaylist();
        i.name = pl.getNamePlaylist();
        i.songCount = pl.getSongCount();
        v.push_back(i);
    }
    return v;
}
std::vector<SongInfo> PlaylistService::songsOf(const std::string& idPlaylist){
    std::vector<SongInfo> v;
    Playlist* pl = this->find(idPlaylist);
    if (pl == nullptr) return v;
    for (Song* s : dlist::toVector(pl->getQueue())) v.push_back(toSongInfo(*s, this->songs));
    return v;
}
std::string PlaylistService::create(const std::string& name){
    Listener* l = this->auth.currentListener();             // quy tắc nghiệp vụ: chỉ Listener có playlist
    if (l == nullptr || textutil::trim(name).empty()) return "";
    std::string id = this->repo.generateId();
    l->addPlaylist(Playlist(id, textutil::trim(name), l->getIdUser()));
    this->repo.save();
    return id;
}
bool PlaylistService::remove(const std::string& idPlaylist){
    Listener* l = this->auth.currentListener();
    if (l == nullptr) return false;
    if (this->afterRemove) this->afterRemove(idPlaylist);   // báo trước để tầng phát nhạc không giữ playlist đã xóa
    bool ok = l->removePlaylist(idPlaylist);
    if (ok) this->repo.save();
    return ok;
}
bool PlaylistService::addSong(const std::string& idPlaylist, const std::string& idSong){
    Playlist* pl = this->find(idPlaylist);
    Song* s = this->songs.findById(idSong);
    if (pl == nullptr || s == nullptr) return false;
    for (Song* x : dlist::toVector(pl->getQueue())) if (x == s) return false;   // bài đã có trong playlist
    pl->addSong(s);
    this->repo.save();
    return true;
}
bool PlaylistService::removeSong(const std::string& idPlaylist, const std::string& idSong){
    Playlist* pl = this->find(idPlaylist);
    Song* s = this->songs.findById(idSong);
    if (pl == nullptr || s == nullptr) return false;
    bool ok = pl->removeSong(s);
    if (ok) this->repo.save();
    return ok;
}
void PlaylistService::purgeSong(Song* s){
    for (User* u : this->users.getAll()){
        Listener* l = dynamic_cast<Listener*>(u);
        if (l == nullptr) continue;
        std::vector<std::string> ids;
        for (const auto& pl : l->getPlaylists()) ids.push_back(pl.getIdPlaylist());
        for (const auto& id : ids) l->getPlaylistById(id)->removeSong(s);
    }
    this->repo.save();
}
