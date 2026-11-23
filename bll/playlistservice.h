#pragma once
#include <string>
#include <vector>
#include "playlistinfo.h"
#include "songinfo.h"
#include "authservice.h"
#include "songservice.h"
#include "src/playlist.h"
#include "src/listener.h"
#include "dal/playlistdao.h"
#include "dal/textutil.h"

// PlaylistService quản lý nghiệp vụ playlist.
// Mỗi Listener có thể sở hữu nhiều Playlist.
// Playlist được lưu bên trong Listener (Composition).

class PlaylistService{
    private:
        PlaylistDAO dao;
        AuthService& auth;
        SongService& songs;
        Playlist* find(const std::string& idPlaylist) const; // Tìm playlist của người dùng đang đăng nhập.
    public:
        PlaylistService(const std::string& root, AuthService& auth, SongService& songs);
        void load();
        bool save();
        std::vector<PlaylistInfo> myPlaylists() const;
        std::vector<SongInfo> songsOf(const std::string& idPlaylist) const;
        std::vector<std::string> songIdsOf(const std::string& idPlaylist) const;
        bool containsSong(const std::string& idPlaylist, const std::string& idSong) const;
        std::string create(const std::string& name);
        bool rename(const std::string& idPlaylist, const std::string& newName);
        bool remove(const std::string& idPlaylist);
        bool addSong(const std::string& idPlaylist, const std::string& idSong);
        bool removeSong(const std::string& idPlaylist, const std::string& idSong);
};
PlaylistService::PlaylistService(const std::string& root, AuthService& auth, SongService& songs) : dao(root), auth(auth), songs(songs){
}
Playlist* PlaylistService::find(const std::string& idPlaylist) const{
    Listener* listener = auth.currentListener(); // Lấy người dùng đang đăng nhập
    // Nếu không có Listener thì không thể tìm playlist.
    if (listener == nullptr){
        return nullptr;
    }
    return listener->getPlaylistById(idPlaylist); // Tìm playlist bên trong Listener
}
void PlaylistService::load(){
    // Đọc toàn bộ playlist từ file
    std::vector<Playlist> allPlaylists = dao.loadAll();
    for (const Playlist& playlist : allPlaylists){
        // Tìm người sở hữu playlist
        User* user = auth.findById(playlist.getIdUser());
        // Không tìm thấy người dùng thì bỏ qua
        if (user == nullptr){
            continue;
        }
        // Admin không sở hữu playlist
        if (user->isAdmin()){
            continue;
        }
        Listener* listener = static_cast<Listener*>(user); // Chuyển User* thành Listener*.
        listener->addPlaylist(playlist); // Thêm playlist vào Listener tương ứng.
    }
}
bool PlaylistService::save(){
    std::vector<Playlist> allPlaylists;
    // Lấy danh sách tất cả Listener
    std::vector<Listener*> listeners = auth.listeners();
    for (Listener* listener : listeners){
        // Lấy các playlist của Listener hiện tại
        const std::vector<Playlist>& playlists = listener->getPlaylists();
        // Gom playlist vào danh sách chung
        for (const Playlist& playlist : playlists){
            allPlaylists.push_back(playlist);
        }
    }
    return dao.saveAll(allPlaylists); // Ghi toàn bộ playlist xuống file
}
// Lấy thông tin tất cả playlist của Listener đang đăng nhập.
std::vector<PlaylistInfo>
PlaylistService::myPlaylists() const{
    std::vector<PlaylistInfo> result;
    Listener* listener = auth.currentListener(); // Lấy Listener hiện tại
    if (listener == nullptr){
        return result;
    }
    for (const Playlist& playlist : listener->getPlaylists()){
        PlaylistInfo info;
        // Lấy ID và tên playlist
        info.id = playlist.getIdPlaylist();
        info.name = playlist.getNamePlaylist();
        for (const std::string& idSong : playlist.getSongIds()){
            Song* song = songs.findSong(idSong);
            if (song == nullptr){
                continue;
            }
            info.songCount++;
            info.totalSeconds += song->getDuration(); // Cộng thời lượng bài hát
        }
        result.push_back(info); // Thêm thông tin playlist vào kết quả
    }
    return result;
}
// Lấy danh sách ID bài hát trong playlist
std::vector<std::string>
PlaylistService::songIdsOf(const std::string& idPlaylist) const{
    Playlist* playlist = find(idPlaylist);
    if (playlist == nullptr){
        return {};
    }
    return playlist->getSongIds();
}
// Lấy thông tin đầy đủ các bài hát trong playlist
std::vector<SongInfo>
PlaylistService::songsOf(const std::string& idPlaylist) const{
    std::vector<std::string> ids = songIdsOf(idPlaylist); // Lấy ID bài hát trong playlist
    return songs.toInfos(ids); // Chuyển danh sách ID thành danh sách SongInfo
}
// Kiểm tra bài hát có nằm trong playlist hay không
bool PlaylistService::containsSong(const std::string& idPlaylist, const std::string& idSong) const{
    Playlist* playlist = find(idPlaylist);
    if (playlist == nullptr){
        return false;
    }
    return playlist->containsSong(idSong);
}
// Tạo playlist mới cho Listener đang đăng nhập
std::string PlaylistService::create(const std::string& name){
    Listener* listener = auth.currentListener(); // Lấy Listener hiện tại
    std::string newName = textutil::trim(name); // Xóa khoảng trắng thừa ở đầu và cuối tên
    if (listener == nullptr){
        return "";
    }
    if (newName.empty()){
        return "";
    }
    if (!textutil::isSafeText(newName)){ // Kiểm tra tên playlist có hợp lệ không
        return "";
    }
    // Thu thập ID của tất cả playlist trong hệ thống
    std::vector<std::string> ids;
    for (Listener* other : auth.listeners()){
        for (const Playlist& playlist : other->getPlaylists()){
            ids.push_back(playlist.getIdPlaylist());
        }
    }
    std::string id = textutil::nextId(ids, "PL"); // Tạo ID mới, tiền tố là PL
    Playlist newPlaylist(id, newName, listener->getIdUser()); // Tạo đối tượng Playlist mới
    listener->addPlaylist(newPlaylist); // Thêm playlist vào Listener
    save(); // Lưu dữ liệu xuống file
    return id;
}
// Đổi tên playlist theo ID
bool PlaylistService::rename(const std::string& idPlaylist, const std::string& newName){
    Playlist* playlist = find(idPlaylist); // Tìm playlist cần đổi tên
    std::string name = textutil::trim(newName); // Chuẩn hóa tên mới
    if (playlist == nullptr){
        return false;
    }
    if (name.empty()){
        return false;
    }
    if (!textutil::isSafeText(name)){
        return false;
    }
    playlist->setNamePlaylist(name); // Cập nhật tên playlist
    save(); // Lưu thay đổi
    return true;
}
// Xóa playlist của Listener đang đăng nhập
bool PlaylistService::remove(const std::string& idPlaylist){
    Listener* listener = auth.currentListener(); // Lấy Listener hiện tại
    if (listener == nullptr){
        return false;
    }
    // Xóa playlist khỏi Listener
    bool success = listener->removePlaylist(idPlaylist);
    if (!success){
        return false;
    }
    save(); // Lưu thay đổi xuống file
    return true;
}
// Thêm một bài hát vào playlist
bool PlaylistService::addSong(const std::string& idPlaylist, const std::string& idSong){
    Playlist* playlist = find(idPlaylist); // Tìm playlist
    if (playlist == nullptr){
        return false;
    }
    if (songs.findSong(idSong) == nullptr){
        return false;
    }
    // Thêm bài hát vào playlist, nếu bài hát đã tồn tại thì addSong trả về false.
    bool success = playlist->addSong(idSong);
    if (!success){
        return false;
    }
    save();
    return true;
}
// Xóa một bài hát khỏi playlist
bool PlaylistService::removeSong(const std::string& idPlaylist, const std::string& idSong){
    Playlist* playlist = find(idPlaylist); // Tìm playlist
    if (playlist == nullptr){
        return false;
    }
    // Xóa bài hát khỏi playlist
    bool success = playlist->removeSong(idSong);
    if (!success){
        return false;
    }
    save();
    return true;
}
