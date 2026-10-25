#pragma once
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "songinfo.h"
#include "playlistinfo.h"
#include "src/song.h"
#include "src/user.h"
#include "src/admin.h"
#include "src/listener.h"
#include "src/playlist.h"
#include "dsa/hashtable/hashtable.h"
#include "dsa/maxheap/maxheap.h"
#include "dsa/dlinkedlist/dlinkedlist.h"

namespace musicdetail{
struct ByPlayCount{
    bool operator()(Song* a, Song* b) const{ 
        return a->getPlays() > b->getPlays(); 
    }
};
std::string trim(const std::string& s){
    size_t b = 0, e = s.size();
    while (b < e && std::isspace((unsigned char)s[b])) ++b;
    while (e > b && std::isspace((unsigned char)s[e - 1])) --e;
    return s.substr(b, e - b);
}
std::string lower(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}
int toInt(const std::string& s, int def = 0){ 
    try{ 
        return std::stoi(s); 
    } 
    catch (...){ 
        return def; 
    } 
}
long long toLL(const std::string& s, long long def = 0){ 
    try{ 
        return std::stoll(s); 
    } 
    catch (...){ 
        return def; 
    } 
}
char detectDelim(const std::string& line){
    for (char c : {'|', ';', '\t', ','}) if (line.find(c) != std::string::npos) return c;
    return '|';
}
std::vector<std::string> split(const std::string& line, char d){
    std::vector<std::string> parts; std::string item; std::stringstream ss(line);
    while (std::getline(ss, item, d)) parts.push_back(trim(item));
    return parts;
}
std::vector<std::string> readLines(const std::string& path){
    std::vector<std::string> lines; std::ifstream in(path); std::string l; bool first = true;
    while (std::getline(in, l)){
        if (first && l.size() >= 3 && (unsigned char)l[0] == 0xEF && (unsigned char)l[1] == 0xBB && (unsigned char)l[2] == 0xBF)
            l.erase(0, 3);                                  
        first = false;
        if (!l.empty() && l.back() == '\r') l.pop_back();
        if (!trim(l).empty() && l[0] != '#') lines.push_back(l);
    }
    return lines;
}
std::string nextId(const std::vector<std::string>& ids, const std::string& defPrefix){
    std::string prefix = defPrefix; size_t width = 3; long long mx = 0; bool first = true;
    for (const auto& id : ids){
        size_t i = id.size();
        while (i > 0 && std::isdigit((unsigned char)id[i - 1])) --i;
        std::string digits = id.substr(i);
        if (digits.empty()) continue;
        if (first){ 
            prefix = id.substr(0, i); 
            width = digits.size(); 
            first = false; 
        }
        mx = std::max(mx, toLL(digits));
    }
    std::string num = std::to_string(mx + 1);
    if (num.size() < width) num = std::string(width - num.size(), '0') + num;
    return prefix + num;
}
bool seekTo(DoublyLinkedList<Song*>& l, Song* target){
    if (l.isEmpty() || !target) return false;
    l.resetPlayback();
    for (int i = 0; i < l.getSize(); ++i){
        if (l.getCurrent() == target) return true;
        if (!l.hasNext()) break;
        l.next();
    }
    return false;
}
std::vector<Song*> toVector(DoublyLinkedList<Song*>& l){
    std::vector<Song*> v;
    if (l.isEmpty()) return v;
    Song* cur = nullptr;
    try{ 
        cur = l.getCurrent(); 
    } 
    catch (const std::exception&){
    }
    l.resetPlayback();
    for (int i = 0; i < l.getSize(); ++i){
        v.push_back(l.getCurrent());
        if (i + 1 < l.getSize()) l.next();
    }
    if (cur) seekTo(l, cur);
    return v;
}
} 
class MusicSystem{
     private:
        enum Mode{ 
            None, 
            QueueMode, 
            PlaylistMode 
        };
        std::string root, dataDir, musicDir;
        char songDelim = '|';
        std::vector<Song*> songs;                        // so huu Song
        std::vector<User*> users;                        // so huu User (Admin/Listener)
        HashTable<std::string, User*> byName;            // dang nhap: username -> User*
        HashTable<std::string, User*> byId;              // id_user -> User*
        HashTable<std::string, Song*> songById;          // id_song -> Song*
        User* current = nullptr;
        DoublyLinkedList<Song*> queue;                   // hang phat tu thu vien / top
        Mode mode = None;
        std::string activePl;                            // playlist dang phat (khi mode == PlaylistMode)
        Listener* curListener() const;
        Playlist* getPlaylist(const std::string& id) const;
        DoublyLinkedList<Song*>* activeList();
        std::string resolve(const std::string& p) const;
        SongInfo info(Song* s) const;
        Song* findSong(const std::string& id) const;
        void registerUser(User* u);
        void loadSongs();
        void saveSongs() const;
        void loadUsers();
        void loadPlaylists();
        void savePlaylists();
    public:
        MusicSystem();
        ~MusicSystem();
        MusicSystem(const MusicSystem&) = delete;
        MusicSystem& operator=(const MusicSystem&) = delete;
        void load(const std::string& root); // root = thu muc goc du an (co data/ va assets/music/)

        bool login(const std::string& username, const std::string& password);
        void logout();
        bool isLoggedIn() const;
        bool isAdmin() const;
        std::string currentUsername() const;

        std::vector<SongInfo> allSongs() const;
        std::vector<SongInfo> search(const std::string& keyword) const;
        std::vector<SongInfo> topSongs(int n) const;           
        void recordPlay(const std::string& idSong);
        std::string musicFolder() const;

        bool startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out);
        bool startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out);
        bool nextTrack(SongInfo& out);
        bool prevTrack(SongInfo& out);

        std::vector<PlaylistInfo> myPlaylists() const;
        std::vector<SongInfo> playlistSongs(const std::string& idPlaylist);
        std::string createPlaylist(const std::string& name);    // tra ve id, "" neu that bai
        bool deletePlaylist(const std::string& idPlaylist);
        bool addToPlaylist(const std::string& idPlaylist, const std::string& idSong);
        bool removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong);

        bool addSong(const std::string& name, const std::string& artist, const std::string& album,
                 const std::string& type, int durationSec, const std::string& dateRelease, const std::string& fileName);
        bool removeSong(const std::string& idSong);
};
MusicSystem::MusicSystem(){}
MusicSystem::~MusicSystem(){
    for (User* u : users) delete u;
    for (Song* s : songs) delete s;
}
void MusicSystem::load(const std::string& root){
    this->root = root;
    this->dataDir = root + "/data";
    this->musicDir = root + "/assets/music";
    loadSongs();
    loadUsers();
    loadPlaylists();
}
Listener* MusicSystem::curListener() const{ 
    return dynamic_cast<Listener*>(current); 
}
Playlist* MusicSystem::getPlaylist(const std::string& id) const{
    Listener* l = curListener();
    return l ? l->getPlaylistById(id) : nullptr;
}
DoublyLinkedList<Song*>* MusicSystem::activeList(){
    if (mode == QueueMode) return &queue;
    if (mode == PlaylistMode){ 
        Playlist* p = getPlaylist(activePl); 
        return p ? &p->getQueue() : nullptr; 
    }
    return nullptr;
}
std::string MusicSystem::resolve(const std::string& p0) const{
    namespace fs = std::filesystem;
    if (p0.empty()) return "";
    std::string p = p0;
    std::replace(p.begin(), p.end(), '\\', '/');
    std::error_code ec;

    std::vector<std::string> bases = { root + "/" + p, musicDir + "/" + p, p,
                                       musicDir + "/" + fs::path(p).filename().string() };
    for (const auto& b : bases){
        if (fs::exists(fs::path(b), ec)) return b;
        if (fs::exists(fs::path(b + ".mp3"), ec)) return b + ".mp3";
    }
    std::string want = musicdetail::lower(fs::path(p).stem().string());
    for (const auto& e : fs::directory_iterator(musicDir, ec))
        if (musicdetail::lower(e.path().stem().string()) == want)
            return e.path().string();
    return "";
}
SongInfo MusicSystem::info(Song* s) const{
    SongInfo i;
    i.id = s->getIdSong(); 
    i.name = s->getName(); 
    i.artist = s->getArtist();
    i.album = s->getAlbum(); 
    i.type = s->getTypeSong(); 
    i.date = s->getDateRelease();
    i.duration = s->getDuration(); 
    i.count = s->getPlays(); 
    i.path = resolve(s->getFilePath());
    return i;
}
Song* MusicSystem::findSong(const std::string& id) const{
    Song* s = nullptr;
    return songById.search(id, s) ? s : nullptr;
}
void MusicSystem::registerUser(User* u){
    users.push_back(u);
    byName.insert(u->getUsername(), u);
    byId.insert(u->getIdUser(), u);
}
void MusicSystem::loadSongs(){
    using namespace musicdetail;
    auto lines = readLines(dataDir + "/songs.txt");
    if (!lines.empty()) songDelim = detectDelim(lines[0]);
    for (const auto& l : lines){
        auto p = split(l, songDelim);
        if (p.size() < 9) continue;
        Song* s = new Song(p[0], p[1], p[2], p[3], p[4], toInt(p[5]), p[6], p[8], 0, toLL(p[7]));
        songs.push_back(s);
        songById.insert(s->getIdSong(), s);
    }
}
void MusicSystem::saveSongs() const{
    std::ofstream out(dataDir + "/songs.txt");
    const std::string d = (songDelim == '|') ? " | " : std::string(1, songDelim);
    for (Song* s : songs)
        out << s->getIdSong() << d << s->getName() << d << s->getArtist() << d << s->getAlbum() << d
            << s->getTypeSong() << d << s->getDuration() << d << s->getDateRelease() << d
            << s->getPlays() << d << s->getFilePath() << '\n';
}
void MusicSystem::loadUsers(){
    using namespace musicdetail;
    for (const auto& l : readLines(dataDir + "/admin.txt")){
        auto p = split(l, detectDelim(l));
        if (p.size() < 4) continue;
        registerUser(new Admin(p[0], p[1], p[2], p[3], p.size() > 4 ? p[4] : "ADMIN"));
    }
    for (const auto& l : readLines(dataDir + "/listener.txt")) {
        auto p = split(l, detectDelim(l));
        if (p.size() < 4) continue;
        Listener* ls = new Listener(p[0], p[1], p[2], p[3]);
        if (p.size() > 4) ls->setTotalSongsPlayed(toInt(p[4]));
        registerUser(ls);
    }
}
void MusicSystem::loadPlaylists(){
    using namespace musicdetail;
    for (const auto& l : readLines(dataDir + "/playlists.txt")){
        char d = detectDelim(l);
        auto p = split(l, d);
        if (p.size() < 3) continue;
        User* u = nullptr;
        if (!byId.search(p[2], u)) continue;
        Listener* ls = dynamic_cast<Listener*>(u);
        if (!ls) continue;
        ls->addPlaylist(Playlist(p[0], p[1], p[2]));
        Playlist* pl = ls->getPlaylistById(p[0]);
        if (p.size() > 3 && pl)
            for (const auto& sid : split(p[3], d == ';' ? ',' : ';'))
                if (Song* s = findSong(sid)) pl->addSong(s);
    }
}
void MusicSystem::savePlaylists(){
    std::ofstream out(dataDir + "/playlists.txt");
    for (User* u : users){
        Listener* ls = dynamic_cast<Listener*>(u);
        if (!ls) continue;
        std::vector<std::string> ids;
        for (const auto& pl : ls->getPlaylists()) ids.push_back(pl.getIdPlaylist());
        for (const auto& id : ids){
            Playlist* pl = ls->getPlaylistById(id);
            out << pl->getIdPlaylist() << '|' << pl->getNamePlaylist() << '|' << pl->getIdUser() << '|';
            bool first = true;
            for (Song* s : musicdetail::toVector(pl->getQueue())){ 
                out << (first ? "" : ";") << s->getIdSong(); 
                first = false; 
            }
            out << '\n';
        }
    }
}
bool MusicSystem::login(const std::string& username, const std::string& password){
    User* u = nullptr;
    if (!byName.search(username, u) || !u || u->getPassword() != password) return false;  
    current = u;
    mode = None;
    return true;
}
void MusicSystem::logout(){ 
    current = nullptr; 
    mode = None; 
}
bool MusicSystem::isLoggedIn() const{ 
    return current != nullptr; 
}
bool MusicSystem::isAdmin() const{ 
    return dynamic_cast<Admin*>(current) != nullptr; 
}   
std::string MusicSystem::currentUsername() const{ 
    return current ? current->getUsername() : ""; 
}
std::string MusicSystem::musicFolder() const{ 
    return musicDir; 
}
std::vector<SongInfo> MusicSystem::allSongs() const{
    std::vector<SongInfo> v;
    for (Song* s : songs) v.push_back(info(s));
    return v;
}
std::vector<SongInfo> MusicSystem::search(const std::string& keyword) const{
    std::vector<SongInfo> v;
    std::string k = musicdetail::lower(keyword);
    for (Song* s : songs)
        if (musicdetail::lower(s->getName()).find(k) != std::string::npos ||
            musicdetail::lower(s->getArtist()).find(k) != std::string::npos ||
            musicdetail::lower(s->getAlbum()).find(k) != std::string::npos)
            v.push_back(info(s));
    return v;
}
std::vector<SongInfo> MusicSystem::topSongs(int n) const{
    MaxHeap<Song*, musicdetail::ByPlayCount> heap;      
    for (Song* s : songs) heap.insert(s);
    std::vector<SongInfo> v;
    for (int i = 0; i < n && !heap.isEmpty(); ++i) v.push_back(info(heap.extractMax()));
    return v;
}
void MusicSystem::recordPlay(const std::string& idSong){
    Song* s = findSong(idSong);
    if (!s) return;
    s->increasePlayCount();
    s->setPlays(s->getPlays() + 1);
    if (Listener* l = curListener()) l->increasePlayedCount();
    saveSongs();
}
bool MusicSystem::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    queue.clear();
    for (const auto& id : ids) if (Song* s = findSong(id)) queue.pushBack(s);
    Song* st = findSong(startId);
    if (!st || !musicdetail::seekTo(queue, st)) return false;
    mode = QueueMode;
    out = info(st);
    return true;
}
bool MusicSystem::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    Playlist* pl = getPlaylist(idPlaylist);
    if (!pl || pl->getQueue().isEmpty()) return false;
    DoublyLinkedList<Song*>& l = pl->getQueue();
    Song* st = startId.empty() ? nullptr : findSong(startId);
    if (!st) { l.resetPlayback(); st = l.getCurrent(); }
    else if (!musicdetail::seekTo(l, st)) return false;
    mode = PlaylistMode;
    activePl = idPlaylist;
    out = info(st);
    return true;
}
bool MusicSystem::nextTrack(SongInfo& out){
    DoublyLinkedList<Song*>* l = activeList();
    if (!l || !l->hasNext()) return false;
    out = info(l->next());
    return true;
}
bool MusicSystem::prevTrack(SongInfo& out){
    DoublyLinkedList<Song*>* l = activeList();
    if (!l || !l->hasPrev()) return false;
    out = info(l->prev());
    return true;
}
std::vector<PlaylistInfo> MusicSystem::myPlaylists() const{
    std::vector<PlaylistInfo> v;
    if (Listener* l = curListener())
        for (const auto& pl : l->getPlaylists())
            v.push_back({pl.getIdPlaylist(), pl.getNamePlaylist(), pl.getSongCount()});
    return v;
}
std::vector<SongInfo> MusicSystem::playlistSongs(const std::string& idPlaylist){
    std::vector<SongInfo> v;
    if (Playlist* pl = getPlaylist(idPlaylist))
        for (Song* s : musicdetail::toVector(pl->getQueue())) v.push_back(info(s));
    return v;
}
std::string MusicSystem::createPlaylist(const std::string& name){
    Listener* l = curListener();
    if (!l || musicdetail::trim(name).empty()) return "";
    std::vector<std::string> ids;
    for (User* u : users)
        if (Listener* x = dynamic_cast<Listener*>(u))
            for (const auto& pl : x->getPlaylists()) ids.push_back(pl.getIdPlaylist());
    std::string id = musicdetail::nextId(ids, "PL");
    l->addPlaylist(Playlist(id, musicdetail::trim(name), l->getIdUser()));
    savePlaylists();
    return id;
}
bool MusicSystem::deletePlaylist(const std::string& idPlaylist){
    Listener* l = curListener();
    if (!l) return false;
    if (mode == PlaylistMode && activePl == idPlaylist) mode = None;
    bool ok = l->removePlaylist(idPlaylist);
    if (ok) savePlaylists();
    return ok;
}
bool MusicSystem::addToPlaylist(const std::string& idPlaylist, const std::string& idSong){
    Playlist* pl = getPlaylist(idPlaylist);
    Song* s = findSong(idSong);
    if (!pl || !s) return false;
    for (Song* x : musicdetail::toVector(pl->getQueue())) if (x == s) return false;   // da co
    pl->addSong(s);
    savePlaylists();
    return true;
}
bool MusicSystem::removeFromPlaylist(const std::string& idPlaylist, const std::string& idSong){
    Playlist* pl = getPlaylist(idPlaylist);
    Song* s = findSong(idSong);
    if (!pl || !s) return false;
    bool ok = pl->removeSong(s);
    if (ok) savePlaylists();
    return ok;
}
bool MusicSystem::addSong(const std::string& name, const std::string& artist, const std::string& album,
                                 const std::string& type, int durationSec, const std::string& dateRelease,
                                 const std::string& fileName){
    using musicdetail::trim;
    if (!isAdmin() || trim(name).empty() || trim(fileName).empty()) return false;
    std::vector<std::string> ids;
    for (Song* s : songs) ids.push_back(s->getIdSong());
    Song* s = new Song(musicdetail::nextId(ids, "S"), trim(name), trim(artist), trim(album), trim(type),
                       durationSec, trim(dateRelease), "assets/music/" + fileName, 0, 0);
    songs.push_back(s);
    songById.insert(s->getIdSong(), s);
    saveSongs();
    return true;
}
bool MusicSystem::removeSong(const std::string& idSong){
    if (!isAdmin()) return false;
    Song* s = findSong(idSong);
    if (!s) return false;

    for (User* u : users)
        if (Listener* l = dynamic_cast<Listener*>(u)){
            std::vector<std::string> ids;
            for (const auto& pl : l->getPlaylists()) ids.push_back(pl.getIdPlaylist());
            for (const auto& id : ids) l->getPlaylistById(id)->removeSong(s);
        }
    queue.remove(s);
    songById.remove(idSong);
    songs.erase(std::remove(songs.begin(), songs.end(), s), songs.end());
    delete s;
    saveSongs();
    savePlaylists();
    return true;
}
