#pragma once
#include <string>
#include <vector>
#include "songinfo.h"
#include "playlistinfo.h"
#include "authservice.h"
#include "songservice.h"
#include "playlistservice.h"
#include "playerservice.h"
#include "dal/songrepository.h"
#include "dal/userrepository.h"
#include "dal/playlistrepository.h"

class MusicSystem{
    private:
        TextSongRepository songRepo;
        TextUserRepository userRepo;
        TextPlaylistRepository playlistRepo;
        AuthService auth;
        SongService songSvc;
        PlaylistService playlistSvc;
        PlayerService playerSvc;
    public:
        explicit MusicSystem(const std::string& root);      // root = thư mục gốc dự án (có data/ và assets/)
        MusicSystem(const MusicSystem&) = delete;
        MusicSystem& operator=(const MusicSystem&) = delete;
        void load();

        bool login(const std::string& username, const std::string& password);
        void logout();
        bool isLoggedIn() const;
        bool isAdmin() const;
        std::string currentUsername() const;

        std::vector<SongInfo> allSongs() const;
        std::vector<SongInfo> search(const std::string& keyword) const;
        std::vector<SongInfo> topSongs(int n) const;
        bool addSong(const std::string& name, const std::string& artist, const std::string& album,
                     const std::string& type, int durationSec, const std::string& dateRelease,
                     const std::string& sourceFilePath);
        bool removeSong(const std::string& idSong);

        bool startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out);
        bool startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out);
        bool nextTrack(SongInfo& out);
        bool prevTrack(SongInfo& out);
        void recordPlay(const std::string& idSong);

        std::vector<PlaylistInfo> myPlaylists() const;
        std::vector<SongInfo> playlistSongs(const std::string& idPlaylist);
        std::string createPlaylist(const std::string& name);
        bool deletePlaylist(const std::string& idPlaylist);
        bool addToPlaylist(const std::string& idPlaylist, const std::string& idSong);
        bool removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong);
};

MusicSystem::MusicSystem(const std::string& root)
    : songRepo(root), userRepo(root), playlistRepo(root, userRepo, songRepo),
      auth(userRepo), songSvc(songRepo, auth), playlistSvc(playlistRepo, userRepo, songRepo, auth),
      playerSvc(songRepo, userRepo, auth){
    // Xóa bài hát: gỡ khỏi playlist và hàng phát trước
    this->songSvc.setBeforeRemoveHook([this](Song* s){
        this->playlistSvc.purgeSong(s);
        this->playerSvc.purgeSong(s);
    });
    // Xóa playlist: báo cho tầng phát nhạc
    this->playlistSvc.setAfterRemoveHook([this](const std::string& id){
        this->playerSvc.onPlaylistRemoved(id);
    });
}
void MusicSystem::load(){
    this->songRepo.load();          // thứ tự bắt buộc: bài hát -> người dùng -> playlist
    this->userRepo.load();
    this->playlistRepo.load();
}

bool MusicSystem::login(const std::string& username, const std::string& password){
    bool ok = this->auth.login(username, password);
    if (ok) this->playerSvc.reset();
    return ok;
}
void MusicSystem::logout(){
    this->auth.logout();
    this->playerSvc.reset();
}
bool MusicSystem::isLoggedIn() const{ return this->auth.isLoggedIn(); }
bool MusicSystem::isAdmin() const{ return this->auth.isAdmin(); }
std::string MusicSystem::currentUsername() const{ return this->auth.currentUsername(); }

std::vector<SongInfo> MusicSystem::allSongs() const{ return this->songSvc.getAll(); }
std::vector<SongInfo> MusicSystem::search(const std::string& keyword) const{ return this->songSvc.search(keyword); }
std::vector<SongInfo> MusicSystem::topSongs(int n) const{ return this->songSvc.top(n); }
bool MusicSystem::addSong(const std::string& name, const std::string& artist, const std::string& album,
                          const std::string& type, int durationSec, const std::string& dateRelease,
                          const std::string& sourceFilePath){
    return this->songSvc.addSong(name, artist, album, type, durationSec, dateRelease, sourceFilePath);
}
bool MusicSystem::removeSong(const std::string& idSong){ return this->songSvc.removeSong(idSong); }

bool MusicSystem::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    return this->playerSvc.startQueue(ids, startId, out);
}
bool MusicSystem::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    return this->playerSvc.startPlaylist(idPlaylist, startId, out);
}
bool MusicSystem::nextTrack(SongInfo& out){ return this->playerSvc.next(out); }
bool MusicSystem::prevTrack(SongInfo& out){ return this->playerSvc.prev(out); }
void MusicSystem::recordPlay(const std::string& idSong){ this->playerSvc.recordPlay(idSong); }

std::vector<PlaylistInfo> MusicSystem::myPlaylists() const{ return this->playlistSvc.myPlaylists(); }
std::vector<SongInfo> MusicSystem::playlistSongs(const std::string& idPlaylist){ return this->playlistSvc.songsOf(idPlaylist); }
std::string MusicSystem::createPlaylist(const std::string& name){ return this->playlistSvc.create(name); }
bool MusicSystem::deletePlaylist(const std::string& idPlaylist){ return this->playlistSvc.remove(idPlaylist); }
bool MusicSystem::addToPlaylist(const std::string& idPlaylist, const std::string& idSong){
    return this->playlistSvc.addSong(idPlaylist, idSong);
}
bool MusicSystem::removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong){
    return this->playlistSvc.removeSong(idPlaylist, idSong);
}
