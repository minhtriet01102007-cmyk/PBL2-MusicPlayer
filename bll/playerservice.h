#pragma once
#include <string>
#include <vector>
#include "songinfo.h"
#include "mapper.h"
#include "authservice.h"
#include "src/song.h"
#include "src/playlist.h"
#include "src/listener.h"
#include "dal/songrepository.h"
#include "dal/userrepository.h"
#include "dsa/dlinkedlist/dlinkedlist.h"
#include "dsa/dlinkedlist/dlistutil.h"

class PlayerService{
    private:
        enum Mode{
            None,
            QueueMode,
            PlaylistMode
        };
        ISongRepository& songs;
        IUserRepository& users;
        AuthService& auth;
        DoublyLinkedList<Song*> queue;          // hàng phát khi phát từ thư viện / top
        Mode mode = None;
        std::string activePl;                   // id playlist đang phát (khi mode == PlaylistMode)
        DoublyLinkedList<Song*>* activeList();
    public:
        PlayerService(ISongRepository& songs, IUserRepository& users, AuthService& auth);
        void reset();
        bool startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out);
        bool startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out);
        bool next(SongInfo& out);
        bool prev(SongInfo& out);
        void recordPlay(const std::string& idSong);
        void purgeSong(Song* s);                // gỡ bài khỏi hàng phát khi bài bị xóa
        void onPlaylistRemoved(const std::string& idPlaylist);
};

PlayerService::PlayerService(ISongRepository& songs, IUserRepository& users, AuthService& auth)
    : songs(songs), users(users), auth(auth){
}
DoublyLinkedList<Song*>* PlayerService::activeList(){
    if (this->mode == QueueMode) return &this->queue;
    if (this->mode == PlaylistMode){
        Listener* l = this->auth.currentListener();
        Playlist* p = l != nullptr ? l->getPlaylistById(this->activePl) : nullptr;
        return p != nullptr ? &p->getQueue() : nullptr;
    }
    return nullptr;
}
void PlayerService::reset(){
    this->mode = None;
    this->activePl = "";
    this->queue.clear();
}
bool PlayerService::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    this->queue.clear();
    for (const auto& id : ids) if (Song* s = this->songs.findById(id)) this->queue.pushBack(s);
    Song* st = this->songs.findById(startId);
    if (st == nullptr || !dlist::seekTo(this->queue, st)) return false;
    this->mode = QueueMode;
    out = toSongInfo(*st, this->songs);
    return true;
}
bool PlayerService::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    Listener* ls = this->auth.currentListener();
    Playlist* pl = ls != nullptr ? ls->getPlaylistById(idPlaylist) : nullptr;
    if (pl == nullptr || pl->getQueue().isEmpty()) return false;
    DoublyLinkedList<Song*>& l = pl->getQueue();
    Song* st = startId.empty() ? nullptr : this->songs.findById(startId);
    if (st == nullptr){
        l.resetPlayback();
        st = l.getCurrent();
    }
    else if (!dlist::seekTo(l, st)) return false;
    this->mode = PlaylistMode;
    this->activePl = idPlaylist;
    out = toSongInfo(*st, this->songs);
    return true;
}
bool PlayerService::next(SongInfo& out){
    DoublyLinkedList<Song*>* l = this->activeList();
    if (l == nullptr || !l->hasNext()) return false;
    out = toSongInfo(*l->next(), this->songs);
    return true;
}
bool PlayerService::prev(SongInfo& out){
    DoublyLinkedList<Song*>* l = this->activeList();
    if (l == nullptr || !l->hasPrev()) return false;
    out = toSongInfo(*l->prev(), this->songs);
    return true;
}
void PlayerService::recordPlay(const std::string& idSong){
    Song* s = this->songs.findById(idSong);
    if (s == nullptr) return;
    s->increasePlayCount();
    s->setPlays(s->getPlays() + 1);
    this->songs.save();
    Listener* l = this->auth.currentListener();
    if (l != nullptr){
        l->increasePlayedCount();
        this->users.save();
    }
}
void PlayerService::purgeSong(Song* s){
    this->queue.remove(s);
}
void PlayerService::onPlaylistRemoved(const std::string& idPlaylist){
    if (this->mode == PlaylistMode && this->activePl == idPlaylist) this->mode = None;
}
