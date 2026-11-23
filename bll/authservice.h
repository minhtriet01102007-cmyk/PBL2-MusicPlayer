#pragma once
#include <string>
#include <vector>
#include "admininfo.h"
#include "src/user.h"
#include "src/admin.h"
#include "src/listener.h"
#include "dal/userdao.h"
#include "dal/textutil.h"
#include "dsa/hashtable/hashtable.h"

class AuthService{
    public:
        enum LoginResult{
            LoginOk,
            LoginInvalid,
            LoginLocked
        };
    private:
        UserDAO dao;
        std::vector<User*> users; // Danh sách tất cả người dùng
        HashTable<std::string, User*> byName; // Bảng băm tìm kiếm người dùng theo tên đăng nhập
        HashTable<std::string, User*> byId;   // Bảng băm tìm kiếm người dùng theo ID
        User* current; // Người dùng hiện đang đăng nhập
        bool addUser(User* user); // Thêm người dùng vào hệ thống
    public:
        explicit AuthService(const std::string& root);
        ~AuthService();
        AuthService(const AuthService&) = delete;
        AuthService& operator=(const AuthService&) = delete;
        void load();
        bool save();
        LoginResult login(const std::string& username, const std::string& password);
        void logout();
        bool isLoggedIn() const;
        bool isAdmin() const;
        User* currentUser() const;
        Listener* currentListener() const;
        std::string currentUsername() const;
        std::string currentUserId() const;
        std::string currentEmail() const;
        std::string registerListener(const std::string& username, const std::string& email, const std::string& password);
        std::string changePassword(const std::string& oldPassword, const std::string& newPassword);
        User* findById(const std::string& id) const;
        std::vector<Listener*> listeners() const;
        void purgeSong(const std::string& idSong);
        void purgeArtist(const std::string& idArtist);
        std::vector<UserInfo> listUsers() const;
        std::string setLocked(const std::string& idUser, bool locked);
        int userCount() const;
};
AuthService::AuthService(const std::string& root) : dao(root){
    current = nullptr;
}
AuthService::~AuthService(){
    for (User* user : users){
        delete user;
    }
}
bool AuthService::addUser(User* user){
    // Kiểm tra tên đăng nhập đã tồn tại chưa
    if (byName.contains(user->getUsername())){
        delete user;
        return false;
    }
    // Kiểm tra ID đã tồn tại chưa
    if (byId.contains(user->getIdUser())){
        delete user;
        return false;
    }
    users.push_back(user); // Thêm người dùng vào danh sách 
    byName.insert(user->getUsername(), user); // Thêm vào bảng băm theo username
    byId.insert(user->getIdUser(), user); // Thêm vào bảng băm theo ID
    return true;
}
void AuthService::load(){
    current = nullptr; // Đăng xuất người dùng hiện tại
    for (User* user : users){
        delete user; // Xóa dữ liệu cũ
    }
    users.clear();
    byName.clear();
    byId.clear();
    // Đọc tất cả người dùng từ file
    std::vector<User*> data = dao.loadAll();
    // Đưa từng người dùng vào hệ thống
    for (User* user : data){
        addUser(user);
    }
}
bool AuthService::save(){
    return dao.saveAll(users); // Lưu danh sách người dùng xuống file
}
AuthService::LoginResult AuthService::login(const std::string& username, const std::string& password){
    User* user = nullptr;
    // Xóa khoảng trắng thừa ở tên đăng nhập
    std::string name = textutil::trim(username);
    // Tìm người dùng theo username
    bool found = byName.search(name, user);
    if (!found){ // Không tìm thấy
        return LoginInvalid;
    }
    if (!user->checkPassword(password)){  // Sai mật khẩu
        return LoginInvalid;
    }
    if (!user->isActive()){ // Kiểm tra tài khoản có bị khóa không
        return LoginLocked;
    }
    // Đăng nhập thành công
    current = user;
    return LoginOk;
}
void AuthService::logout(){
    current = nullptr;
}
bool AuthService::isLoggedIn() const{
    if (current == nullptr){
        return false;
    }
    else return true;
}
bool AuthService::isAdmin() const{
    if (current == nullptr){
        return false;
    }
    else return current->isAdmin();
}
User* AuthService::currentUser() const{
    return current;
}
Listener* AuthService::currentListener() const{
    // Chưa đăng nhập
    if (current == nullptr){
        return nullptr;
    }
    // Nếu là Admin thì không phải Listener
    if (current->isAdmin()){
        return nullptr;
    }
    // Nếu không phải Admin thì đây là Listener
    return static_cast<Listener*>(current);
}
std::string AuthService::currentUsername() const{
    if (current == nullptr){
        return "";
    }
    return current->getUsername();
}
std::string AuthService::currentUserId() const{
    if (current == nullptr){
        return "";
    }
    return current->getIdUser();
}
std::string AuthService::currentEmail() const{
    if (current == nullptr){
        return "";
    }
    return current->getEmail();
}
std::string AuthService::registerListener(const std::string& username, const std::string& email, const std::string& password){
    // Loại bỏ khoảng trắng thừa
    std::string name = textutil::trim(username);
    std::string mail = textutil::trim(email);
    // Kiểm tra độ dài username
    if (name.size() < 3 || name.size() > 20){
        return "Ten dang nhap phai tu 3 đen 20 ky tu";
    }
    // Kiểm tra username có khoảng trắng hoặc dấu | không
    if (name.find(' ') != std::string::npos || name.find('|') != std::string::npos){
        return "Ten đang nhap khong duoc chua khoang trang hoac ky tu '|'";
    }
    // Kiểm tra email
    if (mail.find('@') == std::string::npos || mail.find('.') == std::string::npos || mail.find('|') != std::string::npos){
        return "Email khong hop le";
    }
    // Kiểm tra mật khẩu
    if (password.size() < 4){
        return "Mat khau phai co it nhat 4 ky tu";
    }
    if (password.find('|') != std::string::npos){
        return "Mat khau khong duoc chua ky tu '|'";
    }
    // Kiểm tra username đã tồn tại
    if (byName.contains(name)){
        return "Ten dang nhap da ton tai";
    }
    // Lấy ID của tất cả người dùng
    std::vector<std::string> ids;
    for (User* user : users){
        ids.push_back(user->getIdUser());
    }
    // Tạo ID mới
    std::string id = textutil::nextId(ids, "U");
    // Tạo tài khoản Listener mới
    Listener* listener = new Listener(id, name, mail, password);
    // Thêm Listener vào hệ thống
    if (!addUser(listener)){
        return "Khong the tao tai khoan";
    }
    // Lưu dữ liệu xuống file
    if (!save()){
        return "Khong ghi duoc file ngưoi dung";
    }
    return "";
}
std::string AuthService::changePassword(const std::string& oldPassword, const std::string& newPassword){
    // Kiểm tra đã đăng nhập chưa
    if (current == nullptr){
        return "Ban chua dang nhap";
    }
    // Kiểm tra mật khẩu cũ
    if (!current->checkPassword(oldPassword)){
        return "Mat khau cu khong dung";
    }
    // Kiểm tra mật khẩu mới
    if (newPassword.size() < 4){
        return "Mat khau moi phai co it nhat 4 ky tu";
    }
    if (newPassword.find('|') != std::string::npos) {
        return "Mat khau khong duoc chua ky tu '|'";
    }
    // Cập nhật mật khẩu
    current->setPassword(newPassword);
    // Lưu dữ liệu
    if (!save()){
        return "Khong ghi duoc file nguoi dung";
    }
    return "";
}
User* AuthService::findById(const std::string& id) const{
    User* user = nullptr;
    // Tìm người dùng theo ID
    bool found = byId.search(id, user);
    if (!found){
        return nullptr;
    }
    return user;
}
std::vector<Listener*> AuthService::listeners() const{
    std::vector<Listener*> result;
    for (User* user : users) {
        // Chỉ lấy Listener, bỏ qua Admin
        if (!user->isAdmin()) {
            Listener* listener = static_cast<Listener*>(user);
            result.push_back(listener);
        }
    }
    return result;
}
void AuthService::purgeSong(const std::string& idSong){
    // Lấy danh sách tất cả Listener
    std::vector<Listener*> allListeners = listeners();
    // Xóa bài hát khỏi dữ liệu của từng Listener
    for (Listener* listener : allListeners){
        listener->removeSongReferences(idSong);
    }
}
void AuthService::purgeArtist(const std::string& idArtist){
    // Lấy danh sách tất cả Listener
    std::vector<Listener*> allListeners = listeners();
    // Xóa nghệ sĩ khỏi danh sách theo dõi của từng Listener
    for (Listener* listener : allListeners){
        listener->removeArtistReference(idArtist);
    }
}
std::vector<UserInfo> AuthService::listUsers() const{
    std::vector<UserInfo> result;
    // Chỉ Admin mới được xem danh sách
    if (!isAdmin()){
        return result;
    }
    for (User* user : users){
        UserInfo info;
        info.id = user->getIdUser();
        info.username = user->getUsername();
        info.email = user->getEmail();
        info.role = user->getRoleName();
        info.active = user->isActive();
        // Nếu là Listener thì lấy thêm thông tin
        if (!user->isAdmin()){
            Listener* listener = static_cast<Listener*>(user);
            info.totalPlayed = listener->getTotalSongsPlayed();
            info.playlistCount = (int)listener->getPlaylists().size();
        }
        result.push_back(info);
    }
    return result;
}
std::string AuthService::setLocked(const std::string& idUser, bool locked){
    // Kiểm tra quyền Admin
    if (!isAdmin()){
        return "Chi Admin moi co quyen thuc hien thao tac nay";
    }
    // Tìm người dùng theo ID
    User* user = findById(idUser);
    if (user == nullptr){
        return "Khong tim thay nguoi dung";
    }
    // Không cho phép khóa tài khoản Admin
    if (user->isAdmin()){
        return "Khong the khoa tai khoan Admin";
    }
    // locked = true: khóa tài khoản
    // locked = false: mở khóa tài khoản
    if (locked){
        user->setActive(false);
    }
    else{
        user->setActive(true);
    }
    // Lưu thay đổi
    if (!save()){
        return "Khong ghi duoc file nguoi dung";
    }
    return "";
}
int AuthService::userCount() const{
    return (int)users.size();
}

