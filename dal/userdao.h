#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "src/user.h"
#include "src/admin.h"
#include "src/listener.h"
#include "textutil.h"

class UserDAO{
    private:
        std::string adminPath;
        std::string listenerPath;
    public:
        explicit UserDAO(const std::string& root);
        std::vector<User*> loadAll() const; // Đọc danh sách Admin và Listener
        bool saveAll(const std::vector<User*>& users) const; // Ghi danh sách tài khoản vào hai file
};
std::vector<User*> UserDAO::loadAll() const {
    std::vector<User*> danhSachUser;
    std::vector<std::string> danhSachDongAdmin = textutil::readLines(adminPath);
    for (const std::string& dong : danhSachDongAdmin){
        // Xác định dấu phân cách của dòng
        char dauPhanCach = textutil::detectDelim(dong);
        // Tách dòng thành các cột
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach);
        if (cot.size() < 4){ // Admin cần tối thiểu 4 cột
            continue;
        }
        // Tạo đối tượng Admin từ dữ liệu trong file
        Admin* admin = new Admin(
            cot[0],     // ID
            cot[1],     // Username
            cot[2],     // Email
            cot[3]      // Password
        );
        // Thêm Admin vào danh sách chung
        danhSachUser.push_back(admin);
    }
    std::vector<std::string> danhSachDongListener = textutil::readLines(listenerPath);
    for (const std::string& dong : danhSachDongListener){
        // Xác định dấu phân cách
        char dauPhanCach = textutil::detectDelim(dong);
        // Tách dòng thành các cột
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach);
        if (cot.size() < 4){ // Listener cần tối thiểu 4 cột
            continue;
        }
        // Tạo đối tượng Listener
        Listener* listener = new Listener(
            cot[0],     // ID
            cot[1],     // Username
            cot[2],     // Email
            cot[3]      // Password
        );
        // Nếu có cột thứ 5 thì đọc tổng số bài hát đã nghe
        if (cot.size() > 4){
            int tongSoBaiHatDaNghe = textutil::toInt(cot[4]);
            listener->setTotalSongsPlayed(tongSoBaiHatDaNghe);
        }
        // Nếu có cột thứ 6 thì đọc trạng thái tài khoản
        if (cot.size() > 5){
            bool dangHoatDong = (cot[5] != "0");
            listener->setActive(dangHoatDong);
        }
        // Nếu có cột thứ 7 thì đọc danh sách bài hát yêu thích
        if (cot.size() > 6){
            char dauPhanCachDanhSach = textutil::listDelim(dauPhanCach);
            std::vector<std::string> danhSachIdBaiHat = textutil::split(cot[6], dauPhanCachDanhSach);
            for (const std::string& idBaiHat : danhSachIdBaiHat){
                listener->likeSong(idBaiHat);
            }
        }
        // Nếu có cột thứ 8 thì đọc danh sách nghệ sĩ đang theo dõi
        if (cot.size() > 7){
            char dauPhanCachDanhSach = textutil::listDelim(dauPhanCach);
            std::vector<std::string> danhSachIdArtist = textutil::split(cot[7], dauPhanCachDanhSach);
            for (const std::string& idArtist : danhSachIdArtist){
                listener->followArtist(idArtist);
            }
        }
        // Nếu có cột thứ 9 thì đọc lịch sử nghe nhạc
        if (cot.size() > 8){
            char dauPhanCachDanhSach = textutil::listDelim(dauPhanCach);
            std::vector<std::string> lichSuNgheNhac = textutil::split(cot[8], dauPhanCachDanhSach);
            // Thêm lần lượt từ bài cũ đến bài mới
            for (const std::string& idBaiHat : lichSuNgheNhac){
                listener->addRecentSong(idBaiHat);
            }
        }
        // Thêm Listener vào danh sách chung
        danhSachUser.push_back(listener);
    }
    return danhSachUser;
}