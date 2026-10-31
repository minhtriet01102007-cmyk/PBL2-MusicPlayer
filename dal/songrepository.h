#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "src/song.h"
#include "dsa/hashtable/hashtable.h"
#include "textutil.h"

class ISongRepository{
    public:
        virtual ~ISongRepository(){}
        virtual void load() = 0;
        virtual void save() = 0;
        virtual const std::vector<Song*>& getAll() const = 0;
        virtual Song* findById(const std::string& id) const = 0;
        virtual void add(Song* song) = 0;                                   // kho nhận quyền sở hữu Song
        virtual bool remove(const std::string& id) = 0;                     // xóa và giải phóng Song
        virtual std::string generateId() const = 0;                         // id mới không trùng
        virtual std::string resolveAudioPath(const Song& song) const = 0;   // đường dẫn file nhạc thật ("" nếu không có)
        virtual std::string importAudio(const std::string& sourcePath) = 0; // chép file nhạc vào thư mục nhạc, trả về đường dẫn lưu
};

class TextSongRepository : public ISongRepository{
    private:
        std::string root, musicDir, filePath;
        char delim = '|';
        std::vector<Song*> songs;
        HashTable<std::string, Song*> byId;     // tra bài hát theo id
    public:
        explicit TextSongRepository(const std::string& root);
        ~TextSongRepository() override;
        TextSongRepository(const TextSongRepository&) = delete;
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

TextSongRepository::TextSongRepository(const std::string& root){
    this->root = root;
    this->musicDir = root + "/assets/music";
    this->filePath = root + "/data/songs.txt";
}
TextSongRepository::~TextSongRepository(){
    for (Song* s : this->songs) delete s;
}
void TextSongRepository::load(){
    using namespace textutil;
    for (Song* s : this->songs) delete s;
    this->songs.clear();
    this->byId.clear();
    auto lines = readLines(this->filePath);
    if (!lines.empty()) this->delim = detectDelim(lines[0]);
    for (const auto& l : lines){
        auto p = split(l, this->delim);
        if (p.size() < 9) continue;
        Song* s = new Song(p[0], p[1], p[2], p[3], p[4], toInt(p[5]), p[6], p[8], 0, toLL(p[7]));
        this->songs.push_back(s);
        this->byId.insert(s->getIdSong(), s);
    }
}
void TextSongRepository::save(){
    std::ofstream out(this->filePath);
    const std::string d = (this->delim == '|') ? " | " : std::string(1, this->delim);
    for (Song* s : this->songs)
        out << s->getIdSong() << d << s->getName() << d << s->getArtist() << d << s->getAlbum() << d
            << s->getTypeSong() << d << s->getDuration() << d << s->getDateRelease() << d
            << s->getPlays() << d << s->getFilePath() << '\n';
}
const std::vector<Song*>& TextSongRepository::getAll() const{
    return this->songs;
}
Song* TextSongRepository::findById(const std::string& id) const{
    Song* s = nullptr;
    return this->byId.search(id, s) ? s : nullptr;
}
void TextSongRepository::add(Song* song){
    if (song == nullptr) return;
    this->songs.push_back(song);
    this->byId.insert(song->getIdSong(), song);
}
bool TextSongRepository::remove(const std::string& id){
    Song* s = this->findById(id);
    if (s == nullptr) return false;
    this->byId.remove(id);
    this->songs.erase(std::remove(this->songs.begin(), this->songs.end(), s), this->songs.end());
    delete s;
    return true;
}
std::string TextSongRepository::generateId() const{
    std::vector<std::string> ids;
    for (Song* s : this->songs) ids.push_back(s->getIdSong());
    return textutil::nextId(ids, "S");
}
std::string TextSongRepository::resolveAudioPath(const Song& song) const{
    namespace fs = std::filesystem;
    std::string p = song.getFilePath();
    if (p.empty()) return "";
    std::replace(p.begin(), p.end(), '\\', '/');
    std::error_code ec;
    std::vector<std::string> bases = { this->root + "/" + p, this->musicDir + "/" + p, p,
                                       this->musicDir + "/" + fs::path(p).filename().string() };
    for (const auto& b : bases){
        if (fs::exists(fs::path(b), ec)) return b;
        if (fs::exists(fs::path(b + ".mp3"), ec)) return b + ".mp3";
    }
    std::string want = textutil::lower(fs::path(p).stem().string());
    for (const auto& e : fs::directory_iterator(this->musicDir, ec))
        if (textutil::lower(e.path().stem().string()) == want) return e.path().string();
    return "";
}
std::string TextSongRepository::importAudio(const std::string& sourcePath){
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path from(sourcePath);
    if (!fs::exists(from, ec)) return "";
    fs::create_directories(this->musicDir, ec);
    fs::path to = fs::path(this->musicDir) / from.filename();
    if (!fs::exists(to, ec)){
        fs::copy_file(from, to, ec);
        if (ec) return "";
    }
    return "assets/music/" + from.filename().string();
}
