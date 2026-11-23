#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/artist.h"
#include "textutil.h"

class ArtistDAO{
    private:
        std::string filePath;
    public:
        explicit ArtistDAO(const std::string& root);    
        std::vector<Artist*> loadAll() const;
        bool saveAll(const std::vector<Artist*>& artists) const;
};
ArtistDAO::ArtistDAO(const std::string& root){
    filePath = root + "/data/artists.txt";
}
std::vector<Artist*> ArtistDAO::loadAll() const{
    std::vector<Artist*> danhSachArtist;
    std::vector<std::string> danhSachDong = textutil::readLines(filePath);
    if (danhSachDong.empty()){ // Nếu file rỗng thì trả về danh sách rỗng
        return danhSachArtist;
    }
    char dauPhanCach = textutil::detectDelim(danhSachDong[0]); // Xác định dấu phân cách giữa các cột
    for (const std::string& dong : danhSachDong){
        // Tách dòng thành các cột
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach);
        if (cot.size() < 3){ // Artist cần ít nhất 3 thông tin
            continue;
        } 
        long long followers = 0; // Lấy số lượng người theo dõi
        // Nếu có cột Followers thì chuyển sang kiểu số
        if (cot.size() > 3){
            followers = textutil::toLL(cot[3]);
        }
        // Tạo đối tượng Artist
        Artist* artist = new Artist(
            cot[0],       // ID Artist
            cot[1],       // Tên Artist
            cot[2],       // Quốc gia
            followers     // Số người theo dõi
        );
        // Thêm Artist vào danh sách kết quả
        danhSachArtist.push_back(artist);
    }
    return danhSachArtist;
}
// Ghi toàn bộ Artist xuống file
bool ArtistDAO::saveAll(const std::vector<Artist*>& artists) const{
    char dauPhanCach = textutil::detectFileDelim(filePath); // Xác định dấu phân cách của file
    std::ofstream file(filePath);
    if (!file.is_open()){
        return false;
    }
    // Duyệt từng Artist
    for (Artist* artist : artists){
        // Lấy thông tin của Artist
        std::vector<std::string> cot = {
            artist->getIdArtist(),
            artist->getNameArtist(),
            artist->getCountry(),
            std::to_string(artist->getFollowers())
        };
        // Ghép các cột thành một dòng và ghi vào file
        file << textutil::join(cot, textutil::columnSep(dauPhanCach)) << '\n';
    }
    return true;
}