#include "include/song.h"
#include "include/artist.h"
#include "include/album.h"
#include "include/listening_history.h"
#include <iostream>

int main(){
    Song song("S001", "De danh cho em", "Dangrangto", "Mini-EP", "Ballad", 294, "2026-02-28", 414000);
    song.display();
    std::cout << "\nTang luot nghe...\n";
    song.increasePlayCount();
    std::cout << "Luot nghe hien tai: " << song.getCount() << std::endl;
    std::cout << "=================================" << "\n";
    Artist artist("A001", "Dangrangto", " ", "VietNam", 100);
    artist.show();
    std::cout << "=================================" << std::endl;
    Album album("B001", "Mini-EP", "Dangrangto", "2026-02-28", "Ballad");
    
    std::cout << "\n===== LISTENING HISTORY =====\n";
    Listening_History history;
    history.push(song);
    Song song2("S002", "Bai hat thu hai", "Artist 2", "Album 2", "Pop", 200, "2026-03-01", 1000);
    history.push(song2);
    std::cout << "So bai hat trong lich su: "
              << history.getSize() << std::endl;
    Song* topSong = history.getTop();
    if (topSong != nullptr){
        std::cout << "\nBai hat moi nghe gan nhat:\n";
        topSong->display();
    }
    std::cout << "\nXoa bai hat moi nhat...\n";
    history.pop();
    std::cout << "So bai hat con lai: "
              << history.getSize() << std::endl;
    return 0;
}