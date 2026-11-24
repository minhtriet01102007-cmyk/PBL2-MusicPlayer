cmake_minimum_required(VERSION 3.16)
project(PBL2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tầng UI không dùng Q_OBJECT / signals / slots (thay bằng std::function) nên KHÔNG cần moc.
# Nhờ vậy mọi header (src, dsa, dal, bll, ui) chỉ được biên dịch trong đúng một file: main.cpp,
# và việc định nghĩa hàm ngay trong .h (không inline) không gây lỗi "multiple definition".
# Nếu sau này bạn thêm Q_OBJECT: bật AUTOMOC và tách lớp đó ra cặp .h/.cpp riêng.
set(CMAKE_AUTOMOC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

# ${CMAKE_SOURCE_DIR} để include được "src/...", "dsa/...", "dal/...", "bll/...", "ui/..."
include_directories(${CMAKE_SOURCE_DIR})

# Chỉ liệt kê header để Qt Creator hiện file trong cây dự án (không biên dịch riêng từng file)
file(GLOB SRC_HDR src/*.h)
file(GLOB DSA_HDR dsa/*/*.h)
file(GLOB DAL_HDR dal/*.h)
file(GLOB BLL_HDR bll/*.h)
file(GLOB UI_HDR  ui/*.h)

qt_add_executable(PBL2 main.cpp ${SRC_HDR} ${DSA_HDR} ${DAL_HDR} ${BLL_HDR} ${UI_HDR})
target_link_libraries(PBL2 PRIVATE Qt6::Widgets Qt6::Multimedia)

# Thư mục gốc dự án (chứa data/ và assets/music/), tránh lỗi không tìm thấy file khi chạy từ thư mục build
target_compile_definitions(PBL2 PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")

# Ẩn cửa sổ console trên Windows (bật bằng: cmake -DPBL2_HIDE_CONSOLE=ON ...)
option(PBL2_HIDE_CONSOLE "Ẩn cửa sổ console trên Windows" OFF)
if(PBL2_HIDE_CONSOLE)
    set_target_properties(PBL2 PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)
endif()

# Mã nguồn có tiếng Việt có dấu (UTF-8)
if(MSVC)
    target_compile_options(PBL2 PRIVATE /utf-8 /permissive-)
endif()