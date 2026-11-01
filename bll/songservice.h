#pragma once
#include <functional>
#include <string>
#include <vector>
#include "songinfo.h"
#include "mapper.h"
#include "authservice.h"
#include "dal/songrepository.h"
#include "dal/textutil.h"
#include "dsa/maxheap/maxheap.h"

// So sánh cho MaxHeap: bài nhiều lượt nghe hơn thì ưu tiên cao hơn
struct ByPlayCount{
    bool operator()(Song* a, Song* b) const{
        return a->getPlays() > b->getPlays();
    }
};

// Nghiệp vụ bài hát: tìm kiếm, bảng xếp hạng, thêm/xóa (chỉ Admin)
class SongService{
    private:
        ISongRepository& songs;
        AuthService& auth;
        std::function<void(Song*)> beforeRemove;    // các tầng khác dọn tham chiếu tới bài trước khi xóa
    public:
        SongService(ISongRepository& songs, AuthService& auth);
        void setBeforeRemoveHook(const std::function<void(Song*)>& hook);
        std::vector<SongInfo> getAll() const;
        std::vector<SongInfo> search(const std::string& keyword) const;
        std::vector<SongInfo> top(int n) const;                 // dùng MaxHeap
        bool addSong(const std::string& name, const std::string& artist, const std::string& album,
                     const std::string& type, int durationSec, const std::string& dateRelease,
                     const std::string& sourceFilePath);
        bool removeSong(const std::string& id);
};

SongService::SongService(ISongRepository& songs, AuthService& auth) : songs(songs), auth(auth){
}
void SongService::setBeforeRemoveHook(const std::function<void(Song*)>& hook){
    this->beforeRemove = hook;
}
std::vector<SongInfo> SongService::getAll() const{
    std::vector<SongInfo> v;
    for (Song* s : this->songs.getAll()) v.push_back(toSongInfo(*s, this->songs));
    return v;
}
std::vector<SongInfo> SongService::search(const std::string& keyword) const{
    std::vector<SongInfo> v;
    std::string k = textutil::lower(keyword);
    for (Song* s : this->songs.getAll())
        if (textutil::lower(s->getName()).find(k) != std::string::npos ||
            textutil::lower(s->getArtist()).find(k) != std::string::npos ||
            textutil::lower(s->getAlbum()).find(k) != std::string::npos)
            v.push_back(toSongInfo(*s, this->songs));
    return v;
}
std::vector<SongInfo> SongService::top(int n) const{
    MaxHeap<Song*, ByPlayCount> heap;
    for (Song* s : this->songs.getAll()) heap.insert(s);
    std::vector<SongInfo> v;
    for (int i = 0; i < n && !heap.isEmpty(); ++i) v.push_back(toSongInfo(*heap.extractMax(), this->songs));
    return v;
}
bool SongService::addSong(const std::string& name, const std::string& artist, const std::string& album,
                          const std::string& type, int durationSec, const std::string& dateRelease,
                          const std::string& sourceFilePath){
    using textutil::trim;
    if (!this->auth.isAdmin()) return false;                    // quy tắc nghiệp vụ: chỉ Admin
    if (trim(name).empty() || sourceFilePath.empty()) return false;
    std::string stored = this->songs.importAudio(sourceFilePath);
    if (stored.empty()) return false;
    Song* s = new Song(this->songs.generateId(), trim(name), trim(artist), trim(album), trim(type),
                       durationSec, trim(dateRelease), stored, 0, 0);
    this->songs.add(s);
    this->songs.save();
    return true;
}
bool SongService::removeSong(const std::string& id){
    if (!this->auth.isAdmin()) return false;
    Song* s = this->songs.findById(id);
    if (s == nullptr) return false;
    if (this->beforeRemove) this->beforeRemove(s);              // gỡ bài khỏi playlist và hàng phát
    this->songs.remove(id);
    this->songs.save();
    return true;
}
