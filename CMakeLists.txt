cmake_minimum_required(VERSION 3.16)
project(PBL2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

# ${CMAKE_SOURCE_DIR}      : de playlist.h include duoc "dsa/dlinkedlist/dlinkedlist.h"
# ${CMAKE_SOURCE_DIR}/src  : song.h, user.h, admin.h, listener.h, playlist.h ...
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/service
    ${CMAKE_SOURCE_DIR}/ui
)

# Chi con header (de AUTOMOC xu ly Q_OBJECT va Qt Creator hien file trong cay du an)
file(GLOB UI_SRC      ui/*.h)
file(GLOB SERVICE_SRC service/*.h)

qt_add_executable(PBL2 main.cpp ${UI_SRC} ${SERVICE_SRC})
target_link_libraries(PBL2 PRIVATE Qt6::Widgets Qt6::Multimedia)

# Header entity cua ban chua co "inline" -> cho phep trung dinh nghia khi lien ket (MinGW/GCC)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(PBL2 PRIVATE -Wl,--allow-multiple-definition)
endif()

# Thu muc goc du an (chua data/ va assets/music/) -> tranh loi khong tim thay file khi chay tu thu muc build
target_compile_definitions(PBL2 PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")
