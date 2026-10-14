#include <iostream>
#include "include/song.h"
#include "include/artist.h"
#include "include/album.h"
#include "include/user.h"
#include "include/playlist.h"
#include "include/listener.h"
#include "include/admin.h"

int main(){
    Song song1("S001", "De danh cho em", "Dangrangto", "Mini-EP", "Ballad", 294, "2026-02-28", 414000);
    Song song2("S002", "Song 2", "Artist 2", "Album 2", "Pop", 200, "2026-03-01", 100);

    Playlist playlist("P001", "My Playlist", "U001");
    playlist.addSong(song1);
    playlist.addSong(song2);

    Listener listener("U001", "MinhTriet", "triet@gmail.com", "0123456789", "123456", 100, 0);
    listener.addPlaylist(playlist);
    listener.playSong();

    Artist artist("A001", "Dangrangto", "Vietnam", 500000);
    Album album("AL001", "Mini-EP", "Dangrangto", "2026-02-28");
    Admin admin("U002", "Admin", "admin@gmail.com", "0987654321", "admin123", "Administrator");

    std::cout << "===== SONG =====\n";
    song1.display();

    std::cout << "\n===== ARTIST =====\n";
    artist.show();

    std::cout << "\n===== ALBUM =====\n";
    album.display();

    std::cout << "\n===== PLAYLIST =====\n";
    playlist.display();

    std::cout << "\n===== LISTENER =====\n";
    listener.display();

    std::cout << "\n===== ADMIN =====\n";
    admin.display();

    std::cout << "\n===== POLYMORPHISM =====\n";
    User* user1 = &listener;
    User* user2 = &admin;
    user1->display();
    std::cout << "--------------------\n";
    user2->display();

    std::cout << "\n===== SONG ACTION =====\n";
    song1.increasePlayCount();
    song1.like();
    std::cout << "Play count: " << song1.getCount() << '\n';
    std::cout << "Liked: " << (song1.isLiked() ? "Yes" : "No") << '\n';

    playlist.removeSong("S001");
    std::cout << "\n===== AFTER REMOVE SONG =====\n";
    playlist.display();
    return 0;
}
