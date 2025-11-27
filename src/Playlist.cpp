  
  #include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
//בנאי מעתיק
Playlist::Playlist(const Playlist& other) : head(nullptr), playlist_name(other.playlist_name), track_count(0) {
    

    if (!other.head)
        return;

    PlaylistNode* cur = other.head;
    PlaylistNode* tail = nullptr;

    while (cur) {
        AudioTrack* new_track = cur->track->clone().release();
        PlaylistNode* new_node = new PlaylistNode(new_track);
        
        if (!this->head) {
        this->head = new_node;
        tail = new_node;
        }
        else{
            tail->next = new_node;
            tail = new_node;
        }
        this ->track_count++;
        cur = cur->next;
    }
}

// בנאי השמה
Playlist&  Playlist::operator=(const Playlist& other) {
    if (this == &other) 
    return *this;
    clear(); 

    this->head = nullptr;
    this->playlist_name = other.playlist_name;
    this->track_count = 0;

    if (!other.head)
        return *this;

    PlaylistNode* cur = other.head;
    PlaylistNode* tail = nullptr;

    while (cur) {
        AudioTrack* new_track = cur->track->clone().release();
        PlaylistNode* new_node = new PlaylistNode(new_track);
        
        if (!this->head) {
        this->head = new_node;
        tail = new_node;
        }
        else{
            tail->next = new_node;
            tail = new_node;
        }
        this ->track_count++;
        cur = cur->next;
    }

    return *this; 
}

// Move Constructor בנאי הזזה 
Playlist::Playlist(Playlist&& other) noexcept
    : head(other.head), 
      playlist_name(std::move(other.playlist_name)), 
      track_count(other.track_count) {
    
    
    other.head = nullptr;
    other.track_count = 0;

    
      }

//Move Assignment Operator 
Playlist& Playlist::operator=(Playlist&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear(); 

   
    head = other.head;
    playlist_name = std::move(other.playlist_name);
    track_count = other.track_count;

    
    other.head = nullptr;
    other.track_count = 0;

    return *this;
}


// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    clear();

    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif
}

// מימוש הפונקציה למחיקה 
void Playlist::clear() {
     PlaylistNode* current = head;
     while (current) {
        PlaylistNode* nxt = current->next;
        delete current->track;
        delete current;
        current = nxt;
    }
    head = nullptr;
    track_count =0; 
}

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }

        
        track_count--;

        
    /*הוספנו מחיקה של הצומת מהערימה*/

        delete current->track ;
        delete current ;


        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }

    


}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}


AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}