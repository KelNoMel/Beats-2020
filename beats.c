// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by Kellen Liew (z5312656)
// on 26/04/20
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.
#define HIGHER 2
#define LOWER 3
//#define END_OF_BEAT 4
////////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    struct beat *curr;
    struct beat *head;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    struct note *notes;
    struct beat *next;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.

////////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.

// Checks if note is valid
int check_valid_note(int octave, int key, int high_octave, int high_key); 

// Checks the status of a track
int status_checker(Track track);

// Checks whether a note is higher or lower than another
int higher_or_lower(struct note note_a, struct note note_b);

// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat. (a)
int add_note_to_beat(Beat beat, int octave, int key) {
    if (beat->notes == NULL) {     // Initializing first note.
        int high_octave = -1;
        int high_key = -1;
        
        int outcome = check_valid_note(octave, key, high_octave, high_key);
        if (outcome == VALID_NOTE) {
            struct note *new_note = malloc(sizeof(struct note));
            new_note->octave = octave; // Construct note to add.
            new_note->key = key;
            beat->notes = new_note;
            new_note->next = NULL;
        }
        return outcome;
        
    } else {       // For adding notes after track->curr is initialized.
        struct note *new_note = malloc(sizeof(struct note));
        struct note *cursor = beat->notes;
        int high_octave = cursor->octave;  // Determining highest notes.
        int high_key = cursor->key;
        while (cursor->next != NULL) {
            if (octave != -1) {
                cursor = cursor->next;
            }
            high_octave = cursor->octave;
            high_key = cursor->key;
            
        }

        int outcome = check_valid_note(octave, key, high_octave, high_key);
        if (outcome == VALID_NOTE) {
            new_note->octave = octave; // Construct note to add.
            new_note->key = key;
            new_note->next = NULL;
            cursor->next = new_note;
        }
        return outcome;
    }
}

// Print the contents of a beat. (p)
void print_beat(Beat beat) {
    struct note *print = beat->notes;
    while (print != NULL) {
        
        printf("%d ", print->octave);
        if (print->key < 10) {
            printf("0");
        }
        printf("%d", print->key);
        if (print->next != NULL) {
            printf(" | ");
        }
        print = print->next;
    }
    printf("\n");
    return;
}

// Count the number of notes in a beat that are in a given octave. (c)
int count_notes_in_octave(Beat beat, int octave) {
    int number = 0;
    struct note *count = beat->notes;
    while (count != NULL) {
        if (count->octave == octave) {
            number++;
        }
        count = count->next;
    }
    return number;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    struct track *new_track = malloc(sizeof (struct track));
    assert(new_track != NULL);
    
    new_track->curr = NULL;
    new_track->head = NULL;

    return new_track;
}

// Add a beat after the current beat in a track. (A)
void add_beat_to_track(Track track, Beat beat) {
    struct beat *cursor = track->curr;
    if (cursor == NULL && track->head == NULL) {
        track->head = beat;
        
    } else if (cursor == NULL && track->head != NULL) {
        beat->next = track->head;
        track->head = beat;

    } else if (cursor != NULL && cursor->next == NULL) {
        cursor->next = beat;
    
    } else if (cursor != NULL && cursor->next != NULL) {
        beat->next = cursor->next;
        cursor->next = beat;
    }
        
    return;
}

// Set a track's current beat to the next beat. (>)
int select_next_beat(Track track) {
    int result = 0;
    struct beat *cursor = track->curr;
    
    if (track->curr == NULL && track->head != NULL) {
        track->curr = track->head;
        result = TRACK_PLAYING;
    
    } else if (track->curr == NULL && track->head == NULL) {
        result = TRACK_STOPPED;

    } else if (track->curr != NULL && cursor->next != NULL) {
        track->curr = cursor->next;
        result = TRACK_PLAYING;
    
    } else if (track->curr != NULL && cursor->next == NULL) {
        track->curr = cursor->next;
        result = TRACK_STOPPED;
    }
    return result;
}

// Print the contents of a track. (P)
void print_track(Track track) {
    struct beat *cursor = track->head;
    int count = 1;
    
    while (cursor != NULL) {
        if (cursor == track->curr) {
            printf(">");
        } else if (cursor != track->curr) {
            printf(" ");
        }
        
        printf("[%d] ", count);
        
        print_beat(cursor);
        cursor = cursor->next;
        count++;
    }
    return;
}

// Count beats after the current beat in a track. (C)
int count_beats_left_in_track(Track track) {
    int beat_count = 0;
    int track_status = status_checker(track);
    struct beat *count = track->head;
    
    if (track_status == TRACK_STOPPED && track->head != NULL) {
        while (count != NULL) {
            beat_count++;
            count = count->next;
        }
    
    } else if (track_status == TRACK_STOPPED && track->head == NULL) {
        beat_count = 0;
    
    } else if (track_status == TRACK_PLAYING) {
        count = track->curr;
        count = count->next;
        while (count != NULL) {
            beat_count++;
            count = count->next;
        }
    }
    
    return beat_count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    struct note *delete_note = beat->notes;
    while (delete_note != NULL) {
        struct note *next_link = delete_note->next;
        free(delete_note);
        delete_note = next_link;
    }
    free(beat);
    
    return;
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    struct beat *delete_beat = track->head;
    while (delete_beat != NULL) {
        struct beat *next_link = delete_beat->next;
        free_beat(delete_beat);
        delete_beat = next_link;
    }
    free(track);
    return;
}

// Remove the currently selected beat from a track. (R)
int remove_selected_beat(Track track) {
    struct beat *removal_target = track->curr;
    struct beat *cursor = track->head;
    int result = 0;
    
    if (track->head == removal_target && track->head != NULL) {
        track->head = removal_target->next; // Linking adjacent beats.
        
        result = select_next_beat(track); // Move current beat up
        free_beat(removal_target); // Free the current beat.
    
    } else if (removal_target == NULL) {
        result = TRACK_STOPPED;
    
    } else {
        while (cursor->next != removal_target) {    // Linking beats adjacent
            cursor = cursor->next;                  // to current beat.
        }
        cursor->next = removal_target->next;
    
        result = select_next_beat(track); // Move current beat up.
    
        free_beat(removal_target); // Free the current beat.
    
    }    
    return result;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add note to beat, given its 'musical notation'. (m)
int add_musical_note_to_beat(Beat beat, char *musical_notation) {
    int result = VALID_NOTE;
    int i = 0; // Position within musical_note format.
    
    int character = musical_notation[i]; // Getting octave.
    int octave = (character - '0');
    
    if ((character < '0') || (character > '9')) {
        result = INVALID_MUSICAL_NOTE;  // Making sure octave is valid.
        return result;
    }
    
    i++;
    character = musical_notation[i]; // Getting key.
    
    if ((character < 'A') || (character > 'G')) {
        result = INVALID_MUSICAL_NOTE; // Making sure key is valid.
        return result;
    }
    
    int shift = 0; // Shifting key values to match actual values.
    if (character > 'A') {    
        shift++;
    } 
    if (character > 'C') {
        shift++;
    } 
    if (character > 'D') {
        shift++;
    } 
    if (character > 'F') {
        shift++;
    }
    
    int key = (character - 'A' + shift);  // Obtaining shifted value for key.
    
    i++;
    character = musical_notation[i];   // Adding sharps (#).
    while (character != 0) {   
        if (character == '#') {
            key++;
        
            if (key > 11) {     // Move to new octave if key > 11.
                key = (key - 12);
                octave++;
            }
        
            if (octave > 9) {                  // Disqualify if sharps push 
                result = INVALID_MUSICAL_NOTE; //key out of valid range.
                return result;
            }
        
        } else {
            result = INVALID_MUSICAL_NOTE; // Disqualify non-hashtags.
            return result;
        }
        
        i++;
        character = musical_notation[i];
    }
    struct note *new_note = malloc(sizeof(struct note)); // Create note.
    new_note->octave = octave;
    new_note->key = key;
    
    // Inserting note in correct place + checking for duplicates.
    struct note *cursor = beat->notes;
    
    if (cursor == NULL) {  // If there are no notes in beat.
        new_note->next = NULL;
        beat->notes = new_note;
        return result;
    }
    
    int high_or_low = higher_or_lower(*new_note, *cursor); // For 2nd note.
    if (high_or_low == HIGHER && cursor->next == NULL) {
        cursor->next = new_note;
        new_note->next = NULL;
        return result;
    } else if (high_or_low == LOWER) {
        beat->notes = new_note;
        new_note->next = cursor;
        return result;
    } else if (high_or_low == INVALID_MUSICAL_NOTE) {
        result = INVALID_MUSICAL_NOTE;
        free(new_note);
        return result;
    }
    
    struct note *check_ahead = cursor->next; // For notes after second
    high_or_low = higher_or_lower(*new_note, *check_ahead);
    
    while ((check_ahead != NULL) && (high_or_low == HIGHER)) {
        cursor = cursor->next;
        check_ahead = check_ahead->next;
        if (check_ahead != NULL) {
            high_or_low = higher_or_lower(*new_note, *check_ahead);
        }
    }
    
    if (high_or_low == INVALID_MUSICAL_NOTE) {
        free(new_note);
        result = INVALID_MUSICAL_NOTE;
    } else {
        cursor->next = new_note;
        new_note->next = check_ahead;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    struct beat *cursor = track->curr;
    struct beat *cut_beat = track->curr;
    struct beat *save_curr = track->curr;
    struct beat *link = track->head;
    int count = 0;
    int count2 = 0;
    int beats_cut = 0;
    int shorten_range = 0;
    
    if (range_length <= 0) { // Disqualify if range_length is zero or negative
        return;
    }
    
    while (cursor != NULL) {// Check if there are less beats than range_length
        cursor = cursor->next;
        shorten_range++;
    }
    if (shorten_range < range_length) {
        range_length = shorten_range;
    }
        
    cursor = track->head;
    while (cursor != track->curr) {// Identify what number beat is track->curr
        cursor = cursor->next;
        count++;
    }
    
    while (beats_cut != range_length) {  // Cutting beats individually.
        cut_beat = track->curr;
        cursor = track->head;
        count2 = 0;
        if (track->curr == track->head) { // Link beats adjacent to cut_beat.
            track->head = cursor->next;   // If track->curr is track->head.
            track->curr = track->head;
        } else {
            while (count2 != count - 1) { // If track->curr isn't track->head.
                cursor = cursor->next;
                count2++;
            }
            link = cursor;
            cursor = cursor->next;
            link->next = cursor->next;
            track->curr = link->next;
        }
        while (cursor->next != NULL) { // Inserting beat at the end of track.
            cursor = cursor->next;
        }
        cursor->next = cut_beat;
        cut_beat->next = NULL;

        beats_cut++;
    }
    track->curr = save_curr; // Re-align current beat to proper place
    return;
}


// Reverse a list of beats within a range of a track. (V)
int reverse_range(Track track, int range_length) {
    struct beat *cursor = track->curr;
    struct beat *cut_beat = track->curr;
    struct beat *save_curr = track->curr;
    struct beat *link = track->head;
    int count = 0;
    int beats_reversed = 0;
    int shorten_range = 0;
    
    if (range_length < 0) { // Disqualify if range_length is zero or negative
        return 0;
    }
    
    while (cursor != NULL) {// Check if there are less beats than range_length
        cursor = cursor->next;
        shorten_range++;
    }
    if (shorten_range < range_length) {
        range_length = shorten_range;
    }
        
    cursor = track->head;
    while (cursor != track->curr) {// Identify what number beat is track->curr
        cursor = cursor->next;
        count++;
    }
    
    int operation_count = 0; // Tracks how many times this loop runs.
    while (beats_reversed != range_length - 1) {  // Cutting beats 
        cut_beat = track->curr;                   // individually.
        cursor = track->head;
        int count2 = 0;
        
        if (track->curr == track->head) { // Link beats adjacent to cut_beat.
            track->head = cursor->next;   // If track->curr is track->head.
            track->curr = track->head;
        } else {
            while (count2 != count - 1) { // If track->curr isn't track->head.
                cursor = cursor->next;
                count2++;
            }
            link = cursor;
            cursor = cursor->next;
            link->next = cursor->next;
            track->curr = link->next;
        }
        
        count2 = 0;
        while (count2 != (range_length - 1)) { // Inserting beat at the end 
            cursor = cursor->next;            // of range.
            count2++;
        }
        cut_beat->next = cursor->next;
        cursor->next = cut_beat;
        
        range_length--;
        operation_count++;
    }
    
    
    track->curr = save_curr; // Realign current beat to proper beat.
    beats_reversed = operation_count;
    return beats_reversed;
}

////////////////////////////////////////////////////////////////////////
//                         Extra Function Code                        //
////////////////////////////////////////////////////////////////////////

// Checks if note is valid
int check_valid_note(int octave, int key, int high_octave, int high_key) {
    int result = VALID_NOTE; // 0 = valid note
    
    if (key >= 12) {
        result = INVALID_KEY; // -1 = invalid key
    } else if (key < 0) {
        result = INVALID_KEY;
    }
    
    if (octave >= 10) {
        result = INVALID_OCTAVE; // -2 = invalid octave
    } else if (octave < 0) {
        result = INVALID_OCTAVE;
    }
    
    if (octave < high_octave) {
        result = NOT_HIGHEST_NOTE; // -3 = not highest note
    } else if (octave == high_octave && key <= high_key) {
        result = NOT_HIGHEST_NOTE;
    }    
    return result;
}

//Checks a tracks status
int status_checker(Track track) {
    struct beat *cursor = track->curr;
    
    if (cursor == NULL) {
        return TRACK_STOPPED;
    } else if (cursor != NULL) {
        return TRACK_PLAYING;
    }

    return 0;    
}

// Checks whether a note is higher or lower than another
int higher_or_lower(struct note note_a, struct note note_b) {
    int result = 0;
    if (note_a.octave == note_b.octave) {
        if (note_a.key == note_b.key) {
            result = INVALID_MUSICAL_NOTE;
        } else if (note_b.key > note_a.key) {
            result = LOWER;
        } else if (note_b.key < note_a.key) {
            result = HIGHER;
        }
    
    } else if (note_b.octave > note_a.octave) {
        result = LOWER;
    } else if (note_b.octave < note_a.octave) {
        result = HIGHER;
    }
    return result;
}
