#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "songinfo.h"
#include "artistinfo.h"
#include "albuminfo.h"
#include "authservice.h"
#include "src/song.h"
#include "src/artist.h"
#include "src/album.h"
#include "../dal/songdao.h"
#include "../dal/artistdao.h"
#include "../dal/albumdao.h"
#include "../dal/textutil.h"
#include "dsa/hashtable/hashtable.h"

class SongService{
    private:
        SongDAO songDao;
        ArtistDAO artistDao;
        AlbumDAO albumDao;
        AuthService& auth; // Tham chiếu đến AuthService để kiểm tra tài khoản
        std::vector<Song*> songs;
        std::vector<Artist*> artists;
        std::vector<Album*> albums;
        // HashTable giúp tìm đối tượng nhanh bằng ID
        HashTable<std::string, Song*> songById;
        HashTable<std::string, Artist*> artistById;
        HashTable<std::string, Album*> albumById;

        static bool containsText(const std::string& text, const std::string& keywordLower);
        static bool isValidDate(const std::string& date);
        static std::string noPermission();
    public:
        SongService(const std::string& root, AuthService& auth);
        ~SongService();
        SongService(const SongService&) = delete;
        SongService& operator=(const SongService&) = delete;
        void load(); // Nạp dữ liệu từ file

        Song* findSong(const std::string& id) const;
        Artist* findArtist(const std::string& id) const;
        Album* findAlbum(const std::string& id) const;

        SongInfo toInfo(const Song& song, bool withPath = false) const;
        ArtistInfo toInfo(const Artist& artist) const;
        AlbumInfo toInfo(const Album& album) const;
        std::vector<SongInfo> toInfos(const std::vector<std::string>& songIds) const;

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

        std::string addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                            const std::string& type, int durationSec, const std::string& dateRelease, const std::string& sourceFilePath);
        std::string updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum, 
                               const std::string& type, int durationSec, const std::string& dateRelease);
        std::string removeSong(const std::string& id);
        std::string addArtist(const std::string& name, const std::string& country);
        std::string removeArtist(const std::string& id);
        std::string addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease);
        std::string removeAlbum(const std::string& id);

        bool saveSongs();
        bool saveArtists();
        bool saveAlbums();
        void increasePlays(const std::string& idSong);

        int songCount() const;
        int artistCount() const;
        int albumCount() const;
        long long totalPlays() const;
};
SongService::SongService(const std::string& root, AuthService& auth) : songDao(root), artistDao(root), albumDao(root), auth(auth){
}
SongService::~SongService(){
    for (Song* song : songs){
        delete song;
    }
    for (Artist* artist : artists){
        delete artist;
    }
    for (Album* album : albums){
        delete album;
    }
}
bool SongService::containsText(const std::string& text, const std::string& keywordLower){
    std::string textLower = textutil::lower(text);
    if (textLower.find(keywordLower) != std::string::npos){
        return true;
    }
    return false;
}
bool SongService::isValidDate(const std::string& date){ // Kiểm tra ngày có đúng định dạng yyyy-mm-dd hay không
    if (date.empty()){ // Cho phép ngày rỗng
        return true;
    }
    if (date.size() != 10){ // Ngày phải có đúng 10 ký tự
        return false;
    }
    if (date[4] != '-' || date[7] != '-'){ // Vị trí dấu phân cách phải là '-'
        return false;
    }
    // Kiểm tra các vị trí còn lại phải là chữ số
    for (size_t i = 0; i < date.size(); i++){
        if (i == 4 || i == 7){
            continue;
        }
        if (date[i] < '0' || date[i] > '9'){
            return false;
        }
    }
    // Lấy tháng và ngày
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));
    // Kiểm tra phạm vi tháng và ngày
    if (month < 1 || month > 12){
        return false;
    }
    if (day < 1 || day > 31){
        return false;
    }
    return true;
}
std::string SongService::noPermission(){ // Trả về thông báo khi người dùng không có quyền Admin
    return "Chi Admin moi co quyen thuc hien thao tac nay";
}
void SongService::load(){
    // Xóa dữ liệu cũ trước khi nạp lại
    for (Song* song : songs){
        delete song;
    }
    for (Artist* artist : artists){
        delete artist;
    }
    for (Album* album : albums){
        delete album;
    }
    songs.clear();
    artists.clear();
    albums.clear();
    songById.clear();
    artistById.clear();
    albumById.clear();
    // Nạp danh sách bài hát
    std::vector<Song*> loadedSongs = songDao.loadAll();
    for (Song* song : loadedSongs){
        std::string id = song->getIdSong();
        // Nếu ID đã tồn tại thì bỏ qua bản ghi trùng
        if (songById.contains(id)){
            delete song;
            continue;
        }
        songs.push_back(song);
        songById.insert(id, song);
    }
    // Nạp danh sách nghệ sĩ
    std::vector<Artist*> loadedArtists = artistDao.loadAll();
    for (Artist* artist : loadedArtists){
        std::string id = artist->getIdArtist();
        if (artistById.contains(id)){
            delete artist;
            continue;
        }
        artists.push_back(artist);
        artistById.insert(id, artist);
    }
    // Nạp danh sách album
    std::vector<Album*> loadedAlbums = albumDao.loadAll();
    for (Album* album : loadedAlbums){
        std::string id = album->getIdAlbum();
        if (albumById.contains(id)){
            delete album;
            continue;
        }
        albums.push_back(album);
        albumById.insert(id, album);
    }
}
Song* SongService::findSong(const std::string& id) const{
    Song* song = nullptr;
    bool found = songById.search(id, song);
    if (found){
        return song;
    }
    return nullptr;
}
Artist* SongService::findArtist(const std::string& id) const{
    Artist* artist = nullptr;
    bool found = artistById.search(id, artist);
    if (found){
        return artist;
    }
    return nullptr;
}
Album* SongService::findAlbum(const std::string& id) const{
    Album* album = nullptr;
    bool found = albumById.search(id, album);
    if (found){
        return album;
    }
    return nullptr;
}
SongInfo SongService::toInfo(const Song& song, bool withPath) const{ // Chuyển đối tượng Song thành SongInfo
    SongInfo info;
    info.id = song.getIdSong();
    info.name = song.getName();
    info.idArtist = song.getIdArtist();
    info.idAlbum = song.getIdAlbum();
    // Tìm tên nghệ sĩ
    Artist* artist = findArtist(info.idArtist);
    if (artist != nullptr){
        info.artist = artist->getNameArtist();
    } else{
        info.artist = "Khong ro";
    }
    // Tìm tên album
    Album* album = findAlbum(info.idAlbum);
    if (album != nullptr){
        info.album = album->getNameAlbum();
    } else{
        info.album = "";
    }
    info.type = song.getTypeSong();
    info.date = song.getDateRelease();
    info.duration = song.getDuration();
    info.plays = song.getPlays();
    // Kiểm tra người dùng hiện tại đã thích bài hát chưa
    Listener* listener = auth.currentListener();
    if (listener != nullptr){
        info.liked = listener->isLiked(info.id);
    } else{
        info.liked = false;
    }
    // Chỉ lấy đường dẫn file nhạc khi withPath = true
    if (withPath){
        info.path = songDao.resolveAudioPath(song);
    }
    return info;
}
ArtistInfo SongService::toInfo(const Artist& artist) const{ // Chuyển đối tượng Artist thành ArtistInfo
    ArtistInfo info;
    info.id = artist.getIdArtist();
    info.name = artist.getNameArtist();
    info.country = artist.getCountry();
    info.followers = artist.getFollowers();
    // Đếm số bài hát của nghệ sĩ
    for (Song* song : songs){
        if (song->getIdArtist() == info.id){
            info.songCount++;
        }
    }
    // Kiểm tra người dùng hiện tại đã theo dõi nghệ sĩ chưa
    Listener* listener = auth.currentListener();
    if (listener != nullptr){
        info.followed = listener->isFollowing(info.id);
    } else{
        info.followed = false;
    }
    return info;
}
AlbumInfo SongService::toInfo(const Album& album) const{ // Chuyển đối tượng Album thành AlbumInfo
    AlbumInfo info;
    info.id = album.getIdAlbum();
    info.name = album.getNameAlbum();
    info.idArtist = album.getIdArtist();
    // Tìm tên nghệ sĩ của album
    Artist* artist = findArtist(info.idArtist);
    if (artist != nullptr){
        info.artist = artist->getNameArtist();
    } else{
        info.artist = "Không rõ";
    }
    info.date = album.getDateRelease();
    info.trackCount = album.getTrackCount();
    // Tính tổng thời lượng các bài hát trong album
    for (const std::string& id : album.getSongIds()){
        Song* song = findSong(id);
        if (song != nullptr){
            info.totalSeconds += song->getDuration();
        }
    }
    return info;
}
std::vector<SongInfo> SongService::toInfos(const std::vector<std::string>& songIds) const{ // Chuyển danh sách ID bài hát thành danh sách SongInfo
    std::vector<SongInfo> result;
    for (const std::string& id : songIds){
        Song* song = findSong(id);
        // Chỉ thêm bài hát nếu tìm thấy
        if (song != nullptr){
            result.push_back(toInfo(*song));
        }
    }
    return result;
}
std::vector<SongInfo> SongService::getAllSongs() const {
    // Tạo bản sao danh sách con trỏ để sắp xếp
    std::vector<Song*> sortedSongs = songs;
    // Sắp xếp theo tên bài hát
    std::sort(sortedSongs.begin(), sortedSongs.end(), [](Song* a, Song* b){
            return textutil::lower(a->getName()) < textutil::lower(b->getName());
        }
    );
    std::vector<SongInfo> result;
    for (Song* song : sortedSongs){
        result.push_back(toInfo(*song));
    }
    return result;
}
std::vector<ArtistInfo> SongService::getAllArtists() const{
    std::vector<Artist*> sortedArtists = artists;
    // Sắp xếp nghệ sĩ theo tên
    std::sort(sortedArtists.begin(), sortedArtists.end(), [](Artist* a, Artist* b){
            return textutil::lower(a->getNameArtist()) < textutil::lower(b->getNameArtist());
        }
    );
    std::vector<ArtistInfo> result;
    for (Artist* artist : sortedArtists){
        result.push_back(toInfo(*artist));
    }
    return result;
}
std::vector<AlbumInfo> SongService::getAllAlbums() const{
    std::vector<Album*> sortedAlbums = albums;
    // Album mới hơn được đưa lên trước
    std::sort(sortedAlbums.begin(), sortedAlbums.end(), [](Album* a, Album* b){
            return a->getDateRelease() > b->getDateRelease();
        }
    );
    std::vector<AlbumInfo> result;
    for (Album* album : sortedAlbums){
        result.push_back(toInfo(*album));
    }
    return result;
}
std::vector<SongInfo> SongService::searchSongs(const std::string& keyword) const{
    std::vector<SongInfo> result;
    // Chuẩn hóa từ khóa tìm kiếm
    std::string key = textutil::lower(textutil::trim(keyword));
    if (key.empty()){
        return result;
    }
    for (Song* song : songs){
        SongInfo info = toInfo(*song);
        // Tìm theo tên bài hát, nghệ sĩ, album hoặc thể loại
        if (containsText(info.name, key) || containsText(info.artist, key) || containsText(info.album, key) || containsText(info.type, key)){
            result.push_back(info);
        }
    }
    return result;
}
std::vector<ArtistInfo> SongService::searchArtists(const std::string& keyword) const{
    std::vector<ArtistInfo> result;
    std::string key = textutil::lower(textutil::trim(keyword));
    if (key.empty()){
        return result;
    }
    for (Artist* artist : artists){
        // Tìm theo tên nghệ sĩ hoặc quốc gia
        if (containsText(artist->getNameArtist(), key) || containsText(artist->getCountry(), key)){
            result.push_back(toInfo(*artist));
        }
    }
    return result;
}
std::vector<AlbumInfo> SongService::searchAlbums(const std::string& keyword) const{
    std::vector<AlbumInfo> result;
    std::string key = textutil::lower(textutil::trim(keyword));
    if (key.empty()){
        return result;
    }
    for (Album* album : albums){
        AlbumInfo info = toInfo(*album);
        // Tìm theo tên album hoặc tên nghệ sĩ
        if (containsText(info.name, key) || containsText(info.artist, key)){
            result.push_back(info);
        }
    }
    return result;
}
std::vector<SongInfo> SongService::topSongs(int n) const{
    std::vector<SongInfo> result;
    if (n <= 0){
        return result;
    }
    std::vector<Song*> sortedSongs = songs; // Tạo bản sao danh sách bài hát
    int count = std::min(n, (int)sortedSongs.size()); // Số lượng bài hát cần lấy
    // Sắp xếp các bài hát có lượt nghe cao nhất lên đầu
    std::partial_sort(sortedSongs.begin(), sortedSongs.begin() + count, sortedSongs.end(), SongPlaysComparator());
    for (int i = 0; i < count; i++){
        result.push_back(toInfo(*sortedSongs[i]));
    }
    return result;
}
std::vector<ArtistInfo> SongService::topArtists(int n) const{
    std::vector<ArtistInfo> result;
    if (n <= 0){
        return result;
    }
    std::vector<Artist*> sortedArtists = artists;
    int count = std::min(n, (int)sortedArtists.size());
    // Sắp xếp nghệ sĩ có nhiều người theo dõi nhất lên đầu
    std::partial_sort(sortedArtists.begin(), sortedArtists.begin() + count, sortedArtists.end(), ArtistFollowersComparator());
    for (int i = 0; i < count; i++){
        result.push_back(toInfo(*sortedArtists[i]));
    }
    return result;
}
std::vector<SongInfo> SongService::newestSongs(int n) const{
    std::vector<SongInfo> result;
    if (n <= 0){
        return result;
    }
    std::vector<Song*> sortedSongs = songs;
    int count = std::min(n, (int)sortedSongs.size());
    // Sắp xếp theo ngày phát hành mới nhất
    std::partial_sort(sortedSongs.begin(), sortedSongs.begin() + count, sortedSongs.end(), [](Song* a, Song* b){
            return a->getDateRelease() > b->getDateRelease();
        }
    );
    for (int i = 0; i < count; i++){
        result.push_back(toInfo(*sortedSongs[i]));
    }
    return result;
}
std::vector<std::string> SongService::genres() const{
    std::vector<std::string> result;
    for (Song* song : songs){
        std::string type = song->getTypeSong();
        if (!type.empty()){
            result.push_back(type);
        }
    }
    std::sort(result.begin(), result.end()); // Sắp xếp danh sách thể loại
    result.erase(std::unique(result.begin(), result.end()), result.end()); // Xóa các thể loại bị trùng
    return result;
}
std::vector<SongInfo> SongService::songsByGenre(const std::string& genre) const{
    std::vector<Song*> selectedSongs;
    std::string selectedGenre = textutil::lower(genre);
    for (Song* song : songs){
        std::string type = textutil::lower(song->getTypeSong());
        if (type == selectedGenre){
            selectedSongs.push_back(song);
        }
    }
    // Sắp xếp theo lượt nghe giảm dần
    std::sort(selectedSongs.begin(), selectedSongs.end(), SongPlaysComparator());
    std::vector<SongInfo> result;
    for (Song* song : selectedSongs){
        result.push_back(toInfo(*song));
    }
    return result;
}
std::vector<SongInfo> SongService::songsOfArtist(const std::string& idArtist) const{
    std::vector<Song*> selectedSongs;
    for (Song* song : songs){
        if (song->getIdArtist() == idArtist){
            selectedSongs.push_back(song);
        }
    }
    // Sắp xếp theo lượt nghe giảm dần
    std::sort(selectedSongs.begin(), selectedSongs.end(), SongPlaysComparator());
    std::vector<SongInfo> result;
    for (Song* song : selectedSongs){
        result.push_back(toInfo(*song));
    }
    return result;
}
std::vector<SongInfo> SongService::songsOfAlbum(const std::string& idAlbum) const{
    Album* album = findAlbum(idAlbum);
    // Không tìm thấy album
    if (album == nullptr){
        return std::vector<SongInfo>();
    }
    // Chuyển danh sách ID bài hát trong album thành SongInfo
    // Thứ tự bài hát được giữ nguyên theo tracklist
    return toInfos(album->getSongIds());
}
std::vector<AlbumInfo> SongService::albumsOfArtist(const std::string& idArtist) const{
    std::vector<Album*> selectedAlbums;
    // Tìm các album thuộc nghệ sĩ
    for (Album* album : albums){
        if (album->getIdArtist() == idArtist){
            selectedAlbums.push_back(album);
        }
    }
    // Sắp xếp album mới nhất lên trước
    std::sort(selectedAlbums.begin(), selectedAlbums.end(), [](Album* a, Album* b){
            return a->getDateRelease() > b->getDateRelease();
        }
    );
    std::vector<AlbumInfo> result;
    for (Album* album : selectedAlbums){
        result.push_back(toInfo(*album));
    }
    return result;
}
std::string SongService::addSong(const std::string& name, const std::string& idArtist, const std::string& idAlbum, const std::string& type,
                                 int durationSec, const std::string& dateRelease, const std::string& sourceFilePath){
    if (!auth.isAdmin()){ // Chỉ Admin mới được thêm bài hát
        return noPermission();
    }
    // Loại bỏ khoảng trắng thừa
    std::string songName = textutil::trim(name);
    std::string songType = textutil::trim(type);
    std::string releaseDate = textutil::trim(dateRelease);
    if (songName.empty()){
        return "Ten bai hat khong duoc de trong";
    }
    if (!textutil::isSafeText(songName) || !textutil::isSafeText(songType)){
        return "Khong duoc dung ky tu '|' trong ten hoac the loai";
    }
    // Kiểm tra nghệ sĩ
    if (findArtist(idArtist) == nullptr){
        return "Vui long chon nghe si hop le";
    }
    // Kiểm tra album nếu có chọn album
    if (!idAlbum.empty() && findAlbum(idAlbum) == nullptr){
        return "Album khong ton tai";
    }
    if (durationSec <= 0){
        return "Thoi luong phai lon hon 0";
    }
    if (!isValidDate(releaseDate)){
        return "Ngay phat hanh phai co dang yyyy-mm-dd";
    }
    // Nhập file nhạc vào thư mục quản lý
    std::string storedPath = "";
    if (!sourceFilePath.empty()){
        storedPath = songDao.importAudio(sourceFilePath);
        if (storedPath.empty()){
            return "Khong doc hoac sao chep duoc file nhac";
        }
    }
    // Lấy danh sách ID hiện tại
    std::vector<std::string> ids;
    for (Song* song : songs){
        ids.push_back(song->getIdSong());
    }
    std::string newId = textutil::nextId(ids, "S"); // Tạo ID mới
    // Tạo đối tượng Song
    if (songType.empty()){
        songType = "Khac";
    }
    Song* newSong = new Song(newId, songName, idArtist, idAlbum, songType, durationSec, releaseDate, storedPath, 0);
    // Thêm vào vector và HashTable
    songs.push_back(newSong);
    songById.insert(newId, newSong);
    // Nếu có album thì thêm bài hát vào album
    if (!idAlbum.empty()){
        Album* album = findAlbum(idAlbum);
        album->addSong(newId);
        saveAlbums();
    }
    // Lưu danh sách bài hát
    if (!saveSongs()){
        return "Khong ghi duoc file songs.txt";
    }
    return "";
}
std::string SongService::updateSong(const std::string& id, const std::string& name, const std::string& idArtist, const std::string& idAlbum,
                                    const std::string& type, int durationSec, const std::string& dateRelease){
    if (!auth.isAdmin()){
        return noPermission();
    }
    // Tìm bài hát cần cập nhật
    Song* song = findSong(id);
    if (song == nullptr){
        return "Khong tim thay bai hat";
    }
    std::string songName = textutil::trim(name);
    std::string songType = textutil::trim(type);
    std::string releaseDate = textutil::trim(dateRelease);
    if (songName.empty()){
        return "Ten bai hat khong duoc de trong";
    }
    if (!textutil::isSafeText(songName) || !textutil::isSafeText(songType)){
        return "Khong duoc dung ky tu '|' trong ten hoac the loai";
    }
    if (findArtist(idArtist) == nullptr){
        return "Vui long chon nghe si hop le";
    }
    if (!idAlbum.empty() && findAlbum(idAlbum) == nullptr){
        return "Album khong ton tai";
    }
    if (durationSec <= 0){
        return "Thoi luong phai lon hơn 0";
    }
    if (!isValidDate(releaseDate)){
        return "Ngay phat hanh phai co dang yyyy-mm-dd";
    }
    // Nếu đổi album thì cập nhật tracklist của cả hai album
    if (song->getIdAlbum() != idAlbum){
        // Xóa bài hát khỏi album cũ
        Album* oldAlbum = findAlbum(song->getIdAlbum());
        if (oldAlbum != nullptr){
            oldAlbum->removeSong(id);
        }
        // Thêm bài hát vào album mới
        Album* newAlbum = findAlbum(idAlbum);
        if (newAlbum != nullptr){
            newAlbum->addSong(id);
        }
        saveAlbums();
    }
    // Cập nhật thông tin bài hát
    if (songType.empty()){
        songType = "Khác";
    }
    song->setName(songName);
    song->setIdArtist(idArtist);
    song->setIdAlbum(idAlbum);
    song->setTypeSong(songType);
    song->setDuration(durationSec);
    song->setDateRelease(releaseDate);
    // Lưu dữ liệu
    if (!saveSongs()){
        return "Khong ghi duoc file songs.txt";
    }
    return "";
}
std::string SongService::removeSong(const std::string& id){
    if (!auth.isAdmin()){
        return noPermission();
    }
    Song* song = findSong(id);
    if (song == nullptr){
        return "Khong tim thay bai hat";
    }
    // Xóa bài hát khỏi album nếu bài hát thuộc album
    Album* album = findAlbum(song->getIdAlbum());
    if (album != nullptr){
        album->removeSong(id);
        saveAlbums();
    }
    songs.erase(std::remove(songs.begin(), songs.end(), song), songs.end());  // Xóa bài hát khỏi vector
    songById.remove(id); // Xóa bài hát khỏi HashTable
    delete song; // Giải phóng bộ nhớ
    // Lưu dữ liệu
    if (!saveSongs()){
        return "Khong ghi duoc file songs.txt";
    }
    return "";
}
std::string SongService::addArtist(const std::string& name, const std::string& country){
    if (!auth.isAdmin()){
        return noPermission();
    }
    std::string artistName = textutil::trim(name);
    std::string artistCountry = textutil::trim(country);
    if (artistName.empty()){
        return "Ten nghe si khong duoc de trong";
    }
    if (!textutil::isSafeText(artistName) || !textutil::isSafeText(artistCountry)){
        return "Khong duoc dung ky tu '|'";
    }
    // Kiểm tra nghệ sĩ đã tồn tại chưa
    for (Artist* artist : artists){
        if (textutil::lower(artist->getNameArtist()) == textutil::lower(artistName)){
            return "Nghe si nay đa ton tai";
        }
    }
    // Lấy danh sách ID hiện tại
    std::vector<std::string> ids;
    for (Artist* artist : artists){
        ids.push_back(artist->getIdArtist());
    }
    std::string newId = textutil::nextId(ids, "A"); // Tạo ID mới
    Artist* newArtist = new Artist(newId, artistName, artistCountry, 0); // Tạo đối tượng Artist
    // Thêm vào vector và HashTable
    artists.push_back(newArtist);
    artistById.insert(newId, newArtist);
    if (!saveArtists()){
        return "Khong ghi duoc file artists.txt";
    }
    return "";
}
std::string SongService::removeArtist(const std::string& id){
    if (!auth.isAdmin()){
        return noPermission();
    }
    Artist* artist = findArtist(id);
    if (artist == nullptr){
        return "Khong tim thay nghe si";
    }
    // Không cho xóa nếu nghệ sĩ vẫn còn bài hát
    for (Song* song : songs){
        if (song->getIdArtist() == id){
            return "Nghe si con bai hat, hay xoa hoac chuyen bai hat truoc";
        }
    }
    // Không cho xóa nếu nghệ sĩ vẫn còn album
    for (Album* album : albums){
        if (album->getIdArtist() == id){
            return "Nghe si con album, hay xoa album truoc";
        }
    }
    artists.erase(std::remove(artists.begin(), artists.end(), artist), artists.end()); // Xóa khỏi vector
    artistById.remove(id); // Xóa khỏi HashTable
    delete artist; // Giải phóng bộ nhớ
    if (!saveArtists()){
        return "Khong ghi duoc file artists.txt";
    }
    return "";
}
std::string SongService::addAlbum(const std::string& name, const std::string& idArtist, const std::string& dateRelease){
    if (!auth.isAdmin()){
        return noPermission();
    }
    std::string albumName = textutil::trim(name);
    std::string releaseDate = textutil::trim(dateRelease);
    if (albumName.empty()){
        return "Ten album khong duoc de trong";
    }
    if (!textutil::isSafeText(albumName)){
        return "Khong duoc dung ky tu '|'";
    }
    if (findArtist(idArtist) == nullptr){
        return "Vui long chon nghe si hop le";
    }
    if (!isValidDate(releaseDate)){
        return "Ngay phat hanh phai co dang yyyy-mm-dd";
    }
    // Lấy danh sách ID hiện tại
    std::vector<std::string> ids;
    for (Album* album : albums){
        ids.push_back(album->getIdAlbum());
    }
    std::string newId = textutil::nextId(ids, "AL"); // Tạo ID mới
    // Tạo đối tượng Album
    Album* newAlbum = new Album(newId, albumName, idArtist, releaseDate);
    // Thêm vào vector và HashTable
    albums.push_back(newAlbum);
    albumById.insert(newId, newAlbum);
    if (!saveAlbums()){
        return "Khong ghi duoc file albums.txt";
    }
    return "";
}
std::string SongService::removeAlbum(const std::string& id){
    if (!auth.isAdmin()){
        return noPermission();
    }
    Album* album = findAlbum(id);
    if (album == nullptr){
        return "Khong tim thay album";
    }
    // Bài hát vẫn được giữ lại nhưng không còn thuộc album
    for (Song* song : songs){
        if (song->getIdAlbum() == id){
            song->setIdAlbum("");
        }
    }
    albums.erase(std::remove(albums.begin(), albums.end(), album), albums.end()); // Xóa album khỏi vector
    albumById.remove(id); // Xóa album khỏi HashTable
    delete album; // Giải phóng bộ nhớ
    // Lưu lại dữ liệu
    saveSongs();
    if (!saveAlbums()){
        return "Khong ghi duoc file albums.txt";
    }
    return "";
}
bool SongService::saveSongs(){
    return songDao.saveAll(songs);
}
bool SongService::saveArtists(){
    return artistDao.saveAll(artists);
}
bool SongService::saveAlbums(){
    return albumDao.saveAll(albums);
}
void SongService::increasePlays(const std::string& idSong){
    // Tìm bài hát
    Song* song = findSong(idSong);
    if (song == nullptr){
        return;
    }
    song->increasePlayCount(); // Tăng số lượt nghe
    saveSongs(); // Lưu lại dữ liệu
}
int SongService::songCount() const{
    return (int)songs.size();
}
int SongService::artistCount() const{
    return (int)artists.size();
}
int SongService::albumCount() const{
    return (int)albums.size();
}
// Tính tổng lượt nghe của tất cả bài hát
long long SongService::totalPlays() const{
    long long total = 0;
    for (Song* song : songs){
        total += song->getPlays();
    }
    return total;
}