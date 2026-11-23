#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "src/song.h"
#include "textutil.h"

class SongDAO{
    private:
        std::string root;       // Thư mục gốc dự án
        std::string musicDir;   // Thư mục chứa file nhạc
        std::string filePath;   // Đường dẫn đến songs.txt
    public:
        explicit SongDAO(const std::string& rootPath);
        std::vector<Song*> loadAll() const;
        bool saveAll(const std::vector<Song*>& songs) const;
        std::string resolveAudioPath(const Song& song) const; // Tìm đường dẫn file nhạc
        std::string importAudio(const std::string& sourcePath) const;
};
SongDAO::SongDAO(const std::string& rootPath){
    root = rootPath;
    musicDir = rootPath + "/assets/music";
    filePath = rootPath + "/data/songs.txt";
}
std::vector<Song*> SongDAO::loadAll() const{
    std::vector<Song*> danhSachSong;
    std::vector<std::string> danhSachDong = textutil::readLines(filePath);
    if (danhSachDong.empty()){
        return danhSachSong;
    }
    char dauPhanCach = textutil::detectDelim(danhSachDong[0]);
    for (const std::string& dong : danhSachDong){
        std::vector<std::string> cot = textutil::split(dong, dauPhanCach);
        if (cot.size() < 9){  // Kiểm tra dòng có đủ 9 cột hay không
            continue;
        }
        // Chuyển thời lượng bài hát sang số nguyên
        int thoiLuong = textutil::toInt(cot[5]);
        // Chuyển số lượt phát sang long long
        long long soLuotPhat = textutil::toLL(cot[7]);
        // Tạo đối tượng Song từ dữ liệu trong file
        Song* baiHat = new Song(
            cot[0],         // ID bài hát
            cot[1],         // Tên bài hát
            cot[2],         // ID Artist
            cot[3],         // ID Album
            cot[4],         // Thể loại
            thoiLuong,      // Thời lượng
            cot[6],         // Ngày phát hành
            cot[8],         // Đường dẫn file nhạc
            soLuotPhat      // Số lượt phát
        );
        // Thêm bài hát vào danh sách
        danhSachSong.push_back(baiHat);
    }
    return danhSachSong;
}
bool SongDAO::saveAll(const std::vector<Song*>& songs) const{
    char dauPhanCach = textutil::detectFileDelim(filePath);
    std::ofstream file(filePath);
    if (!file.is_open()){
        return false;
    }
    // Duyệt từng bài hát
    for (Song* baiHat : songs){
        // Lấy thông tin của bài hát
        std::vector<std::string> cot = {
            baiHat->getIdSong(),
            baiHat->getName(),
            baiHat->getIdArtist(),
            baiHat->getIdAlbum(),
            baiHat->getTypeSong(),
            std::to_string(baiHat->getDuration()),
            baiHat->getDateRelease(),
            std::to_string(baiHat->getPlays()),
            baiHat->getFilePath()
        };
        // Ghép các cột thành một dòng
        std::string dong = textutil::join(cot, textutil::columnSep(dauPhanCach));
        // Ghi dòng dữ liệu xuống file
        file << dong << '\n';
    }
    return true;
}
std::string SongDAO::resolveAudioPath(const Song& song) const{
    namespace fs = std::filesystem;
    // Lấy đường dẫn file nhạc từ đối tượng Song
    std::string duongDan = song.getFilePath();
    // Nếu không có đường dẫn thì trả về chuỗi rỗng
    if (duongDan.empty()){
        return "";
    }
    // Chuyển dấu \ thành dấu / để thống nhất đường dẫn
    std::replace(
        duongDan.begin(),
        duongDan.end(),
        '\\',
        '/'
    );
    std::error_code loi;
    // Tạo danh sách các đường dẫn có thể chứa file nhạc
    std::vector<std::string> danhSachDuongDan = {
        // Đường dẫn tính từ thư mục gốc dự án
        root + "/" + duongDan,
        // Đường dẫn tính từ thư mục nhạc
        musicDir + "/" + duongDan,
        // Đường dẫn ban đầu
        duongDan,
        // Chỉ lấy tên file và tìm trong thư mục nhạc
        musicDir + "/" + fs::path(duongDan).filename().string()
    };
    for (const std::string& duongDanThu : danhSachDuongDan){
        // Kiểm tra file có tồn tại hay không
        if (fs::exists(fs::path(duongDanThu), loi)) {
            return duongDanThu;
        }
        // Nếu chưa tìm thấy thì thử thêm đuôi .mp3
        if (fs::exists(fs::path(duongDanThu + ".mp3"), loi)) {
            return duongDanThu + ".mp3";
        }
    }
    // Lấy tên file không bao gồm phần mở rộng
    std::string tenFile = fs::path(duongDan).stem().string();
    // Chuyển tên file về chữ thường
    std::string tenFileThuong = textutil::lower(tenFile);
    // Quét các file trong thư mục nhạc
    for (const auto& file : fs::directory_iterator(musicDir, loi)){
        // Lấy tên file không có phần mở rộng
        std::string tenFileTrongThuMuc = file.path().stem().string();
        // Chuyển về chữ thường
        tenFileTrongThuMuc = textutil::lower(tenFileTrongThuMuc);
        // So sánh tên file
        if (tenFileTrongThuMuc == tenFileThuong){
            // Trả về đường dẫn thực tế
            return file.path().string();
        }
    }
    // Không tìm thấy file nhạc
    return "";
}
std::string SongDAO::importAudio(const std::string& sourcePath) const {
    namespace fs = std::filesystem;
    std::error_code loi;
    // Đường dẫn file nhạc ban đầu
    fs::path fileNguon(sourcePath);
    // Kiểm tra file nguồn có tồn tại không
    if (!fs::exists(fileNguon, loi)) {
        return "";
    }
    // Tạo thư mục chứa nhạc nếu chưa tồn tại
    fs::create_directories(musicDir, loi);
    // Lấy tên file và tạo đường dẫn đích
    fs::path fileDich = fs::path(musicDir) / fileNguon.filename();
    // Kiểm tra file đích đã tồn tại chưa
    if (!fs::exists(fileDich, loi)) {
        // Sao chép file nhạc vào thư mục dự án
        fs::copy_file(fileNguon, fileDich, loi);
        // Nếu sao chép thất bại thì trả về chuỗi rỗng
        if (loi) {
            return "";
        }
    }
    // Trả về đường dẫn tương đối để lưu vào database
    return "assets/music/" + fileNguon.filename().string();
}