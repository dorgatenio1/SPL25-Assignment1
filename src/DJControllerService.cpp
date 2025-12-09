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
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }
    PointerWrapper<AudioTrack> new_track = track.clone();
    if (!new_track) {
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return 0;
    }
    new_track->load();
    new_track->analyze_beatgrid();
    size_t size_before = cache.size();
    cache.put(std::move(new_track));
    size_t size_after = cache.size();
    if (size_after > size_before) {
        return 0;
    } else {
        return -1;
    }
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
