#include "include/song.h"
#include "include/artist.h"
#include "include/album.h"
#include "include/listening_history.h"
#include "include/queue.h"
#include "include/playlist.h"
#include "include/search.h"
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
    Song song1("S002", "Bai hat thu hai", "Artist 2", "Album 2", "Pop", 200, "2026-03-01", 1000);
    history.push(song1);
    std::cout << "So bai hat trong lich su: "
              << history.getSize() << std::endl;
    Song* topSong = history.getTop();
    if (topSong != nullptr){
        std::cout << "\nBai hat moi nghe gan nhat:\n";
        topSong->display();
    }
    std::cout << "\nXoa bai hat moi nhat...\n";
    history.pop();
    std::cout << "So bai hat con lai: " << history.getSize() << std::endl;

    std::cout << "\n===== PLAY QUEUE =====\n";

    Queue Q;
    Q.enqueue(song);
    Song song2("S002", "Bai hat thu hai", "Artist 2",   "Album 2", "Pop", 200,  
               "2026-03-01", 1000);
    Song song3("S003", "Bai hat thu ba", "Artist 3", "Album 3", "Rock", 180,
               "2026-03-02", 2000);
    Q.enqueue(song2);
    Q.enqueue(song3);   
    std::cout << "So bai hat trong hang doi: "  << Q.getSize() << std::endl;
    Song* frontSong = Q.getFront();
    if (frontSong != nullptr){
        std::cout << "\nBai hat se phat tiep theo:\n";
        frontSong->display();
    }
    std::cout << "\nPhat xong bai hat dau tien...\n";
    Q.dequeue();
    std::cout << "So bai hat con lai: " << Q.getSize() << std::endl;
    frontSong = Q.getFront();
    if (frontSong != nullptr){
        std::cout << "\nBai hat tiep theo:\n";
        frontSong->display();
    }

    Playlist playlist;
    playlist.add(song);
    playlist.add(song2);
    playlist.add(song3);
    std::cout << "So bai hat trong Playlist: "  << playlist.getSize() << std::endl;
    playlist.display();
    std::cout << "\nXoa bai hat o vi tri 1...\n";
    playlist.remove(1);
    std::cout << "\nPlaylist sau khi xoa:\n";
    playlist.display(); 
    std::cout << "So bai hat con lai: " << playlist.getSize() << std::endl;

    Song songs[3] = {Song("S001", "De danh cho em", "Dangrangto", 
        "Mini-EP", "Ballad", 294, "2026-02-28", 414000),
    Song("S002", "Bai hat 2", "Artist 2",
         "Album 2", "Pop", 200, "2026-01-01", 1000),
    Song("S003", "Bai hat 3", "Artist 3",
         "Album 3", "Rock", 250, "2026-01-02", 2000)
    };
    std::string id_song;
    std::cout << "Nhap ID bai hat can tim: ";
    std::cin >> id_song;
    int index = Search::linearSearchById(songs, 3, id_song);
    if (index != -1){
        std::cout << "Tim thay bai hat tai vi tri: " << index << std::endl;
        songs[index].display();
    }
    else{
        std::cout << "Khong tim thay bai hat!" << std::endl;
    }
    return 0;
}