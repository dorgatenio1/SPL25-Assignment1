#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = decks[1] = nullptr;
    active_deck = 0;
    std::cout <<"[MixingEngineService] Initialized with 2 empty decks.";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout <<"[MixingEngineService] Cleaning up decks.\n";
    if (decks[0]) {
    delete decks[0];
    decks[0] = nullptr;
    }
    if (decks[1]) {
    delete decks[1];
    decks[1] = nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout <<"\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> cloned = track.clone();
    //בדיקה אם השכפול נכשל
    if (!cloned) {
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    size_t target_deck = 1 - active_deck;
    bool active_exist = true;
    if (!decks[0] && !decks[1]) {
        target_deck = 0;
        active_exist = false;
    }
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;
    if (decks[target_deck]) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    cloned->load();
    cloned->analyze_beatgrid();

    if (active_exist && auto_sync) {
        if (!can_mix_tracks(cloned)) {
            sync_bpm(cloned);
        }
    }
    decks[target_deck] = cloned.release();
    std::string title = decks[target_deck]->get_title();
    std::cout << "[Load Complete] '" << title << "' is now loaded on deck " << target_deck << std::endl;
    if (active_exist) {
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr; 
    }
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;
    return active_deck; // Placeholder
}


/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (!decks[active_deck] || !track){
        return false;
    }
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();
    int bpm_diff = 0;
    if (new_track_bpm >= active_deck_bpm) {
        bpm_diff = new_track_bpm - active_deck_bpm;
    }
    else {
        bpm_diff = active_deck_bpm - new_track_bpm;
    }
    if (bpm_diff <= bpm_tolerance) {
        return true;
    }
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] && track) {
        int og_bpm = track->get_bpm();
        int avg_bpm = (decks[active_deck]->get_bpm() + og_bpm) / 2;
        track->set_bpm(avg_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << og_bpm << " to " << avg_bpm << std::endl;
    }
}
