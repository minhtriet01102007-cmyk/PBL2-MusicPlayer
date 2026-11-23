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

        AuthService::LoginResult login(const std::string& username, const std::string& password);
        void logout();
        std::string registerListener(const std::string& username, const std::string& email, const std::string& password);
        std::string changePassword(const std::string& oldPassword, const std::string& newPassword);
        bool isLoggedIn() const;
        bool isAdmin() const;
        std::string currentUsername() const;
        std::string currentEmail() const;

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

        std::string addSong(const std::string& name, const std::string& idArtist,  const std::string& idAlbum, const std::string& type,
                            int durationSec, const std::string& dateRelease, const std::string& sourceFilePath);
        std::string updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                               const std::string& type, int durationSec, const std::string& dateRelease);
        std::string removeSong(const std::string& idSong);
        std::string addArtist(const std::string& name, const std::string& country);
        std::string removeArtist(const std::string& idArtist);
        std::string addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease);
        std::string removeAlbum(const std::string& idAlbum);
        std::vector<UserInfo> listUsers() const;
        std::string setUserLocked(const std::string& idUser, bool locked);
        StatsInfo stats() const;
};
MusicSystem::MusicSystem(const std::string& root) : auth(root), songSvc(root, auth), playlistSvc(root, auth, songSvc),
                                                    librarySvc(auth, songSvc), playerSvc(auth, songSvc, playlistSvc){
}
void MusicSystem::load(){
    songSvc.load(); // Nạp dữ liệu bài hát, nghệ sĩ và album
    auth.load(); // Nạp dữ liệu tài khoản
    playlistSvc.load(); // Nạp playlist sau khi đã nạp tài khoản
}
AuthService::LoginResult MusicSystem::login(const std::string& username, const std::string& password){
    AuthService::LoginResult result;
    result = auth.login(username, password);
    // Đăng nhập thành công thì làm mới trạng thái phát nhạc
    if (result == AuthService::LoginOk){
        playerSvc.reset();
    }
    return result;
}
void MusicSystem::logout(){
    playerSvc.reset(); // Xóa trạng thái phát nhạc
    auth.logout(); // Đăng xuất tài khoản
}
std::string MusicSystem::registerListener(const std::string& username, const std::string& email, const std::string& password){
    return auth.registerListener(username, email, password);
}
std::string MusicSystem::changePassword(const std::string& oldPassword, const std::string& newPassword){
    return auth.changePassword(oldPassword, newPassword);
}
bool MusicSystem::isLoggedIn() const{
    return auth.isLoggedIn();
}
bool MusicSystem::isAdmin() const{
    return auth.isAdmin();
}
std::string MusicSystem::currentUsername() const{
    return auth.currentUsername();
}
std::string MusicSystem::currentEmail() const{
    return auth.currentEmail();
}
std::vector<SongInfo> MusicSystem::allSongs() const{
    return songSvc.getAllSongs();
}
std::vector<ArtistInfo> MusicSystem::allArtists() const{
    return songSvc.getAllArtists();
}
std::vector<AlbumInfo> MusicSystem::allAlbums() const{
    return songSvc.getAllAlbums();
}
std::vector<SongInfo> MusicSystem::searchSongs(const std::string& keyword) const{
    return songSvc.searchSongs(keyword);
}
std::vector<ArtistInfo> MusicSystem::searchArtists(const std::string& keyword) const{
    return songSvc.searchArtists(keyword);
}
std::vector<AlbumInfo> MusicSystem::searchAlbums(const std::string& keyword) const{
    return songSvc.searchAlbums(keyword);
}
std::vector<SongInfo> MusicSystem::topSongs(int n) const{
    return songSvc.topSongs(n);
}
std::vector<ArtistInfo> MusicSystem::topArtists(int n) const{
    return songSvc.topArtists(n);
}
std::vector<SongInfo> MusicSystem::newestSongs(int n) const{
    return songSvc.newestSongs(n);
}
std::vector<std::string> MusicSystem::genres() const{
    return songSvc.genres();
}
std::vector<SongInfo> MusicSystem::songsByGenre(const std::string& genre) const{
    return songSvc.songsByGenre(genre);
}
std::vector<SongInfo> MusicSystem::songsOfArtist(const std::string& idArtist) const{
    return songSvc.songsOfArtist(idArtist);
}
std::vector<SongInfo> MusicSystem::songsOfAlbum(const std::string& idAlbum) const{
    return songSvc.songsOfAlbum(idAlbum);
}
std::vector<AlbumInfo> MusicSystem::albumsOfArtist(const std::string& idArtist) const{
    return songSvc.albumsOfArtist(idArtist);
}
bool MusicSystem::artistInfo(const std::string& idArtist, ArtistInfo& out) const{
    Artist* artist = songSvc.findArtist(idArtist); // Tìm nghệ sĩ theo ID
    // Không tìm thấy nghệ sĩ
    if (artist == nullptr){
        return false;
    }
    // Chuyển thông tin Artist thành ArtistInfo
    out = songSvc.toInfo(*artist);
    return true;
}
bool MusicSystem::albumInfo(const std::string& idAlbum, AlbumInfo& out) const{
    Album* album = songSvc.findAlbum(idAlbum); // Tìm album theo I
    // Không tìm thấy album
    if (album == nullptr){
        return false;
    }
    // Chuyển thông tin Album thành AlbumInfo
    out = songSvc.toInfo(*album);
    return true;
}
bool MusicSystem::toggleLike(const std::string& idSong){
    return librarySvc.toggleLike(idSong);
}
std::vector<SongInfo> MusicSystem::likedSongs() const{
    return librarySvc.likedSongs();
}
bool MusicSystem::toggleFollow(const std::string& idArtist){
    return librarySvc.toggleFollow(idArtist);
}
std::vector<ArtistInfo> MusicSystem::followedArtists() const{
    return librarySvc.followedArtists();
}
std::vector<SongInfo> MusicSystem::recentSongs(int n) const{
    return librarySvc.recentSongs(n);
}
std::vector<PlaylistInfo> MusicSystem::myPlaylists() const{
    return playlistSvc.myPlaylists();
}
std::vector<SongInfo> MusicSystem::playlistSongs(const std::string& idPlaylist) const{
    return playlistSvc.songsOf(idPlaylist);
}
bool MusicSystem::playlistContains(const std::string& idPlaylist, const std::string& idSong) const{
    return playlistSvc.containsSong(idPlaylist, idSong);
}
std::string MusicSystem::createPlaylist(const std::string& name){
    return playlistSvc.create(name);
}
bool MusicSystem::renamePlaylist(const std::string& idPlaylist, const std::string& newName){
    return playlistSvc.rename(idPlaylist, newName);
}
bool MusicSystem::deletePlaylist(const std::string& idPlaylist){
    return playlistSvc.remove(idPlaylist);
}
bool MusicSystem::addToPlaylist(const std::string& idPlaylist, const std::string& idSong){
    return playlistSvc.addSong(idPlaylist, idSong);
}
bool MusicSystem::removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong){
    return playlistSvc.removeSong(idPlaylist, idSong);
}
bool MusicSystem::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    return playerSvc.startQueue(ids, startId, out);
}
bool MusicSystem::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    return playerSvc.startPlaylist(idPlaylist, startId, out);
}
bool MusicSystem::nextTrack(SongInfo& out){
    return playerSvc.next(out);
}
bool MusicSystem::prevTrack(SongInfo& out){
    return playerSvc.prev(out);
}
bool MusicSystem::trackFinished(SongInfo& out){
    return playerSvc.onTrackFinished(out);
}
bool MusicSystem::currentTrack(SongInfo& out) const{
    return playerSvc.getCurrent(out);
}
void MusicSystem::recordPlay(const std::string& idSong){
    playerSvc.recordPlay(idSong);
}
void MusicSystem::setShuffle(bool on){
    playerSvc.setShuffle(on);
}
bool MusicSystem::isShuffle() const{
    return playerSvc.isShuffle();
}
void MusicSystem::setRepeatMode(PlayerService::RepeatMode mode){
    playerSvc.setRepeatMode(mode);
}
PlayerService::RepeatMode MusicSystem::getRepeatMode() const{
    return playerSvc.getRepeatMode();
}
bool MusicSystem::playNext(const std::string& idSong){
    return playerSvc.playNext(idSong);
}
bool MusicSystem::addToQueue(const std::string& idSong){
    return playerSvc.addToQueue(idSong);
}
std::vector<SongInfo> MusicSystem::upcoming() const{
    return playerSvc.upcoming();
}
std::string MusicSystem::addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
    int durationSec, const std::string& dateRelease, const std::string& sourceFilePath){
    return songSvc.addSong(name, idArtist, idAlbum, type, durationSec, dateRelease, sourceFilePath);
}
std::string MusicSystem::updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                                    const std::string& type, int durationSec, const std::string& dateRelease){
    return songSvc.updateSong(id, name, idArtist, idAlbum, type, durationSec, dateRelease);
}
std::string MusicSystem::removeSong(const std::string& idSong){
    std::string error = songSvc.removeSong(idSong); // Xóa bài hát trong SongService
    // Nếu có lỗi thì dừng lại
    if (!error.empty()){
        return error;
    }
    playerSvc.purgeSong(idSong); // Xóa bài hát khỏi hàng đợi đang phát
    auth.purgeSong(idSong); // Xóa bài hát khỏi playlist, yêu thích và lịch sử của tất cả Listener
    // Lưu lại dữ liệu
    auth.save();
    playlistSvc.save();
    return "";
}
std::string MusicSystem::addArtist(const std::string& name, const std::string& country){
    return songSvc.addArtist(name, country);
}
std::string MusicSystem::removeArtist(const std::string& idArtist){
    std::string error = songSvc.removeArtist(idArtist); // Xóa nghệ sĩ
    // Nếu có lỗi thì dừng lại
    if (!error.empty()){
        return error;
    }
    // Xóa nghệ sĩ khỏi danh sách theo dõi của người dùng
    auth.purgeArtist(idArtist);
    // Lưu dữ liệu tài khoản
    auth.save();
    return "";
}
std::string MusicSystem::addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease){
    return songSvc.addAlbum(name, idArtist, dateRelease);
}
std::string MusicSystem::removeAlbum(const std::string& idAlbum){
    return songSvc.removeAlbum(idAlbum);
}
std::vector<UserInfo> MusicSystem::listUsers() const{
    return auth.listUsers();
}
std::string MusicSystem::setUserLocked(const std::string& idUser, bool locked){
    return auth.setLocked(idUser, locked);
}
StatsInfo MusicSystem::stats() const{
    StatsInfo result;
    // Đếm số lượng bài hát
    result.songs = songSvc.songCount();
    // Đếm số lượng nghệ sĩ
    result.artists = songSvc.artistCount();
    // Đếm số lượng album
    result.albums = songSvc.albumCount();
    // Đếm tổng số tài khoản
    result.users = auth.userCount();
    // Đếm số Listener
    result.listeners = (int)auth.listeners().size();
    // Lấy tổng lượt nghe
    result.totalPlays = songSvc.totalPlays();
    return result;
}
