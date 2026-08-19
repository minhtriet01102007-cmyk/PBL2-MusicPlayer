#include "include/song.h"
#include "include/artist.h"
#include "include/album.h"
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
    return 0;
}