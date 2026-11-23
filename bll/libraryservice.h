#pragma once
#include <string>
#include <vector>
#include "songinfo.h"
#include "artistinfo.h"
#include "authservice.h"
#include "songservice.h"
#include "src/listener.h"

// Quản lý thư viện cá nhân của Listener
class LibraryService{
    private: 
        AuthService& auth; // Quản lý tài khoản đang đăng nhập
        SongService& songs; // Quản lý bài hát và nghệ sĩ
    public:
        LibraryService(AuthService& auth, SongService& songs);
        bool toggleLike(const std::string& idSong); // Thích hoặc bỏ thích bài hát
        std::vector<SongInfo> likedSongs() const;   // Lấy danh sách bài hát đã thích
        bool toggleFollow(const std::string& idArtist); // Theo dõi hoặc bỏ theo dõi nghệ sĩ
        std::vector<ArtistInfo> followedArtists() const; // Lấy danh sách nghệ sĩ đang theo dõi
        std::vector<SongInfo> recentSongs(int n) const; // Lấy lịch sử nghe gần đây
};
LibraryService::LibraryService(AuthService& auth, SongService& songs) : auth(auth), songs(songs){
}
bool LibraryService::toggleLike(const std::string& idSong){
    Listener* listener = auth.currentListener(); // Lấy Listener đang đăng nhập
    if (listener == nullptr){ // Kiểm tra người dùng đã đăng nhập chưa
        return false;
    }
    Song* song = songs.findSong(idSong);  // Kiểm tra bài hát có tồn tại không
    if (song == nullptr){
        return false;
    }
    // Nếu bài hát đã thích thì bỏ thích
    if (listener->isLiked(idSong)){
        listener->unlikeSong(idSong);
    }
    else{
        // Nếu chưa thích thì thêm vào danh sách yêu thích
        listener->likeSong(idSong);
    }
    auth.save(); // Lưu thay đổi xuống file
    return listener->isLiked(idSong); // Trả về trạng thái mới của bài hát
}
std::vector<SongInfo> LibraryService::likedSongs() const{
    std::vector<SongInfo> result;
    Listener* listener = auth.currentListener();  // Lấy Listener đang đăng nhập
    // Nếu chưa đăng nhập thì trả về danh sách rỗng
    if (listener == nullptr){
        return result;
    }
    // Lấy danh sách ID bài hát đã thích
    std::vector<std::string> ids = listener->getLikedSongIds();
    // Đảo ngược danh sách để bài mới thích nằm trên cùng
    std::reverse(ids.begin(), ids.end());
    // Chuyển danh sách ID thành danh sách thông tin bài hát
    result = songs.toInfos(ids);
    return result;
}
bool LibraryService::toggleFollow(const std::string& idArtist){
    // Lấy Listener đang đăng nhập
    Listener* listener = auth.currentListener();
    // Tìm nghệ sĩ theo ID
    Artist* artist = songs.findArtist(idArtist);
    // Kiểm tra Listener và nghệ sĩ có tồn tại không
    if (listener == nullptr || artist == nullptr){
        return false;
    }
    // Kiểm tra Listener đã theo dõi nghệ sĩ chưa
    if (listener->isFollowing(idArtist)){
        // Đã theo dõi thì bỏ theo dõi
        listener->unfollowArtist(idArtist);
        // Giảm số lượng người theo dõi
        artist->decreaseFollowers();
    }
    else{
        // Chưa theo dõi thì thêm vào danh sách
        listener->followArtist(idArtist);
        // Tăng số lượng người theo dõi
        artist->increaseFollowers();
    }
    // Lưu thông tin nghệ sĩ
    songs.saveArtists();
    // Lưu thông tin Listener
    auth.save();
    // Trả về trạng thái theo dõi mới
    return listener->isFollowing(idArtist);
}
std::vector<ArtistInfo> LibraryService::followedArtists() const{
    std::vector<ArtistInfo> result;
    // Lấy Listener đang đăng nhập
    Listener* listener = auth.currentListener();
    // Nếu chưa đăng nhập thì trả về danh sách rỗng
    if (listener == nullptr){
        return result;
    }
    // Lấy danh sách ID nghệ sĩ đang theo dõi
    std::vector<std::string> ids = listener->getFollowedArtistIds();
    for (const std::string& id : ids){
        // Tìm nghệ sĩ trong SongService
        Artist* artist = songs.findArtist(id);
        // Nếu tìm thấy nghệ sĩ
        if (artist != nullptr){
            // Chuyển Artist thành ArtistInfo
            ArtistInfo info = songs.toInfo(*artist);
            // Thêm vào danh sách kết quả
            result.push_back(info);
        }
    }
    return result;
}
std::vector<SongInfo> LibraryService::recentSongs(int n) const{
    std::vector<SongInfo> result;
    // Lấy Listener đang đăng nhập
    Listener* listener = auth.currentListener();
    // Kiểm tra người dùng và số lượng bài hát cần lấy
    if (listener == nullptr || n <= 0){
        return result;
    }
    // Lấy danh sách ID bài hát đã nghe
    std::vector<std::string> ids = listener->getRecentSongIds();
    // Đảo ngược để bài nghe gần nhất nằm trên cùng
    std::reverse(ids.begin(), ids.end());
    // Chỉ lấy tối đa n bài hát
    if ((int)ids.size() > n){
        ids.resize(n);
    }
    // Chuyển danh sách ID thành thông tin bài hát
    result = songs.toInfos(ids);
    return result;
}

