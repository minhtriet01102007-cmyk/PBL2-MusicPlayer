#pragma once

#include "user.h"
#include "playlist.h"
#include <string>
#include <vector>

class Listener : public User{
private:
    long long followers;
    int total_songs_played;
    std::vector<Playlist> playlists;

public:
    Listener();

    Listener(std::string id_user,
             std::string username,
             std::string email,
             std::string phone_number,
             std::string password,
             long long followers,
             int total_songs_played);

    Listener(const Listener& listener);

    ~Listener() override;

    long long getFollowers() const;
    int getTotalSongsPlayed() const;
    int getPlaylistCount() const;

    void setFollowers(long long followers);
    void setTotalSongsPlayed(int total_songs_played);

    void playSong();

    void addPlaylist(const Playlist& playlist);
    void removePlaylist(std::string id_playlist);

    const std::vector<Playlist>& getPlaylists() const;

    void display() const override;
};