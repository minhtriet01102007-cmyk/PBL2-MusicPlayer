#pragma once
#include <string>
#include <vector>
#include "songinfo.h"
#include "artistinfo.h"
#include "albuminfo.h"
#include "playlistinfo.h"
#include "admininfo.h"
#include "authservice.h"
#include "songservice.h"
#include "playlistservice.h"
#include "libraryservice.h"
#include "playerservice.h"

// Cổng duy nhất của tầng nghiệp vụ: giao diện chỉ nói chuyện với MusicSystem, không đụng tới service, DAO hay entity.
// MusicSystem cũng là nơi điều phối các thao tác liên quan nhiều service (ví dụ xóa bài hát phải dọn playlist, hàng đợi...).
// Quy ước: các hàm trả về std::string là thông báo lỗi, chuỗi rỗng nghĩa là thành công.
class MusicSystem{
    private:
        AuthService auth;
        SongService songSvc;
        PlaylistService playlistSvc;
        LibraryService librarySvc;
        PlayerService playerSvc;
    public:
        explicit MusicSystem(const std::string& root);
        MusicSystem(const MusicSystem&) = delete;
        MusicSystem& operator=(const MusicSystem&) = delete;
        void load();

        // --- Tài khoản ---
        AuthService::LoginResult login(const std::string& username, const std::string& password);
        void logout();
        std::string registerListener(const std::string& username, const std::string& email, const std::string& password);
        std::string changePassword(const std::string& oldPassword, const std::string& newPassword);
        bool isLoggedIn() const;
        bool isAdmin() const;
        std::string currentUsername() const;
        std::string currentEmail() const;

        // --- Khám phá ---
        std::vector<SongInfo> allSongs() const;
        std::vector<ArtistInfo> allArtists() const;
        std::vector<AlbumInfo> allAlbums() const;
        std::vector<SongInfo> searchSongs(const std::string& keyword) const;
        std::vector<ArtistInfo> searchArtists(const std::string& keyword) const;
        std::vector<AlbumInfo> searchAlbums(const std::string& keyword) const;
        std::vector<SongInfo> topSongs(int n) const;
        std::vector<ArtistInfo> topArtists(int n) const;
        std::vector<SongInfo> newestSongs(int n) const;
        std::vector<std::string> genres() const;
        std::vector<SongInfo> songsByGenre(const std::string& genre) const;
        std::vector<SongInfo> songsOfArtist(const std::string& idArtist) const;
        std::vector<SongInfo> songsOfAlbum(const std::string& idAlbum) const;
        std::vector<AlbumInfo> albumsOfArtist(const std::string& idArtist) const;
        bool artistInfo(const std::string& idArtist, ArtistInfo& out) const;
        bool albumInfo(const std::string& idAlbum, AlbumInfo& out) const;

        // --- Thư viện cá nhân ---
        bool toggleLike(const std::string& idSong);
        std::vector<SongInfo> likedSongs() const;
        bool toggleFollow(const std::string& idArtist);
        std::vector<ArtistInfo> followedArtists() const;
        std::vector<SongInfo> recentSongs(int n) const;
        std::vector<PlaylistInfo> myPlaylists() const;
        std::vector<SongInfo> playlistSongs(const std::string& idPlaylist) const;
        bool playlistContains(const std::string& idPlaylist, const std::string& idSong) const;
        std::string createPlaylist(const std::string& name);
        bool renamePlaylist(const std::string& idPlaylist, const std::string& newName);
        bool deletePlaylist(const std::string& idPlaylist);
        bool addToPlaylist(const std::string& idPlaylist, const std::string& idSong);
        bool removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong);

        // --- Phát nhạc ---
        bool startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out);
        bool startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out);
        bool nextTrack(SongInfo& out);
        bool prevTrack(SongInfo& out);
        bool trackFinished(SongInfo& out);
        bool currentTrack(SongInfo& out) const;
        void recordPlay(const std::string& idSong);
        void setShuffle(bool on);
        bool isShuffle() const;
        void setRepeatMode(PlayerService::RepeatMode mode);
        PlayerService::RepeatMode getRepeatMode() const;
        bool playNext(const std::string& idSong);
        bool addToQueue(const std::string& idSong);
        std::vector<SongInfo> upcoming() const;

        // --- Quản trị (Admin) ---
        std::string addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
                            int durationSec, const std::string& dateRelease, const std::string& sourceFilePath);
        std::string updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                               const std::string& type, int durationSec, const std::string& dateRelease);
        std::string removeSong(const std::string& idSong);
        std::string addArtist(const std::string& name, const std::string& country);
        std::string removeArtist(const std::string& idArtist);
        std::string addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease);
        std::string removeAlbum(const std::string& idAlbum);
        std::string setAlbumSongs(const std::string& idAlbum, const std::vector<std::string>& songIds);
        std::vector<UserInfo> listUsers() const;
        std::string setUserLocked(const std::string& idUser, bool locked);
        StatsInfo stats() const;
};

MusicSystem::MusicSystem(const std::string& root)
    : auth(root), songSvc(root, auth), playlistSvc(root, auth, songSvc), librarySvc(auth, songSvc), playerSvc(auth, songSvc, playlistSvc){
}
void MusicSystem::load(){
    this->songSvc.load();
    this->auth.load();
    this->playlistSvc.load();   // phải sau auth.load() vì cần gắn playlist vào đúng Listener
}

AuthService::LoginResult MusicSystem::login(const std::string& username, const std::string& password){
    AuthService::LoginResult r = this->auth.login(username, password);
    if (r == AuthService::LoginOk) this->playerSvc.reset();
    return r;
}
void MusicSystem::logout(){
    this->playerSvc.reset();
    this->auth.logout();
}
std::string MusicSystem::registerListener(const std::string& username, const std::string& email, const std::string& password){
    return this->auth.registerListener(username, email, password);
}
std::string MusicSystem::changePassword(const std::string& oldPassword, const std::string& newPassword){
    return this->auth.changePassword(oldPassword, newPassword);
}
bool MusicSystem::isLoggedIn() const{ return this->auth.isLoggedIn(); }
bool MusicSystem::isAdmin() const{ return this->auth.isAdmin(); }
std::string MusicSystem::currentUsername() const{ return this->auth.currentUsername(); }
std::string MusicSystem::currentEmail() const{ return this->auth.currentEmail(); }

std::vector<SongInfo> MusicSystem::allSongs() const{ return this->songSvc.getAllSongs(); }
std::vector<ArtistInfo> MusicSystem::allArtists() const{ return this->songSvc.getAllArtists(); }
std::vector<AlbumInfo> MusicSystem::allAlbums() const{ return this->songSvc.getAllAlbums(); }
std::vector<SongInfo> MusicSystem::searchSongs(const std::string& keyword) const{ return this->songSvc.searchSongs(keyword); }
std::vector<ArtistInfo> MusicSystem::searchArtists(const std::string& keyword) const{ return this->songSvc.searchArtists(keyword); }
std::vector<AlbumInfo> MusicSystem::searchAlbums(const std::string& keyword) const{ return this->songSvc.searchAlbums(keyword); }
std::vector<SongInfo> MusicSystem::topSongs(int n) const{ return this->songSvc.topSongs(n); }
std::vector<ArtistInfo> MusicSystem::topArtists(int n) const{ return this->songSvc.topArtists(n); }
std::vector<SongInfo> MusicSystem::newestSongs(int n) const{ return this->songSvc.newestSongs(n); }
std::vector<std::string> MusicSystem::genres() const{ return this->songSvc.genres(); }
std::vector<SongInfo> MusicSystem::songsByGenre(const std::string& genre) const{ return this->songSvc.songsByGenre(genre); }
std::vector<SongInfo> MusicSystem::songsOfArtist(const std::string& idArtist) const{ return this->songSvc.songsOfArtist(idArtist); }
std::vector<SongInfo> MusicSystem::songsOfAlbum(const std::string& idAlbum) const{ return this->songSvc.songsOfAlbum(idAlbum); }
std::vector<AlbumInfo> MusicSystem::albumsOfArtist(const std::string& idArtist) const{ return this->songSvc.albumsOfArtist(idArtist); }
bool MusicSystem::artistInfo(const std::string& idArtist, ArtistInfo& out) const{
    Artist* a = this->songSvc.findArtist(idArtist);
    if (a == nullptr) return false;
    out = this->songSvc.toInfo(*a);
    return true;
}
bool MusicSystem::albumInfo(const std::string& idAlbum, AlbumInfo& out) const{
    Album* al = this->songSvc.findAlbum(idAlbum);
    if (al == nullptr) return false;
    out = this->songSvc.toInfo(*al);
    return true;
}

bool MusicSystem::toggleLike(const std::string& idSong){ return this->librarySvc.toggleLike(idSong); }
std::vector<SongInfo> MusicSystem::likedSongs() const{ return this->librarySvc.likedSongs(); }
bool MusicSystem::toggleFollow(const std::string& idArtist){ return this->librarySvc.toggleFollow(idArtist); }
std::vector<ArtistInfo> MusicSystem::followedArtists() const{ return this->librarySvc.followedArtists(); }
std::vector<SongInfo> MusicSystem::recentSongs(int n) const{ return this->librarySvc.recentSongs(n); }
std::vector<PlaylistInfo> MusicSystem::myPlaylists() const{ return this->playlistSvc.myPlaylists(); }
std::vector<SongInfo> MusicSystem::playlistSongs(const std::string& idPlaylist) const{ return this->playlistSvc.songsOf(idPlaylist); }
bool MusicSystem::playlistContains(const std::string& idPlaylist, const std::string& idSong) const{
    return this->playlistSvc.containsSong(idPlaylist, idSong);
}
std::string MusicSystem::createPlaylist(const std::string& name){ return this->playlistSvc.create(name); }
bool MusicSystem::renamePlaylist(const std::string& idPlaylist, const std::string& newName){ return this->playlistSvc.rename(idPlaylist, newName); }
bool MusicSystem::deletePlaylist(const std::string& idPlaylist){ return this->playlistSvc.remove(idPlaylist); }
bool MusicSystem::addToPlaylist(const std::string& idPlaylist, const std::string& idSong){ return this->playlistSvc.addSong(idPlaylist, idSong); }
bool MusicSystem::removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong){ return this->playlistSvc.removeSong(idPlaylist, idSong); }

bool MusicSystem::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    return this->playerSvc.startQueue(ids, startId, out);
}
bool MusicSystem::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    return this->playerSvc.startPlaylist(idPlaylist, startId, out);
}
bool MusicSystem::nextTrack(SongInfo& out){ return this->playerSvc.next(out); }
bool MusicSystem::prevTrack(SongInfo& out){ return this->playerSvc.prev(out); }
bool MusicSystem::trackFinished(SongInfo& out){ return this->playerSvc.onTrackFinished(out); }
bool MusicSystem::currentTrack(SongInfo& out) const{ return this->playerSvc.getCurrent(out); }
void MusicSystem::recordPlay(const std::string& idSong){ this->playerSvc.recordPlay(idSong); }
void MusicSystem::setShuffle(bool on){ this->playerSvc.setShuffle(on); }
bool MusicSystem::isShuffle() const{ return this->playerSvc.isShuffle(); }
void MusicSystem::setRepeatMode(PlayerService::RepeatMode mode){ this->playerSvc.setRepeatMode(mode); }
PlayerService::RepeatMode MusicSystem::getRepeatMode() const{ return this->playerSvc.getRepeatMode(); }
bool MusicSystem::playNext(const std::string& idSong){ return this->playerSvc.playNext(idSong); }
bool MusicSystem::addToQueue(const std::string& idSong){ return this->playerSvc.addToQueue(idSong); }
std::vector<SongInfo> MusicSystem::upcoming() const{ return this->playerSvc.upcoming(); }

std::string MusicSystem::addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
                                 int durationSec, const std::string& dateRelease, const std::string& sourceFilePath){
    return this->songSvc.addSong(name, idArtist, idAlbum, type, durationSec, dateRelease, sourceFilePath);
}
std::string MusicSystem::updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                                    const std::string& type, int durationSec, const std::string& dateRelease){
    return this->songSvc.updateSong(id, name, idArtist, idAlbum, type, durationSec, dateRelease);
}
std::string MusicSystem::removeSong(const std::string& idSong){
    std::string err = this->songSvc.removeSong(idSong);
    if (!err.empty()) return err;
    this->playerSvc.purgeSong(idSong);   // gỡ khỏi hàng đợi đang phát
    this->auth.purgeSong(idSong);        // gỡ khỏi playlist, yêu thích, lịch sử của mọi Listener
    this->auth.save();
    this->playlistSvc.save();
    return "";
}
std::string MusicSystem::addArtist(const std::string& name, const std::string& country){
    return this->songSvc.addArtist(name, country);
}
std::string MusicSystem::removeArtist(const std::string& idArtist){
    std::string err = this->songSvc.removeArtist(idArtist);
    if (!err.empty()) return err;
    this->auth.purgeArtist(idArtist);    // gỡ khỏi danh sách theo dõi
    this->auth.save();
    return "";
}
std::string MusicSystem::addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease){
    return this->songSvc.addAlbum(name, idArtist, dateRelease);
}
std::string MusicSystem::removeAlbum(const std::string& idAlbum){ return this->songSvc.removeAlbum(idAlbum); }
std::string MusicSystem::setAlbumSongs(const std::string& idAlbum, const std::vector<std::string>& songIds){
    return this->songSvc.setAlbumSongs(idAlbum, songIds);
}
std::vector<UserInfo> MusicSystem::listUsers() const{ return this->auth.listUsers(); }
std::string MusicSystem::setUserLocked(const std::string& idUser, bool locked){ return this->auth.setLocked(idUser, locked); }
StatsInfo MusicSystem::stats() const{
    StatsInfo s;
    s.songs = this->songSvc.songCount();
    s.artists = this->songSvc.artistCount();
    s.albums = this->songSvc.albumCount();
    s.users = this->auth.userCount();
    s.listeners = (int)this->auth.listeners().size();
    s.totalPlays = this->songSvc.totalPlays();
    return s;
}