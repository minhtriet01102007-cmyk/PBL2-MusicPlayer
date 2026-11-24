#pragma once
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include "songinfo.h"
#include "artistinfo.h"
#include "albuminfo.h"
#include "authservice.h"
#include "src/song.h"
#include "src/artist.h"
#include "src/album.h"
#include "dal/songdao.h"
#include "dal/artistdao.h"
#include "dal/albumdao.h"
#include "dal/textutil.h"
#include "dsa/hashtable/hashtable.h"

// Nghiệp vụ danh mục nhạc: bài hát, nghệ sĩ, album (tìm kiếm, bảng xếp hạng, thêm/sửa/xóa dành cho Admin).
// Giữ dữ liệu trong bộ nhớ (vector + HashTable tra theo id), chỉ gọi DAO để đọc/ghi file.
// Quy ước: các hàm trả về std::string là thông báo lỗi, chuỗi rỗng nghĩa là thành công.
class SongService{
    private:
        SongDAO songDao;
        ArtistDAO artistDao;
        AlbumDAO albumDao;
        AuthService& auth;
        std::string logPath;            // data/repair_log.txt: ghi lại những gì repairLinks đã sửa
        std::vector<Song*> songs;       // SongService sở hữu và delete
        std::vector<Artist*> artists;
        std::vector<Album*> albums;
        HashTable<std::string, Song*> songById;
        HashTable<std::string, Artist*> artistById;
        HashTable<std::string, Album*> albumById;
        static bool containsText(const std::string& text, const std::string& keywordKey);   // keywordKey = textutil::foldKey(...)
        static bool isValidDate(const std::string& date);   // yyyy-mm-dd hoặc rỗng
        static std::string noPermission();
        bool repairLinks();   // sửa dữ liệu cũ: đổi tên nghệ sĩ/album thành id, đồng bộ tracklist; true nếu có thay đổi
    public:
        SongService(const std::string& root, AuthService& auth);
        ~SongService();
        SongService(const SongService&) = delete;
        SongService& operator=(const SongService&) = delete;
        void load();

        // Tra cứu theo id
        Song* findSong(const std::string& id) const;
        Artist* findArtist(const std::string& id) const;
        Album* findAlbum(const std::string& id) const;

        // Đổi sang DTO cho UI
        SongInfo toInfo(const Song& s, bool withPath = false) const;   // withPath = true mới dò file nhạc thật trên đĩa
        ArtistInfo toInfo(const Artist& a) const;
        AlbumInfo toInfo(const Album& a) const;
        std::vector<SongInfo> toInfos(const std::vector<std::string>& songIds) const; // giữ thứ tự, bỏ id không tồn tại

        // Danh sách và tìm kiếm
        std::vector<SongInfo> getAllSongs() const;
        std::vector<ArtistInfo> getAllArtists() const;
        std::vector<AlbumInfo> getAllAlbums() const;
        std::vector<SongInfo> searchSongs(const std::string& keyword) const;
        std::vector<ArtistInfo> searchArtists(const std::string& keyword) const;
        std::vector<AlbumInfo> searchAlbums(const std::string& keyword) const;
        std::vector<SongInfo> topSongs(int n) const;
        std::vector<ArtistInfo> topArtists(int n) const;
        std::vector<SongInfo> newestSongs(int n) const;
        std::vector<std::string> genres() const;
        std::vector<SongInfo> songsByGenre(const std::string& genre) const;
        std::vector<SongInfo> songsOfArtist(const std::string& idArtist) const;
        std::vector<SongInfo> songsOfAlbum(const std::string& idAlbum) const;
        std::vector<AlbumInfo> albumsOfArtist(const std::string& idArtist) const;

        // Chỉ Admin
        std::string addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
                            int durationSec, const std::string& dateRelease, const std::string& sourceFilePath);
        std::string updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                               const std::string& type, int durationSec, const std::string& dateRelease);
        std::string removeSong(const std::string& id);     // chỉ xóa trong danh mục, việc dọn playlist/người dùng do MusicSystem làm
        std::string addArtist(const std::string& name, const std::string& country);
        std::string removeArtist(const std::string& id);   // từ chối nếu nghệ sĩ còn bài hát hoặc album
        std::string addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease);
        std::string removeAlbum(const std::string& id);    // các bài trong album được gỡ khỏi album, không bị xóa
        std::string setAlbumSongs(const std::string& idAlbum, const std::vector<std::string>& songIds); // đặt toàn bộ tracklist (theo thứ tự)

        // Ghi file
        bool saveSongs();
        bool saveArtists();
        bool saveAlbums();
        void increasePlays(const std::string& idSong);     // +1 lượt nghe rồi ghi file

        int songCount() const;
        int artistCount() const;
        int albumCount() const;
        long long totalPlays() const;
};

SongService::SongService(const std::string& root, AuthService& auth)
    : songDao(root), artistDao(root), albumDao(root), auth(auth), logPath(root + "/data/repair_log.txt"){
}
SongService::~SongService(){
    for (Song* s : this->songs) delete s;
    for (Artist* a : this->artists) delete a;
    for (Album* al : this->albums) delete al;
}
bool SongService::containsText(const std::string& text, const std::string& keywordKey){
    return textutil::foldKey(text).find(keywordKey) != std::string::npos;   // tìm không phân biệt dấu, hoa thường
}
bool SongService::isValidDate(const std::string& date){
    if (date.empty()) return true;
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (size_t i = 0; i < date.size(); ++i){
        if (i == 4 || i == 7) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}
std::string SongService::noPermission(){
    return "Chỉ Admin mới có quyền thực hiện thao tác này";
}

void SongService::load(){
    for (Song* s : this->songs) delete s;
    for (Artist* a : this->artists) delete a;
    for (Album* al : this->albums) delete al;
    this->songs.clear(); this->artists.clear(); this->albums.clear();
    this->songById.clear(); this->artistById.clear(); this->albumById.clear();
    for (Song* s : this->songDao.loadAll()){
        if (this->songById.contains(s->getIdSong())){ delete s; continue; }
        this->songs.push_back(s);
        this->songById.insert(s->getIdSong(), s);
    }
    for (Artist* a : this->artistDao.loadAll()){
        if (this->artistById.contains(a->getIdArtist())){ delete a; continue; }
        this->artists.push_back(a);
        this->artistById.insert(a->getIdArtist(), a);
    }
    for (Album* al : this->albumDao.loadAll()){
        if (this->albumById.contains(al->getIdAlbum())){ delete al; continue; }
        this->albums.push_back(al);
        this->albumById.insert(al->getIdAlbum(), al);
    }
    if (this->repairLinks()){   // dữ liệu cũ (lưu tên thay vì id) được tự động sửa và ghi lại
        this->saveSongs();
        this->saveArtists();
        this->saveAlbums();
    }
}
bool SongService::repairLinks(){
    bool changed = false;
    std::vector<std::string> log;
    auto key = [](const std::string& s){ return textutil::foldKey(s); };
    auto artistByName = [this, &key](const std::string& name) -> Artist*{
        for (Artist* a : this->artists){
            if (key(a->getNameArtist()) == key(name)) return a;
        }
        return nullptr;
    };
    auto albumByName = [this, &key](const std::string& name) -> Album*{
        for (Album* al : this->albums){
            if (key(al->getNameAlbum()) == key(name)) return al;
        }
        return nullptr;
    };
    auto createArtist = [this](const std::string& name) -> Artist*{
        std::vector<std::string> ids;
        for (Artist* a : this->artists) ids.push_back(a->getIdArtist());
        Artist* a = new Artist(textutil::nextId(ids, "A"), textutil::trim(name), "", 0);
        this->artists.push_back(a);
        this->artistById.insert(a->getIdArtist(), a);
        return a;
    };
    auto createAlbum = [this](const std::string& name, const std::string& idArtist, const std::string& date) -> Album*{
        std::vector<std::string> ids;
        for (Album* al : this->albums) ids.push_back(al->getIdAlbum());
        Album* al = new Album(textutil::nextId(ids, "AL"), textutil::trim(name), idArtist, date);
        this->albums.push_back(al);
        this->albumById.insert(al->getIdAlbum(), al);
        return al;
    };

    // 0) Gộp các nghệ sĩ trùng tên (giữ bản nằm trước trong artists.txt)
    for (size_t i = 0; i < this->artists.size(); ++i){
        for (size_t j = i + 1; j < this->artists.size();){
            Artist* keep = this->artists[i];
            Artist* dup = this->artists[j];
            if (key(keep->getNameArtist()).empty() || key(keep->getNameArtist()) != key(dup->getNameArtist())){
                ++j;
                continue;
            }
            for (Song* s : this->songs){
                if (s->getIdArtist() == dup->getIdArtist()) s->setIdArtist(keep->getIdArtist());
            }
            for (Album* al : this->albums){
                if (al->getIdArtist() == dup->getIdArtist()) al->setIdArtist(keep->getIdArtist());
            }
            if (dup->getFollowers() > keep->getFollowers()) keep->setFollowers(dup->getFollowers());
            if (keep->getCountry().empty()) keep->setCountry(dup->getCountry());
            log.push_back("Gộp nghệ sĩ trùng " + dup->getIdArtist() + " \"" + dup->getNameArtist() + "\" vào " + keep->getIdArtist());
            this->artistById.remove(dup->getIdArtist());
            this->artists.erase(this->artists.begin() + j);
            delete dup;
            changed = true;
        }
    }
    // 0b) Gộp các album trùng tên và cùng nghệ sĩ
    for (size_t i = 0; i < this->albums.size(); ++i){
        for (size_t j = i + 1; j < this->albums.size();){
            Album* keep = this->albums[i];
            Album* dup = this->albums[j];
            bool sameName = !key(keep->getNameAlbum()).empty() && key(keep->getNameAlbum()) == key(dup->getNameAlbum());
            bool sameArtist = keep->getIdArtist() == dup->getIdArtist() || this->findArtist(keep->getIdArtist()) == nullptr || this->findArtist(dup->getIdArtist()) == nullptr;
            if (!sameName || !sameArtist){
                ++j;
                continue;
            }
            for (Song* s : this->songs){
                if (s->getIdAlbum() == dup->getIdAlbum()) s->setIdAlbum(keep->getIdAlbum());
            }
            std::vector<std::string> ids = dup->getSongIds();
            for (const std::string& sid : ids) keep->addSong(sid);
            log.push_back("Gộp album trùng " + dup->getIdAlbum() + " \"" + dup->getNameAlbum() + "\" vào " + keep->getIdAlbum());
            this->albumById.remove(dup->getIdAlbum());
            this->albums.erase(this->albums.begin() + j);
            delete dup;
            changed = true;
        }
    }
    // 1) Bài hát: cột nghệ sĩ / album đang là tên -> đổi thành id (không có thì tạo mới)
    for (Song* s : this->songs){
        std::string rawArtist = textutil::trim(s->getIdArtist());
        if (!rawArtist.empty() && this->findArtist(rawArtist) == nullptr){
            Artist* a = artistByName(rawArtist);
            if (a == nullptr){
                a = createArtist(rawArtist);
                log.push_back("Tạo nghệ sĩ mới " + a->getIdArtist() + " \"" + a->getNameArtist() + "\" (từ bài " + s->getIdSong() + ")");
            }
            log.push_back("Bài " + s->getIdSong() + ": nghệ sĩ \"" + rawArtist + "\" -> " + a->getIdArtist());
            s->setIdArtist(a->getIdArtist());
            changed = true;
        }
        std::string rawAlbum = textutil::trim(s->getIdAlbum());
        if (!rawAlbum.empty() && this->findAlbum(rawAlbum) == nullptr){
            Album* al = albumByName(rawAlbum);
            if (al == nullptr){
                al = createAlbum(rawAlbum, s->getIdArtist(), s->getDateRelease());
                log.push_back("Tạo album mới " + al->getIdAlbum() + " \"" + al->getNameAlbum() + "\" (từ bài " + s->getIdSong() + ")");
            }
            log.push_back("Bài " + s->getIdSong() + ": album \"" + rawAlbum + "\" -> " + al->getIdAlbum());
            s->setIdAlbum(al->getIdAlbum());
            changed = true;
        }
    }
    // 2) Album chưa có nghệ sĩ hợp lệ: đổi tên thành id, hoặc lấy theo nghệ sĩ của bài hát trong album
    for (Album* al : this->albums){
        if (this->findArtist(al->getIdArtist()) != nullptr) continue;
        std::string raw = textutil::trim(al->getIdArtist());
        Artist* a = raw.empty() ? nullptr : artistByName(raw);
        if (a == nullptr){
            for (Song* s : this->songs){
                if (s->getIdAlbum() == al->getIdAlbum() && this->findArtist(s->getIdArtist()) != nullptr){
                    a = this->findArtist(s->getIdArtist());
                    break;
                }
            }
        }
        if (a == nullptr && !raw.empty()){
            a = createArtist(raw);
            log.push_back("Tạo nghệ sĩ mới " + a->getIdArtist() + " \"" + a->getNameArtist() + "\" (từ album " + al->getIdAlbum() + ")");
        }
        if (a != nullptr){
            log.push_back("Album " + al->getIdAlbum() + ": nghệ sĩ \"" + raw + "\" -> " + a->getIdArtist());
            al->setIdArtist(a->getIdArtist());
            changed = true;
        }
    }
    // 3) Tracklist của album phải khớp với Song::id_album
    for (Album* al : this->albums){
        std::vector<std::string> bad;
        for (const std::string& sid : al->getSongIds()){
            Song* s = this->findSong(sid);
            if (s == nullptr){
                bad.push_back(sid);                                  // bài không tồn tại
            } else if (s->getIdAlbum().empty()){
                s->setIdAlbum(al->getIdAlbum());                     // bài chưa ghi album nhưng nằm trong tracklist
                log.push_back("Bài " + sid + ": gán vào album " + al->getIdAlbum() + " theo tracklist");
                changed = true;
            } else if (s->getIdAlbum() != al->getIdAlbum()){
                bad.push_back(sid);                                  // bài thuộc album khác
            }
        }
        for (const std::string& sid : bad){
            al->removeSong(sid);
            log.push_back("Album " + al->getIdAlbum() + ": bỏ " + sid + " khỏi tracklist");
            changed = true;
        }
    }
    for (Song* s : this->songs){
        Album* al = this->findAlbum(s->getIdAlbum());
        if (al != nullptr && al->addSong(s->getIdSong())) changed = true;   // bài có album nhưng chưa nằm trong tracklist
    }
    if (!log.empty()){
        std::ofstream out(this->logPath, std::ios::app);
        for (const std::string& line : log) out << line << '\n';
    }
    return changed;
}

Song* SongService::findSong(const std::string& id) const{
    Song* s = nullptr;
    return this->songById.search(id, s) ? s : nullptr;
}
Artist* SongService::findArtist(const std::string& id) const{
    Artist* a = nullptr;
    return this->artistById.search(id, a) ? a : nullptr;
}
Album* SongService::findAlbum(const std::string& id) const{
    Album* al = nullptr;
    return this->albumById.search(id, al) ? al : nullptr;
}

SongInfo SongService::toInfo(const Song& s, bool withPath) const{
    SongInfo i;
    i.id = s.getIdSong();
    i.name = s.getName();
    i.idArtist = s.getIdArtist();
    i.idAlbum = s.getIdAlbum();
    Artist* a = this->findArtist(i.idArtist);
    i.artist = (a != nullptr) ? a->getNameArtist() : "Không rõ";
    Album* al = this->findAlbum(i.idAlbum);
    i.album = (al != nullptr) ? al->getNameAlbum() : "";
    i.type = s.getTypeSong();
    i.date = s.getDateRelease();
    i.duration = s.getDuration();
    i.plays = s.getPlays();
    Listener* l = this->auth.currentListener();
    i.liked = (l != nullptr) && l->isLiked(i.id);
    if (withPath) i.path = this->songDao.resolveAudioPath(s);
    return i;
}
ArtistInfo SongService::toInfo(const Artist& a) const{
    ArtistInfo i;
    i.id = a.getIdArtist();
    i.name = a.getNameArtist();
    i.country = a.getCountry();
    i.followers = a.getFollowers();
    for (Song* s : this->songs){
        if (s->getIdArtist() == i.id) i.songCount++;
    }
    Listener* l = this->auth.currentListener();
    i.followed = (l != nullptr) && l->isFollowing(i.id);
    return i;
}
AlbumInfo SongService::toInfo(const Album& al) const{
    AlbumInfo i;
    i.id = al.getIdAlbum();
    i.name = al.getNameAlbum();
    i.idArtist = al.getIdArtist();
    Artist* a = this->findArtist(i.idArtist);
    i.artist = (a != nullptr) ? a->getNameArtist() : "Không rõ";
    i.date = al.getDateRelease();
    i.trackCount = al.getTrackCount();
    for (const auto& id : al.getSongIds()){
        Song* s = this->findSong(id);
        if (s != nullptr) i.totalSeconds += s->getDuration();
    }
    return i;
}
std::vector<SongInfo> SongService::toInfos(const std::vector<std::string>& songIds) const{
    std::vector<SongInfo> result;
    for (const auto& id : songIds){
        Song* s = this->findSong(id);
        if (s != nullptr) result.push_back(this->toInfo(*s));
    }
    return result;
}

std::vector<SongInfo> SongService::getAllSongs() const{
    std::vector<Song*> v = this->songs;
    std::sort(v.begin(), v.end(), [](Song* a, Song* b){ return textutil::lower(a->getName()) < textutil::lower(b->getName()); });
    std::vector<SongInfo> result;
    for (Song* s : v) result.push_back(this->toInfo(*s));
    return result;
}
std::vector<ArtistInfo> SongService::getAllArtists() const{
    std::vector<Artist*> v = this->artists;
    std::sort(v.begin(), v.end(), [](Artist* a, Artist* b){ return textutil::lower(a->getNameArtist()) < textutil::lower(b->getNameArtist()); });
    std::vector<ArtistInfo> result;
    for (Artist* a : v) result.push_back(this->toInfo(*a));
    return result;
}
std::vector<AlbumInfo> SongService::getAllAlbums() const{
    std::vector<Album*> v = this->albums;
    std::sort(v.begin(), v.end(), [](Album* a, Album* b){ return a->getDateRelease() > b->getDateRelease(); });
    std::vector<AlbumInfo> result;
    for (Album* al : v) result.push_back(this->toInfo(*al));
    return result;
}

std::vector<SongInfo> SongService::searchSongs(const std::string& keyword) const{
    std::vector<SongInfo> result;
    std::string k = textutil::foldKey(textutil::trim(keyword));
    if (k.empty()) return result;
    for (Song* s : this->songs){
        SongInfo i = this->toInfo(*s);
        if (containsText(i.name, k) || containsText(i.artist, k) || containsText(i.album, k) || containsText(i.type, k))
            result.push_back(i);
    }
    return result;
}
std::vector<ArtistInfo> SongService::searchArtists(const std::string& keyword) const{
    std::vector<ArtistInfo> result;
    std::string k = textutil::foldKey(textutil::trim(keyword));
    if (k.empty()) return result;
    for (Artist* a : this->artists){
        if (containsText(a->getNameArtist(), k) || containsText(a->getCountry(), k)) result.push_back(this->toInfo(*a));
    }
    return result;
}
std::vector<AlbumInfo> SongService::searchAlbums(const std::string& keyword) const{
    std::vector<AlbumInfo> result;
    std::string k = textutil::foldKey(textutil::trim(keyword));
    if (k.empty()) return result;
    for (Album* al : this->albums){
        AlbumInfo i = this->toInfo(*al);
        if (containsText(i.name, k) || containsText(i.artist, k)) result.push_back(i);
    }
    return result;
}

std::vector<SongInfo> SongService::topSongs(int n) const{
    std::vector<SongInfo> result;
    if (n <= 0) return result;
    std::vector<Song*> v = this->songs;
    int k = std::min(n, (int)v.size());
    std::partial_sort(v.begin(), v.begin() + k, v.end(), SongPlaysComparator()); // nhiều lượt nghe nhất đứng đầu
    for (int i = 0; i < k; ++i) result.push_back(this->toInfo(*v[i]));
    return result;
}
std::vector<ArtistInfo> SongService::topArtists(int n) const{
    std::vector<ArtistInfo> result;
    if (n <= 0) return result;
    std::vector<Artist*> v = this->artists;
    int k = std::min(n, (int)v.size());
    std::partial_sort(v.begin(), v.begin() + k, v.end(), ArtistFollowersComparator());
    for (int i = 0; i < k; ++i) result.push_back(this->toInfo(*v[i]));
    return result;
}
std::vector<SongInfo> SongService::newestSongs(int n) const{
    std::vector<SongInfo> result;
    if (n <= 0) return result;
    std::vector<Song*> v = this->songs;
    int k = std::min(n, (int)v.size());
    std::partial_sort(v.begin(), v.begin() + k, v.end(), [](Song* a, Song* b){ return a->getDateRelease() > b->getDateRelease(); });
    for (int i = 0; i < k; ++i) result.push_back(this->toInfo(*v[i]));
    return result;
}
std::vector<std::string> SongService::genres() const{
    std::vector<std::string> result;
    for (Song* s : this->songs){
        if (!s->getTypeSong().empty()) result.push_back(s->getTypeSong());
    }
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}
std::vector<SongInfo> SongService::songsByGenre(const std::string& genre) const{
    std::vector<Song*> v;
    std::string g = textutil::lower(genre);
    for (Song* s : this->songs){
        if (textutil::lower(s->getTypeSong()) == g) v.push_back(s);
    }
    std::sort(v.begin(), v.end(), SongPlaysComparator());
    std::vector<SongInfo> result;
    for (Song* s : v) result.push_back(this->toInfo(*s));
    return result;
}
std::vector<SongInfo> SongService::songsOfArtist(const std::string& idArtist) const{
    std::vector<Song*> v;
    for (Song* s : this->songs){
        if (s->getIdArtist() == idArtist) v.push_back(s);
    }
    std::sort(v.begin(), v.end(), SongPlaysComparator());
    std::vector<SongInfo> result;
    for (Song* s : v) result.push_back(this->toInfo(*s));
    return result;
}
std::vector<SongInfo> SongService::songsOfAlbum(const std::string& idAlbum) const{
    Album* al = this->findAlbum(idAlbum);
    if (al == nullptr) return std::vector<SongInfo>();
    return this->toInfos(al->getSongIds()); // đúng thứ tự track
}
std::vector<AlbumInfo> SongService::albumsOfArtist(const std::string& idArtist) const{
    std::vector<Album*> v;
    for (Album* al : this->albums){
        if (al->getIdArtist() == idArtist) v.push_back(al);
    }
    std::sort(v.begin(), v.end(), [](Album* a, Album* b){ return a->getDateRelease() > b->getDateRelease(); });
    std::vector<AlbumInfo> result;
    for (Album* al : v) result.push_back(this->toInfo(*al));
    return result;
}

std::string SongService::addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
                                 int durationSec, const std::string& dateRelease, const std::string& sourceFilePath){
    if (!this->auth.isAdmin()) return noPermission();
    std::string n = textutil::trim(name), t = textutil::trim(type), d = textutil::trim(dateRelease);
    if (n.empty()) return "Tên bài hát không được để trống";
    if (!textutil::isSafeText(n) || !textutil::isSafeText(t)) return "Không được dùng ký tự '|' trong tên hoặc thể loại";
    if (this->findArtist(idArtist) == nullptr) return "Vui lòng chọn nghệ sĩ hợp lệ";
    if (!idAlbum.empty() && this->findAlbum(idAlbum) == nullptr) return "Album không tồn tại";
    if (durationSec <= 0) return "Thời lượng phải lớn hơn 0";
    if (!isValidDate(d)) return "Ngày phát hành phải có dạng yyyy-mm-dd";
    std::string stored = "";
    if (!sourceFilePath.empty()){
        stored = this->songDao.importAudio(sourceFilePath);
        if (stored.empty()) return "Không đọc hoặc sao chép được file nhạc";
    }
    std::vector<std::string> ids;
    for (Song* s : this->songs) ids.push_back(s->getIdSong());
    std::string id = textutil::nextId(ids, "S");
    Song* song = new Song(id, n, idArtist, idAlbum, t.empty() ? "Khác" : t, durationSec, d, stored, 0);
    this->songs.push_back(song);
    this->songById.insert(id, song);
    if (!idAlbum.empty()){
        this->findAlbum(idAlbum)->addSong(id);
        this->saveAlbums();
    }
    if (!this->saveSongs()) return "Không ghi được file songs.txt";
    return "";
}
std::string SongService::updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                                    const std::string& type, int durationSec, const std::string& dateRelease){
    if (!this->auth.isAdmin()) return noPermission();
    Song* s = this->findSong(id);
    if (s == nullptr) return "Không tìm thấy bài hát";
    std::string n = textutil::trim(name), t = textutil::trim(type), d = textutil::trim(dateRelease);
    if (n.empty()) return "Tên bài hát không được để trống";
    if (!textutil::isSafeText(n) || !textutil::isSafeText(t)) return "Không được dùng ký tự '|' trong tên hoặc thể loại";
    if (this->findArtist(idArtist) == nullptr) return "Vui lòng chọn nghệ sĩ hợp lệ";
    if (!idAlbum.empty() && this->findAlbum(idAlbum) == nullptr) return "Album không tồn tại";
    if (durationSec <= 0) return "Thời lượng phải lớn hơn 0";
    if (!isValidDate(d)) return "Ngày phát hành phải có dạng yyyy-mm-dd";
    if (s->getIdAlbum() != idAlbum){ // đổi album: cập nhật tracklist hai bên
        Album* oldAlbum = this->findAlbum(s->getIdAlbum());
        if (oldAlbum != nullptr) oldAlbum->removeSong(id);
        Album* newAlbum = this->findAlbum(idAlbum);
        if (newAlbum != nullptr) newAlbum->addSong(id);
        this->saveAlbums();
    }
    s->setName(n);
    s->setIdArtist(idArtist);
    s->setIdAlbum(idAlbum);
    s->setTypeSong(t.empty() ? "Khác" : t);
    s->setDuration(durationSec);
    s->setDateRelease(d);
    if (!this->saveSongs()) return "Không ghi được file songs.txt";
    return "";
}
std::string SongService::removeSong(const std::string& id){
    if (!this->auth.isAdmin()) return noPermission();
    Song* s = this->findSong(id);
    if (s == nullptr) return "Không tìm thấy bài hát";
    Album* al = this->findAlbum(s->getIdAlbum());
    if (al != nullptr){
        al->removeSong(id);
        this->saveAlbums();
    }
    this->songs.erase(std::remove(this->songs.begin(), this->songs.end(), s), this->songs.end());
    this->songById.remove(id);
    delete s;
    if (!this->saveSongs()) return "Không ghi được file songs.txt";
    return "";
}
std::string SongService::addArtist(const std::string& name, const std::string& country){
    if (!this->auth.isAdmin()) return noPermission();
    std::string n = textutil::trim(name), c = textutil::trim(country);
    if (n.empty()) return "Tên nghệ sĩ không được để trống";
    if (!textutil::isSafeText(n) || !textutil::isSafeText(c)) return "Không được dùng ký tự '|'";
    for (Artist* a : this->artists){
        if (textutil::lower(a->getNameArtist()) == textutil::lower(n)) return "Nghệ sĩ này đã tồn tại";
    }
    std::vector<std::string> ids;
    for (Artist* a : this->artists) ids.push_back(a->getIdArtist());
    std::string id = textutil::nextId(ids, "A");
    Artist* artist = new Artist(id, n, c, 0);
    this->artists.push_back(artist);
    this->artistById.insert(id, artist);
    if (!this->saveArtists()) return "Không ghi được file artists.txt";
    return "";
}
std::string SongService::removeArtist(const std::string& id){
    if (!this->auth.isAdmin()) return noPermission();
    Artist* a = this->findArtist(id);
    if (a == nullptr) return "Không tìm thấy nghệ sĩ";
    for (Song* s : this->songs){
        if (s->getIdArtist() == id) return "Nghệ sĩ còn bài hát, hãy xóa hoặc chuyển bài hát trước";
    }
    for (Album* al : this->albums){
        if (al->getIdArtist() == id) return "Nghệ sĩ còn album, hãy xóa album trước";
    }
    this->artists.erase(std::remove(this->artists.begin(), this->artists.end(), a), this->artists.end());
    this->artistById.remove(id);
    delete a;
    if (!this->saveArtists()) return "Không ghi được file artists.txt";
    return "";
}
std::string SongService::addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease){
    if (!this->auth.isAdmin()) return noPermission();
    std::string n = textutil::trim(name), d = textutil::trim(dateRelease);
    if (n.empty()) return "Tên album không được để trống";
    if (!textutil::isSafeText(n)) return "Không được dùng ký tự '|'";
    if (this->findArtist(idArtist) == nullptr) return "Vui lòng chọn nghệ sĩ hợp lệ";
    if (!isValidDate(d)) return "Ngày phát hành phải có dạng yyyy-mm-dd";
    std::vector<std::string> ids;
    for (Album* al : this->albums) ids.push_back(al->getIdAlbum());
    std::string id = textutil::nextId(ids, "AL");
    Album* album = new Album(id, n, idArtist, d);
    this->albums.push_back(album);
    this->albumById.insert(id, album);
    if (!this->saveAlbums()) return "Không ghi được file albums.txt";
    return "";
}
std::string SongService::removeAlbum(const std::string& id){
    if (!this->auth.isAdmin()) return noPermission();
    Album* al = this->findAlbum(id);
    if (al == nullptr) return "Không tìm thấy album";
    for (Song* s : this->songs){
        if (s->getIdAlbum() == id) s->setIdAlbum("");
    }
    this->albums.erase(std::remove(this->albums.begin(), this->albums.end(), al), this->albums.end());
    this->albumById.remove(id);
    delete al;
    this->saveSongs();
    if (!this->saveAlbums()) return "Không ghi được file albums.txt";
    return "";
}

std::string SongService::setAlbumSongs(const std::string& idAlbum, const std::vector<std::string>& songIds){
    if (!this->auth.isAdmin()) return noPermission();
    Album* al = this->findAlbum(idAlbum);
    if (al == nullptr) return "Không tìm thấy album";
    std::vector<std::string> chosen;
    for (const std::string& id : songIds){
        if (this->findSong(id) != nullptr && std::find(chosen.begin(), chosen.end(), id) == chosen.end()) chosen.push_back(id);
    }
    for (Song* s : this->songs){   // bài đang thuộc album mà không được chọn nữa thì gỡ ra
        if (s->getIdAlbum() == idAlbum && std::find(chosen.begin(), chosen.end(), s->getIdSong()) == chosen.end()) s->setIdAlbum("");
    }
    for (const std::string& id : chosen){
        Song* s = this->findSong(id);
        if (!s->getIdAlbum().empty() && s->getIdAlbum() != idAlbum){   // đang ở album khác: gỡ khỏi album đó
            Album* old = this->findAlbum(s->getIdAlbum());
            if (old != nullptr) old->removeSong(id);
        }
        s->setIdAlbum(idAlbum);
    }
    std::vector<std::string> existing = al->getSongIds();
    for (const std::string& id : existing) al->removeSong(id);
    for (const std::string& id : chosen) al->addSong(id);
    this->saveAlbums();
    if (!this->saveSongs()) return "Không ghi được file songs.txt";
    return "";
}

bool SongService::saveSongs(){ return this->songDao.saveAll(this->songs); }
bool SongService::saveArtists(){ return this->artistDao.saveAll(this->artists); }
bool SongService::saveAlbums(){ return this->albumDao.saveAll(this->albums); }
void SongService::increasePlays(const std::string& idSong){
    Song* s = this->findSong(idSong);
    if (s == nullptr) return;
    s->increasePlayCount();
    this->saveSongs();
}
int SongService::songCount() const{ return (int)this->songs.size(); }
int SongService::artistCount() const{ return (int)this->artists.size(); }
int SongService::albumCount() const{ return (int)this->albums.size(); }
long long SongService::totalPlays() const{
    long long sum = 0;
    for (Song* s : this->songs) sum += s->getPlays();
    return sum;
}