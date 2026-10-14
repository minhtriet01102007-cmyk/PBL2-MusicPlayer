#pragma once
#include "song.h"

struct CompareSongByPlays{
    bool operator()(const Song& a, const Song& b) const;
};
inline bool CompareSongByPlays::operator()(const Song& a, const Song& b) const{
        return a.getCount() > b.getCount();
}