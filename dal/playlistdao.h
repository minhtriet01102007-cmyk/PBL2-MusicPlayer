#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/playlist.h"
#include "textutil.h"

class PlaylistDAO{
    private:
        std::string filePath;
    public:
        explicit PlaylistDAO(const std::string& root);
        std::vector<Playlist> loadAll() const;
        bool saveAll(const std::vector<Playlist>& playlists) const;
};
PlaylistDAO::PlaylistDAO(const std::string& root){
    filePath = root + "/data/playlists.txt";
}
std::vector<Playlist> PlaylistDAO::loadAll() const{
    std::vector<Playlist> danhSachPlaylist;
    std::vector<std::string> danhSachDong = textutil::readLines(filePath);
    if (danhSachDong.empty()){
        return danhSachPlaylist;
    }
    char dauPhanCach = textutil::detectDelim(danhSachDong[0]);
    for (const std::string& dong : danhSachDong){
        // Tách dòng thành các cột
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach);
        if (cot.size() < 3){ // Playlist cần ít nhất 3 thông tin
            continue;
        }
        // Tạo đối tượng Playlist
        Playlist playlist(
            cot[0], // ID Playlist
            cot[1], // Tên Playlist
            cot[2]  // ID User sở hữu
        );
        // Nếu có danh sách bài hát thì đọc thêm
        if (cot.size() > 3){
            // Xác định dấu phân cách giữa các ID bài hát
            char dauPhanCachBaiHat = textutil::listDelim(dauPhanCach);
            // Tách danh sách ID bài hát
            std::vector<std::string> danhSachIdBaiHat = textutil::split(cot[3], dauPhanCachBaiHat);
            // Thêm từng bài hát vào Playlist
            for (const std::string& idBaiHat : danhSachIdBaiHat){
                playlist.addSong(idBaiHat);
            }
        }
        // Thêm Playlist vào danh sách kết quả
        danhSachPlaylist.push_back(playlist);
    }
    return danhSachPlaylist;
}
bool PlaylistDAO::saveAll(const std::vector<Playlist>& playlists) const{
    char dauPhanCach = textutil::detectFileDelim(filePath);
    std::ofstream file(filePath);
    if (!file.is_open()){
        return false;
    }
    char dauPhanCachBaiHat = textutil::listDelim(dauPhanCach);
    std::string dauPhanCachDanhSach(1, dauPhanCachBaiHat);
    // Duyệt từng Playlist
    for (const Playlist& playlist : playlists){
        // Lấy danh sách ID bài hát
        std::vector<std::string> danhSachIdBaiHat = playlist.getSongIds();
        // Ghép các ID bài hát thành một chuỗi
        std::string danhSachBaiHat = textutil::join(danhSachIdBaiHat, dauPhanCachDanhSach);
        // Tạo danh sách các cột dữ liệu
        std::vector<std::string> cot ={
            playlist.getIdPlaylist(),
            playlist.getNamePlaylist(),
            playlist.getIdUser(),
            danhSachBaiHat
        };
        // Ghép các cột và ghi xuống file
        file << textutil::join(cot, textutil::columnSep(dauPhanCach)) << '\n';
    }
    return true;
}