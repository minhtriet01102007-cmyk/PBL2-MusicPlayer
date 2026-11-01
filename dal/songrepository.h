#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "src/song.h"
#include "dsa/hashtable/hashtable.h"
#include "textutil.h"

// GIAO DIỆN QUẢN LÝ KHO BÀI HÁT
class ISongRepository{
    public:
        virtual ~ISongRepository() {}
        virtual void load() = 0;                                    // Nạp dữ liệu từ file lên RAM
        virtual void save() = 0;                                    // Ghi dữ liệu từ RAM xuống file
        virtual const std::vector<Song*>& getAll() const = 0;       // Lấy toàn bộ danh sách bài hát
        virtual Song* findById(const std::string& id) const = 0;    // Tìm bài hát theo ID
        virtual void add(Song* song) = 0;                           // Thêm bài hát mới vào kho
        virtual bool remove(const std::string& id) = 0;             // Xóa bài hát theo ID và giải phóng RAM
        virtual std::string generateId() const = 0;                 // Sinh tự động ID tiếp theo (không trùng)
        virtual std::string resolveAudioPath(const Song& song) const = 0; // Tìm đường dẫn file nhạc thực tế trên máy
        virtual std::string importAudio(const std::string& sourcePath) = 0; // Chép file nhạc vào thư mục chung
};

class TextSongRepository : public ISongRepository{
    private:
        std::string root;     // Thư mục gốc của dự án (Project Root)
        std::string musicDir; // Thư mục chứa các file nhạc: root/assets/music
        std::string filePath; // Đường dẫn file lưu trữ dữ liệu bài hát: root/data/songs.txt
        char delim = '|';     // Ký tự phân cách cột trong file text (mặc định là '|')
        std::vector<Song*> songs;          // Danh sách lưu toàn bộ con trỏ bài hát (để duyệt)
        HashTable<std::string, Song*> byId; // Bảng băm giúp tra cứu bài hát siêu nhanh theo ID (O(1))
    public:
        explicit TextSongRepository(const std::string& rootPath);  // Constructor khởi tạo đường dẫn
        ~TextSongRepository() override;
        TextSongRepository(const TextSongRepository&) = delete;  // Chặn tính năng copy để tránh lỗi trỏ trùng vùng nhớ
        TextSongRepository& operator=(const TextSongRepository&) = delete;
        void load() override;
        void save() override;
        const std::vector<Song*>& getAll() const override;
        Song* findById(const std::string& id) const override;
        void add(Song* song) override;
        bool remove(const std::string& id) override;
        std::string generateId() const override;
        std::string resolveAudioPath(const Song& song) const override;
        std::string importAudio(const std::string& sourcePath) override;
};

TextSongRepository::TextSongRepository(const std::string& rootPath) : root(rootPath),  musicDir(rootPath + "/assets/music"),
      filePath(rootPath + "/data/songs.txt"){
}
TextSongRepository::~TextSongRepository(){
    for (Song* s : songs){
        delete s; // Xóa từng đối tượng Song 
    }
}
void TextSongRepository::load(){
    using namespace textutil;
    for (Song* s : songs) delete s;    // Xóa dữ liệu cũ (nếu có) trước khi load mới
    songs.clear();
    byId.clear();
    // Đọc tất cả các dòng hợp lệ từ file (bỏ qua dòng trống và dòng chú thích #)
    auto lines = readLines(filePath);
    if (!lines.empty()){
        delim = detectDelim(lines[0]); // Tự động nhận diện dấu phân cách ở dòng đầu tiên (VD: |, ,, \t)
    }
    for (const auto& l : lines){     // Duyệt qua từng dòng để tạo đối tượng Song
        auto p = split(l, delim);    // Cắt dòng thành các cột dữ liệu
        if (p.size() < 9) continue;  // Nếu dòng thiếu cột dữ liệu thì bỏ qua
        // Thứ tự: p[0]=id, p[1]=name, p[2]=artist, p[3]=album, p[4]=type, p[5]=duration, p[6]=date, p[8]=filePath, p[7]=plays
        Song* s = new Song(p[0], p[1], p[2], p[3], p[4], toInt(p[5]), p[6], p[8], 0, toLL(p[7]));
        songs.push_back(s);             // Lưu vào danh sách vector
        byId.insert(s->getIdSong(), s); // Lưu vào bảng băm để tra cứu nhanh bằng ID
    }
}
void TextSongRepository::save(){
    std::ofstream out(filePath);
    const std::string separator = (delim == '|') ? " | " : std::string(1, delim);
    for (Song* s : songs){
        out << s->getIdSong()     << separator 
            << s->getName()       << separator 
            << s->getArtist()     << separator 
            << s->getAlbum()      << separator
            << s->getTypeSong()   << separator 
            << s->getDuration()   << separator 
            << s->getDateRelease() << separator
            << s->getPlays()      << separator 
            << s->getFilePath()   << std::endl;
    }
}
const std::vector<Song*>& TextSongRepository::getAll() const{ // Lấy danh sách bài hát 
    return songs;
}
Song* TextSongRepository::findById(const std::string& id) const{
    Song* s = nullptr;
    return byId.search(id, s) ? s : nullptr; // Trả về con trỏ Song nếu tìm thấy, ngược lại trả về nullptr
}
void TextSongRepository::add(Song* song){
    if (song == nullptr) return;
    songs.push_back(song);
    byId.insert(song->getIdSong(), song);
}
bool TextSongRepository::remove(const std::string& id){
    Song* s = findById(id);
    if (s == nullptr) return false; // Không tìm thấy bài hát để xóa
    byId.remove(id); 
    songs.erase(std::remove(songs.begin(), songs.end(), s), songs.end());
    delete s; 
    return true;
}
std::string TextSongRepository::generateId() const{ // Sinh mã ID tự động tiếp theo 
    std::vector<std::string> ids;
    for (Song* s : songs){
        ids.push_back(s->getIdSong());
    }
    return textutil::nextId(ids, "S"); // Gọi hàm tiện ích sinh ID với tiền tố "S"
}
std::string TextSongRepository::resolveAudioPath(const Song& song) const{
    namespace fs = std::filesystem;
    std::string p = song.getFilePath();
    if (p.empty()) return "";
    std::replace(p.begin(), p.end(), '\\', '/');
    std::error_code ec;
    std::vector<std::string> possiblePaths = {   // Danh sách các khả năng đường dẫn có thể xảy ra để dò tìm
        root + "/" + p,                          // Đường dẫn tuyệt đối tính từ gốc dự án
        musicDir + "/" + p,                      // Nằm trực tiếp trong thư mục nhạc
        p,                                       // Đường dẫn nguyên bản
        musicDir + "/" + fs::path(p).filename().string() // Chỉ lấy tên file rọi vào thư mục nhạc chung
    };
    // Kiểm tra xem file có thực sự tồn tại không (thử cả trường hợp có hoặc không có đuôi .mp3)
    for (const auto& basePath : possiblePaths) {
        if (fs::exists(fs::path(basePath), ec)) return basePath;
        if (fs::exists(fs::path(basePath + ".mp3"), ec)) return basePath + ".mp3";
    }
    // Nếu vẫn không thấy, tiến hành quét mờ (không phân biệt chữ hoa/thường) trong thư mục nhạc
    std::string targetName = textutil::lower(fs::path(p).stem().string());
    for (const auto& entry : fs::directory_iterator(musicDir, ec)){
        if (textutil::lower(entry.path().stem().string()) == targetName){
            return entry.path().string(); // Trả về đường dẫn của file tìm thấy
        }
    }
    return ""; // Không tìm thấy file nhạc hợp lệ
}
std::string TextSongRepository::importAudio(const std::string& sourcePath){ // Chép file nhạc từ máy người dùng vào thư mục chung của ứng dụng 
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path source(sourcePath);
    if (!fs::exists(source, ec)) return ""; // Nếu file nguồn không tồn tại thì hủy
    // Tự động tạo thư mục chứa nhạc (musicDir) nếu trước đó chưa có
    fs::create_directories(musicDir, ec);
    // Xác định đường dẫn đích trong thư mục ứng dụng
    fs::path destination = fs::path(musicDir) / source.filename();
    // Nếu file chưa tồn tại trong thư mục chung thì tiến hành copy
    if (!fs::exists(destination, ec)) {
        fs::copy_file(source, destination, ec);
        if (ec) return ""; // Trả về chuỗi rỗng nếu copy lỗi
    }
    // Trả về đường dẫn tương đối để lưu vào cơ sở dữ liệu
    return "assets/music/" + source.filename().string();
}