# HỆ THỐNG PHÁT NHẠC VÀ QUẢN LÝ BÀI HÁT

## 1. Thông tin dự án
- Tên đề tài: Hệ thống phát nhạc và quản lý bài hát
- Môn học: PBL2

## 2. Giới thiệu
Hệ thống phát nhạc và quản lý bài hát được xây dựng bằng ngôn ngũ C++, áp dụng các kiến thức về:
- Lập trình hướng đối tượng (OOP)
- Cấu trúc dữ liệu và giải thuật (DSA)
- Quản lý file dữ liệu
- Giao diện người dùng với Qt
- Git và GitHub
Hệ thống cho phép người dùng quản lý thư viện bài hát, tạo playlist, phát nhạc, tìm kiếm, xem lịch sử nghe và nhận đề xuất bài hát.

## 3. Mục tiêu
Dự án hướng đến các mục tiêu:
- Áp dụng 4 tính chất của OOP.
- Áp dụng nhiều cấu trúc dữ liệu vào bài toán thực tế.
- Xây dựng hệ thống quản lý bài hát hoàn chỉnh.
- Xây dựng giao diện người dùng bằng Qt.
- Hỗ trợ phát nhạc trực tiếp.
- Lưu và đọc dữ liệu từ file.
- Xây dựng hệ thống tìm kiếm và sắp xếp.
- Xây dựng hệ thống đề xuất bài hát.
- Sử dụng Git/GitHub để quản lý mã nguồn nhóm.

## 4. Công nghệ sử dụng
- Ngôn ngữ C++: Ngôn ngữ lập trình chính 
- Qt: Xây dựng giao diện và xử lý phát nhạc
- Qt Multimedia: Phát và điều khiển file nhạc
- Git: Quản lý phiên bản 
- GitHub: Lưu trữ và quản lý mã nguồn 
- File `.txt`: Lưu trữ dữ liệu 

## 5. Cấu trúc dữ liệu và giải thuật
|   Cấu trúc dữ liệu |       Áp dụng     |                Chức năng                          |
|--------------------|-------------------|---------------------------------------------------|
|        Vector      |     Music Library |             Lưu toàn bộ bài hát                   |
| Doubly Linked List |      Playlist     | Thêm, xóa, Next, Previous, di chuyển bài          |
|        Queue       |      Play Queue   |           Quản lý danh sách chờ phát              |
|        Stack       | Listening History |             Lưu lịch sử nghe                      |
|     Hash Table     |      Search       |         Tìm kiếm nhanh theo ID/tên                |
|         BST        |    Music Library  |       Insert, Search, Delete, In-order            |
|        Graph       |    Recommendation | Biểu diễn quan hệ giữa bài hát, nghệ sĩ, thể loại |
|    Priority Queue  |    Recommendation |         Xếp hạng bài hát đề xuất                  |

### Searching
Hệ thống dự kiến hỗ trợ:
- Linear Search
- Binary Search
- Hash Search

### Sorting
Hệ thống hỗ trợ sắp xếp:
- Theo tên bài hát
- Theo lượt nghe
- Theo thời lượng
- Theo ngày phát hành

## 6. Áp dụng 4 tính chất OOP
### 6.1. Tính đóng gói
Các thuộc tính của đối tượng được đặt ở mức `private` và được truy cập thông qua các phương thức phù hợp.
### 6.2. Tính kế thừa
Lớp Admin và Listener kế thừa từ lớp cơ sở User.
User
├── Admin
└── Listener
### 6.3. Tính đa hình
Sử dụng virtual function và override để các loại người dùng có cách xử lý chức năng phù hợp.
### 6.4. Tính trừu tượng
Các thao tác quản lý có thể được định nghĩa thông qua abstract class hoặc interface. Mục tiêu là che giấu chi tiết triển khai và chỉ cung cấp các chức năng cần thiết cho đối tượng sử dụng.

## 7.Các chức năng chính
### 7.1. Quản lý bài hát
- Thêm bài hát
- Xóa bài hát
- Sửa thông tin bài hát
- Xem thông tin bài hát
- Xem toàn bộ thư viện bài hát
- Cập nhật lượt nghe
### 7.2. Music Player
- Play
- Pause
- Resume
- Stop
- Next
- Previous
- Replay
- Shuffle
- Repeat One
- Repeat All
- Điều chỉnh âm lượng
- Hiển thị bài hát đang phát
- Hiển thị thời gian phát
### 7.3. Playlist
- Tạo playlist
- Xóa playlist
- Đổi tên playlist
- Thêm bài hát vào playlist
- Xóa bài hát khỏi playlist
- Di chuyển vị trí bài hát
- Xem danh sách bài hát trong playlist
- Phát playlist
### 7.4. Play Queue
- Thêm bài hát vào hàng đợi
- Xóa bài hát khỏi hàng đợi
- Xem danh sách chờ phát
- Play Next
- Clear Queue
- Tự động phát bài tiếp theo
### 7.5. Listening History
- Lưu lịch sử nghe nhạc
- Xem lịch sử nghe
- Phát lại bài hát đã nghe
- Xóa một bài khỏi lịch sử
- Xóa toàn bộ lịch sử
### 7.6. Search
Có thể tìm kiếm bài hát theo:
- Song ID
- Tên bài hát
- Nghệ sĩ
- Album
- Thể loại
Hệ thống áp dụng:
- Linear Search
- Binary Search
- Hash Search
### 7.7. Sorting
Có thể sắp xếp danh sách bài hát theo:
- Tên bài hát A-Z
- Tên bài hát Z-A
- Lượt nghe tăng dần
- Lượt nghe giảm dần
- Thời lượng
- Ngày phát hành
### 7.8. Favorite
- Thêm bài hát vào danh sách yêu thích
- Xóa bài hát khỏi danh sách yêu thích
- Xem danh sách bài hát yêu thích
- Phát danh sách yêu thích
### 7.9. Recommendation
- Phân tích quan hệ giữa các bài hát
- Tìm bài hát có liên quan
- Tính điểm đề xuất
- Xếp hạng bài hát đề xuất
- Hiển thị Top bài hát đề xuất
- Phát bài hát được đề xuất
## 8. Phân quyền người dùng
SoundWave sử dụng hai quyền chính:
User
├── Admin
└── Listener
### 8.1. Admin
Admin chịu trách nhiệm quản lý dữ liệu và hệ thống.
* Quản lý bài hát
- Xem danh sách bài hát
- Thêm bài hát
- Sửa thông tin bài hát
- Xóa bài hát
- Cập nhật lượt nghe
- Tìm kiếm bài hát
- Sắp xếp bài hát
* Quản lý người dùng
- Xem danh sách người dùng
- Tìm kiếm người dùng
- Xem thông tin người dùng
- Khóa tài khoản
- Mở khóa tài khoản
- Xóa tài khoản
- Admin không được xem mật khẩu của người dùng.
* Quản lý nghệ sĩ
- Xem danh sách nghệ sĩ
- Thêm nghệ sĩ
- Sửa thông tin nghệ sĩ
- Xóa nghệ sĩ
* Quản lý album
- Xem danh sách album
- Thêm album
- Sửa album
- Xóa album
- Thêm bài hát vào album
- Xem danh sách bài hát trong album
* Thống kê hệ thống
- Tổng số bài hát
- Tổng số người dùng
- Tổng số playlist
- Tổng lượt nghe
- Top bài hát được nghe nhiều nhất
### 8.2. Listener
Listener sử dụng hệ thống để nghe nhạc và quản lý trải nghiệm cá nhân.
* Music Player
- Play
- Pause
- Resume
- Stop
- Next
- Previous
- Replay
- Shuffle
- Repeat One
- Repeat All
- Điều chỉnh âm lượng
- Search
- Tìm theo tên bài hát
- Tìm theo nghệ sĩ
- Tìm theo album
- Tìm theo thể loại
- Tìm theo Song ID
* Playlist
- Tạo playlist
- Đổi tên playlist
- Xóa playlist
- Thêm bài hát
- Xóa bài hát
- Di chuyển bài hát
- Xem playlist
- Phát playlist
* Play Queue
- Thêm bài hát vào queue
- Xóa bài hát
- Xem queue
- Play Next
- Clear Queue
* Listening History
- Lưu lịch sử nghe
- Xem lịch sử
- Phát lại bài hát
- Xóa lịch sử
* Favorite
- Add Favorite
- Remove Favorite
- Xem danh sách yêu thích
- Phát danh sách yêu thích
* Recommendation
- Nhận bài hát đề xuất
- Xem Top bài hát đề xuất
- Phát bài hát được đề xuất
## 9. Graph và hệ thống đề xuất
- Graph biểu diễn quan hệ giữa bài hát, nghệ sĩ, album và thể loại.
- Tìm các bài hát có liên quan đến bài đang nghe.
- Tính điểm đề xuất cho từng bài hát.
- Sử dụng Priority Queue để chọn Top bài hát đề xuất.

## 10. Quản lý file dữ liệu
- Lưu dữ liệu trong thư mục `data/`.
- Các file chính: `songs.txt`, `users.txt`, `artists.txt`, `albums.txt`, `playlists.txt`, `history.txt`.
- Đọc dữ liệu khi khởi động chương trình.
- Cập nhật và lưu dữ liệu khi chương trình thay đổi.

# 11. Giao diện người dùng
- Xây dựng giao diện bằng Qt.
- Màn hình chính: Home, Search, Library, Playlist, History, Favorites, Recommendation.
- Music Player hỗ trợ Play, Pause, Next, Previous, Volume và Progress Bar.
- Giao diện Admin và Listener có chức năng phù hợp với từng quyền.

