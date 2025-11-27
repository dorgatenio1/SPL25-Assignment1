#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // בדיקה אם השיר כבר קיים
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }    
    PointerWrapper<AudioTrack> new_track = track.clone();
    AudioTrack* cur_track = new_track.release();
    if (!cur_track) {
    std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to cline" << std::endl;
    return 0;
    }
    cur_track->load();
    cur_track->analyze_beatgrid();
    PointerWrapper<AudioTrack> final_track = PointerWrapper<AudioTrack>(cur_track);
    bool evicted = cache.put(std::move(final_track));
    if (evicted) {
        return -1;
    }
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* track = cache.get(track_title);
    if (track) {
        return track;
    }
    std::cout << "[ERROR] Track: \"" << track_title << "\" failed to clone" << std::endl;
    return nullptr; 
}
