cmake_minimum_required(VERSION 3.16)

project(MusicSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Multimedia
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    music_service.h
    comparesongs.h
    src/song.h
    src/user.h
    src/listener.h
    src/admin.h
    src/album.h
    src/artist.h
    src/playlist.h
    dsa/dlinkedlist/dlinkedlist.h
    dsa/hashtable/hashtable.h
    dsa/maxheap/maxheap.h
)

add_executable(MusicSystem
    ${PROJECT_SOURCES}
)

target_include_directories(MusicSystem PRIVATE
    .
    src
    dsa
    dsa/dlinkedlist
    dsa/hashtable
    dsa/maxheap
)

# Link các thư viện Qt cần thiết
target_link_libraries(MusicSystem PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
)

set_target_properties(MusicSystem PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
