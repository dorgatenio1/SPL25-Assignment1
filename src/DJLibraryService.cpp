#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
//הוספנו בנאי הורס כי המחלקה היא הבעלים של הספרייה שהיא מערך של AudioTrack
DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}

DJLibraryService::DJLibraryService(const DJLibraryService& other) {
    playlist = other.playlist;
    for (const AudioTrack* original_track : other.library) {
        if (original_track) {
            PointerWrapper<AudioTrack> cloned_track = original_track->clone();
            library.push_back(cloned_track.release());
        }
    }
}

DJLibraryService&  DJLibraryService::operator=(const DJLibraryService& other) {
    if (this != &other) {
        for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
    playlist = other.playlist;
    for (const AudioTrack* original_track : other.library) {
        if (original_track) {
            PointerWrapper<AudioTrack> cloned_track = original_track->clone();
            library.push_back(cloned_track.release());
            }
        }
    }
    return *this;
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
      for (AudioTrack* track : library) {
            delete track;
        }
        library.clear();

    for (const SessionConfig::TrackInfo& info : library_tracks) {

        AudioTrack* new_track = nullptr;
        bool has_tags = false;
        if (info.extra_param2 == 1) {
            has_tags = true;
        }
        if (info.type == "MP3") {
            new_track = new MP3Track(info.title,info.artists,info.duration_seconds,info.bpm,info.extra_param1,has_tags);
            //std::cout <<"MP3Track created: " << info.extra_param1 << " kbps" << std::endl;
        }
        if (info.type == "WAV"){
            new_track = new WAVTrack(info.title,info.artists,info.duration_seconds,info.bpm,info.extra_param1,info.extra_param2);
            //std::cout << "WAVTrack created: "<< info.extra_param1 << "Hz/" << info.extra_param2 << "bit" << std::endl;
        }

        if (new_track) {
            library.push_back(new_track);
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    AudioTrack* track = playlist.find_track(track_title); 
    // Your implementation here
    return track; // Placeholder
}
void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name,
                                               const std::vector<int>& track_indices) {
   
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
   
    
    for (int index : track_indices) {
       
        if (index < 1 || static_cast<size_t>(index) > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue;
        }

        AudioTrack* curr_org = library[index-1];
        PointerWrapper<AudioTrack> clone_track = curr_org->clone();

        AudioTrack* Un_Wrap_track = clone_track.release();
       
        if (!Un_Wrap_track) {
            std::cerr << "[ERROR] Failed to clone track" << std::endl;
        }
        else {
            Un_Wrap_track->load();
            Un_Wrap_track->analyze_beatgrid();
            playlist.add_track(Un_Wrap_track);
           
            
        }          
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name
              << " (" << playlist.get_track_count() << " tracks)" << std::endl;
}
// void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
//                                                const std::vector<int>& track_indices) {
//     // Your implementation here
//    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;

//    //בשביל זה היינו צריכים לממש את חוק ה-5 בפליליסט
//     playlist = Playlist(playlist_name);
    
    
//     // לולאה מהסוף להתחלה שהישירים יכנסו בסדר הנכון 
//     for (int i = track_indices.size() - 1; i >= 0; --i) {
        
//         int index = track_indices[i];

//         if (!(index >= 1 && (size_t)index <= library.size())) {
//             std::cout << "[WARNING] Invalid track index: " << index << std::endl;
//         }
//         else {
//             AudioTrack* curr_org = library[index-1]; 
//             PointerWrapper<AudioTrack> clone_track = curr_org -> clone(); 

//             AudioTrack* Un_Wrap_track = clone_track.release();
//             if(!Un_Wrap_track) {
//                 std::cerr << "[ERROR] Failed to clone track" <<std::endl;
//             }
//             else{ 
//                 Un_Wrap_track->load(); 
//                 Un_Wrap_track->analyze_beatgrid();
//                 playlist.add_track(Un_Wrap_track); 
//                 std::cout << "Added '" << Un_Wrap_track->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
//             }           
//         }
//     }

    
//     std::cout << "[INFO] Playlist loaded: " << playlist_name 
//               << " (" << playlist.get_track_count() << " tracks)" << std::endl;
// }
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> TrackTitles ; 
    std::vector<AudioTrack*> tracks = playlist.getTracks();


    for( AudioTrack* track : tracks  ){
        if (track) 
            TrackTitles.push_back(track ->get_title()); 
    }
    return TrackTitles ; // Placeholder


}
