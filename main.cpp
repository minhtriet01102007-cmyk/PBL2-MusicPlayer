#include "include/Song.h"
#include <iostream>

int main(){
    Song song("S001","De danh cho em", "Dangrangto", "Mini-EP", "Ballad", 294, "2026-02-28", 414000);
    song.display();
    std::cout << "\nTang luot nghe...\n";
    song.increasePlayCount();
    std::cout << "Luot nghe hien tai: " << song.getCount() << std::endl;
    return 0;
}