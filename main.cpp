#include <iostream>
#include "include/song.h"
#include "include/playlist.h"
#include "include/listener.h"

int main(){
    Song song1("S001", "De danh cho em", "Dangrangto", "Mini-EP", "Ballad", 294, "2026-02-28", 414000);
    Song song2("S002", "Song 2", "Artist 2", "Album 2", "Pop", 200, "2026-03-01", 1000);
    Playlist playlist("P001", "My Playlist", "U001");
    playlist.addSong(song1);
    playlist.addSong(song2);

    Listener listener("U001", "MinhTriet", "triet@gmail.com", "0123456789", "123456", 100, 0);
    listener.addPlaylist(playlist);
    listener.playSong();
    listener.display();
    return 0;
}
