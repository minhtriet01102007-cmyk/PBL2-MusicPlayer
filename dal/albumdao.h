#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/album.h"
#include "textutil.h"

class AlbumDAO{
    private:
        std::string filePath;
    public:
        explicit AlbumDAO(const std::string& root);
        std::vector<Album*> loadAll() const;
        bool saveAll(const std::vector<Album*>& albums) const;
};
AlbumDAO::AlbumDAO(const std::string& root){
    filePath = root + "/data/albums.txt";
}
std::vector<Album*> AlbumDAO::loadAll() const{
    std::vector<Album*> danhSachAlbum;
    std::vector<std::string> danhSachDong = textutil::readLines(filePath);  // Đọc từng dòng trong file
    if (danhSachDong.empty()){ 
        return danhSachAlbum; // Nếu file không có dữ liệu thì trả về danh sách rỗng
    }
    char dauPhanCach = textutil::detectDelim(danhSachDong[0]);  // Xác định ký tự phân cách giữa các cột
    for (const std::string& dong : danhSachDong){
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach); // Tách dòng thành các cột
        if (cot.size() < 4){ // Album cần ít nhất 4 thông tin
            continue;
        }
        // Tạo đối tượng Album từ dữ liệu trong file
        Album* album = new Album(
            cot[0], // ID Album
            cot[1], // Tên Album
            cot[2], // ID Artist
            cot[3]  // Ngày phát hành
        );
        if (cot.size() > 4){  // Nếu có danh sách bài hát thì đọc thêm
            char dauPhanCachDanhSach = textutil::listDelim(dauPhanCach);
            std::vector<std::string> danhSachBaiHat = textutil::split(cot[4], dauPhanCachDanhSach);
            for (const std::string& idBaiHat : danhSachBaiHat){  // Thêm từng bài hát vào Album
                album->addSong(idBaiHat);
            }
        }
        danhSachAlbum.push_back(album); // Thêm Album vào danh sách kết quả
    }
    return danhSachAlbum;
}
bool AlbumDAO::saveAll(const std::vector<Album*>& albums) const{ // Ghi toàn bộ Album xuống file
    char dauPhanCach = textutil::detectFileDelim(filePath);
    std::ofstream file(filePath); // Mở file để ghi dữ liệu
    if (!file.is_open()){
        return false;
    }
    for (Album* album : albums){  // Duyệt từng Album
        // Lấy danh sách ID bài hát
        std::string danhSachBaiHat = textutil::join(
            album->getSongIds(),
            std::string(1, textutil::listDelim(dauPhanCach))
        );
        // Tạo danh sách các cột dữ liệu
        std::vector<std::string> cot = {
            album->getIdAlbum(),
            album->getNameAlbum(),
            album->getIdArtist(),
            album->getDateRelease(),
            danhSachBaiHat
        };
        // Ghép các cột và ghi xuống file
        file << textutil::join(cot, textutil::columnSep(dauPhanCach)) << '\n';
    }
    return true;
}