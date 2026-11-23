#pragma once
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include "songinfo.h"
#include "authservice.h"
#include "songservice.h"
#include "playlistservice.h"
#include "dsa/dlinkedlist/dlinkedlist.h"
#include "dsa/dlinkedlist/dlistutil.h"

// PlayerService quản lý nghiệp vụ phát nhạc, lớp này quyết định bài hát nào sẽ được phát tiếp theo.
class PlayerService{
    public:
        enum RepeatMode{
            RepeatNone,  // Không lặp
            RepeatAll,   // Lặp toàn bộ danh sách
            RepeatOne    // Lặp lại bài hiện tại
        };
    private:
        AuthService& auth;
        SongService& songs;
        PlaylistService& playlists;
        DoublyLinkedList<std::string> queue;
        std::vector<std::string> sourceIds;
        std::string currentId;
        bool shuffle;
        RepeatMode repeatMode;
        std::mt19937 rng;
        bool rebuild(const std::string& startId);
        bool moveTo(const std::string& id, SongInfo& out);
        bool fillQueue(const std::vector<std::string>& order, const std::string& focusId);
        std::vector<std::string> queueIds() const;
    public:
        PlayerService(AuthService& auth, SongService& songs, PlaylistService& playlists);
        void reset();
        bool startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out);
        bool startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out);
        bool next(SongInfo& out);
        bool prev(SongInfo& out);
        bool onTrackFinished(SongInfo& out);
        bool getCurrent(SongInfo& out) const;
        void setShuffle(bool on);
        bool isShuffle() const;
        void setRepeatMode(RepeatMode mode);
        RepeatMode getRepeatMode() const;
        bool playNext(const std::string& idSong);
        bool addToQueue(const std::string& idSong);
        std::vector<SongInfo> upcoming() const;
        void recordPlay(const std::string& idSong);
        void purgeSong(const std::string& idSong);
};
PlayerService::PlayerService(AuthService& auth, SongService& songs, PlaylistService& playlists) : auth(auth), songs(songs), playlists(playlists),
      shuffle(false), repeatMode(RepeatNone), rng(std::random_device{}()){
}
void PlayerService::reset(){
    queue.clear();
    sourceIds.clear();
    currentId = "";
    shuffle = false;
    repeatMode = RepeatNone;
}
bool PlayerService::fillQueue(const std::vector<std::string>& order, const std::string& focusId){
    queue.clear();
    // Đưa từng bài hát vào hàng đợi.
    for (const auto& id : order) {
        queue.pushBack(id);
    }
    // Di chuyển con trỏ current đến bài cần phát.
    return dlist::seekTo(queue, focusId);
}
std::vector<std::string> PlayerService::queueIds() const{
    DoublyLinkedList<std::string> copy = queue;
    return dlist::toVector(copy);
}
bool PlayerService::rebuild(const std::string& startId){ // Xây dựng lại hàng đợi khi bật hoặc tắt shuffle
    std::vector<std::string> order;
    // Chỉ lấy những bài hát vẫn còn tồn tại.
    for (const auto& id : sourceIds){
        Song* song = songs.findSong(id);
        if (song != nullptr){
            order.push_back(id);
        }
    }
    // Nếu đang bật shuffle thì xáo trộn danh sách.
    if (shuffle){
        // Tìm vị trí bài hát đang phát.
        auto it = std::find(order.begin(), order.end(), startId);
        // Loại bài đang phát ra khỏi danh sách.
        if (it != order.end()){
            order.erase(it);
        }
        // Xáo trộn các bài còn lại.
        std::shuffle(order.begin(), order.end(), rng);
        // Đưa bài đang phát lên đầu tiên.
        order.insert(order.begin(), startId);
    }
    // Nạp lại hàng đợi và đặt con trỏ vào startId.
    bool success = fillQueue(order, startId);
    if (!success){
        return false;
    }
    currentId = startId;
    return true;
}
bool PlayerService::moveTo(const std::string& id, SongInfo& out){
    Song* song = songs.findSong(id);
    // Không tìm thấy bài hát.
    if (song == nullptr){
        return false;
    }
    currentId = id; // Cập nhật bài hát hiện tại
    out = songs.toInfo(*song, true); // Lấy thông tin bài hát, bao gồm đường dẫn file
    return true;
}
bool PlayerService::startQueue(const std::vector<std::string>& ids, const std::string& startId, SongInfo& out){
    std::string start;
    // Nếu không truyền startId thì phát bài đầu tiên.
    if (startId.empty()){
        if (ids.empty()){
            return false;
        }
        start = ids.front();
    }
    else{
        start = startId;
    }
    // Kiểm tra bài hát bắt đầu có tồn tại không.
    if (songs.findSong(start) == nullptr){
        return false;
    }
    sourceIds = ids; // Lưu danh sách gốc.
    bool success = rebuild(start); // Xây dựng hàng đợi.
    if (!success){
        return false;
    }
    return moveTo(start, out); // Chuyển sang bài hát bắt đầu.
}
bool PlayerService::startPlaylist(const std::string& idPlaylist, const std::string& startId, SongInfo& out){
    std::vector<std::string> ids = playlists.songIdsOf(idPlaylist); // Lấy danh sách ID bài hát trong playlist.
    return startQueue(ids, startId, out); // Sử dụng lại chức năng phát danh sách thông thường.
}
bool PlayerService::next(SongInfo& out){
    if (queue.isEmpty() || currentId.empty()){
        return false;
    }
    std::string nextId;
    // Nếu còn bài phía sau thì chuyển sang bài tiếp theo.
    if (queue.hasNext()){
        nextId = queue.next();
    }
    else{
        // Nếu hết danh sách và bật RepeatAll.
        if (repeatMode == RepeatAll){
            queue.resetPlayback(); // Quay con trỏ về đầu danh sách.
            nextId = queue.getCurrent();
        }
        else{
            return false;
        }
    }
    return moveTo(nextId, out);
}
bool PlayerService::prev(SongInfo& out){
    if (queue.isEmpty() || currentId.empty()){
        return false;
    }
    std::string prevId;
    // Nếu còn bài phía trước thì chuyển về bài trước.
    if (queue.hasPrev()){
        prevId = queue.prev();
    }
    else{
        // Nếu đang ở đầu danh sách và bật RepeatAll.
        if (repeatMode == RepeatAll){
            std::vector<std::string> ids = queueIds();
            // Lấy bài hát cuối cùng.
            if (ids.empty()){
                return false;
            }
            prevId = ids.back();
            // Di chuyển con trỏ đến bài cuối.
            dlist::seekTo(queue, prevId);
        }
        else{
            return false;
        }
    }
    return moveTo(prevId, out);
}
bool PlayerService::onTrackFinished(SongInfo& out){
    // RepeatOne: phát lại bài hiện tại
    if (repeatMode == RepeatOne){
        return moveTo(currentId, out);
    }
    return next(out); // Các chế độ còn lại chuyển sang bài tiếp theo
}
bool PlayerService::getCurrent(SongInfo& out) const{
    Song* song = songs.findSong(currentId);
    if (song == nullptr){
        return false;
    }
    out = songs.toInfo(*song, true);
    return true;
}
void PlayerService::setShuffle(bool on){
    if (shuffle == on){
        return;
    }
    shuffle = on;
    // Nếu đang có bài hát thì xây dựng lại hàng đợi
    if (!currentId.empty() && !sourceIds.empty()){
        rebuild(currentId);
    }
}
bool PlayerService::isShuffle() const{
    return shuffle;
}
void PlayerService::setRepeatMode(RepeatMode mode){
    repeatMode = mode;
}
PlayerService::RepeatMode
PlayerService::getRepeatMode() const{
    return repeatMode;
}
bool PlayerService::playNext(const std::string& idSong){
    if (queue.isEmpty() || currentId.empty()){
        return false;
    }
    if (songs.findSong(idSong) == nullptr){
        return false;
    }
    // Nếu bài được chọn chính là bài hiện tại thì không cần thay đổi.
    if (idSong == currentId){
        return true;
    }
    std::vector<std::string> order = queueIds(); // Lấy danh sách ID hiện tại
    order.erase(std::remove(order.begin(), order.end(), idSong), order.end());  // Xóa bài hát cũ nếu nó đã tồn tại trong hàng đợi
    auto it = std::find(order.begin(), order.end(), currentId); // Tìm vị trí bài đang phát
    // Chèn bài mới ngay sau bài đang phát
    if (it != order.end()){
        order.insert(it + 1, idSong);
    }
    else{
        order.push_back(idSong);
    }
    fillQueue(order, currentId); // Nạp lại hàng đợi, giữ nguyên bài hiện tại
    // Cập nhật danh sách gốc để phục vụ shuffle.
    sourceIds.erase(std::remove(sourceIds.begin(), sourceIds.end(), idSong), sourceIds.end());
    sourceIds.push_back(idSong);
    return true;
}
bool PlayerService::addToQueue(const std::string& idSong){
    if (queue.isEmpty() || currentId.empty()){
        return false;
    }
    if (songs.findSong(idSong) == nullptr){   // Kiểm tra bài hát có tồn tại không
        return false;
    }
    if (queue.contains(idSong)){ // Không thêm nếu bài hát đã có trong hàng đợi
        return false;
    }
    queue.pushBack(idSong);
    sourceIds.push_back(idSong);
    return true;
}
// Lấy danh sách những bài hát sắp được phát
std::vector<SongInfo>
PlayerService::upcoming() const{
    std::vector<SongInfo> result;
    bool passedCurrent = false;
    std::vector<std::string> ids = queueIds();
    for (const auto& id : ids){
        // Bỏ qua các bài đứng trước bài hiện tại.
        if (!passedCurrent){
            if (id == currentId){
                passedCurrent = true;
            }
            continue;
        }
        // Từ vị trí tiếp theo trở đi là các bài sắp phát.
        Song* song = songs.findSong(id);
        if (song != nullptr){
            result.push_back(songs.toInfo(*song));
        }
    }
    return result;
}
// Ghi nhận một lượt nghe bài hát.
void PlayerService::recordPlay(const std::string& idSong){
    if (songs.findSong(idSong) == nullptr){ // Không ghi nhận nếu bài hát không tồn tại
        return;
    }
    songs.increasePlays(idSong); // Tăng tổng lượt nghe của bài hát
    Listener* listener = auth.currentListener(); // Lấy người dùng đang đăng nhập
    if (listener != nullptr){
        listener->increasePlayedCount(); // Tăng số bài hát người dùng đã nghe
        listener->addRecentSong(idSong); // Thêm bài hát vào lịch sử nghe gần đây
        auth.save(); // Lưu lại thông tin người dùng
    }
}
// Xóa bài hát khỏi hàng đợi khi bài hát bị xóa khỏi hệ thống.
void PlayerService::purgeSong(const std::string& idSong){
    // Xóa tất cả vị trí có chứa ID bài hát.
    while (queue.remove(idSong)){}
    // Xóa ID khỏi danh sách gốc.
    sourceIds.erase(std::remove(sourceIds.begin(), sourceIds.end(), idSong), sourceIds.end());
    // Nếu bài bị xóa đang phát thì xóa trạng thái hiện tại.
    if (currentId == idSong){
        currentId = "";
    }
}
