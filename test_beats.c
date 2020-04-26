// Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by Kellen Liew (z5312656)
// on 26/04/20
//
// Version 1.0.0: Assignment released.
#include <stdio.h>
#include <stdlib.h>

#include "test_beats.h"
#include "beats.h"

struct track {
    struct beat *curr;
    struct beat *head;
};

struct beat {
    struct note *notes;
    struct beat *next;
};

// Test function for `add_note_to_beat`
int test_add_note_to_beat(void) {
    // Test 1: Rejecting negative inputs.
    Beat test_beat = create_beat();
    if (add_note_to_beat(test_beat, -1, -1) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, -1, 0) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, -1) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Rejecting inputs outside of valid range.
    if (add_note_to_beat(test_beat, 10, 2) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, 12) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 999, 999) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Reject notes lower than previous notes.
    add_note_to_beat(test_beat, 4, 4);
    if (add_note_to_beat(test_beat, 3, 3) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 4, 3) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 4, 4) != NOT_HIGHEST_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 4, 5) != VALID_NOTE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    return MEETS_SPEC;
}

// Test function for `count_notes_in_octave`
int test_count_notes_in_octave(void){
    // Test 1: Not counting invalid octaves.
    Beat test_beat = create_beat();
    add_note_to_beat(test_beat, 1, 1);
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 11) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Not counting keys.
    add_note_to_beat(test_beat, 2, 3);
    if (count_notes_in_octave(test_beat, 2) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 3) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Not counting notes that were not added.
    add_note_to_beat(test_beat, 4, 5);
    add_note_to_beat(test_beat, 4, 5);
    add_note_to_beat(test_beat, 4, 5);
    add_note_to_beat(test_beat, 3, 4);
    if (count_notes_in_octave(test_beat, 4) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 3) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    return MEETS_SPEC;
}

// Test function for `add_beat_to_track`
int test_add_beat_to_track(void){
    // Test 1: If track->curr is NULL, notes are placed at track->head.
    Track test_track = create_track();
    Beat beat_a = create_beat();
    add_note_to_beat(beat_a, 1, 1);
    add_beat_to_track(test_track, beat_a);
    
    Beat beat_b = create_beat(); // beat_b is created.
    add_note_to_beat(beat_b, 2, 1);
    add_beat_to_track(test_track, beat_b); // b is track->head and links to 
                                           //beat_a.
    select_next_beat(test_track);
    Beat current = create_beat();
    current = test_track->curr;
    if (count_notes_in_octave(current, 2) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(current, 1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    

    // Test 2: Added notes do not replace current note.
    // Uses beats 'a' and 'b' from last test (two beats total).
    int status = select_next_beat(test_track); // Check that there are two
    if (status != TRACK_PLAYING) {             // beats and not just one.
        return DOES_NOT_MEET_SPEC; // First beat is currently selected.
    }
    
    status = select_next_beat(test_track); // If track is not playing
    if (status != TRACK_PLAYING) {         // beat_b replaced beat_a.
        return DOES_NOT_MEET_SPEC;
    }
    
    status = select_next_beat(test_track); // Making sure track stopped
    if (status != TRACK_STOPPED) {         // at two beats.
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Adding a beat does not change current selected beat.
    status = select_next_beat(test_track);
    status = select_next_beat(test_track); // track->curr should be beat_b.
    Beat beat_c = create_beat();
    
    if (current == beat_b) { // Make definite sure it is b.
        add_beat_to_track(test_track, beat_c);
        if (count_notes_in_octave(current, 2) != 1) {
            return DOES_NOT_MEET_SPEC;
        }
    } else {
        return DOES_NOT_MEET_SPEC;
    }
    
    Beat beat_d = create_beat();
    add_beat_to_track(test_track, beat_d);
    if (count_notes_in_octave(current, 2) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`.
int test_remove_selected_beat(void){
    
    Track test_track = create_track();
    Beat beat_a = create_beat(); // First Beat.
    add_note_to_beat(beat_a, 3, 1);
    add_beat_to_track(test_track, beat_a);
    
    Beat beat_b = create_beat(); // Second Beat.
    add_note_to_beat(beat_b, 2, 1);
    add_beat_to_track(test_track, beat_b);
    
    Beat beat_c = create_beat(); // Third Beat.
    add_note_to_beat(beat_c, 1, 1);
    add_beat_to_track(test_track, beat_b);
    
    select_next_beat(test_track);
    Beat current = create_beat();
    current = test_track->curr;
    // Test 1: Removes the right beat.
    remove_selected_beat(test_track);
    current = test_track->curr;
    if (count_notes_in_octave(current, 1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Track->curr moves to correct beat.
    if (count_notes_in_octave(current, 2) != 1) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(current, 3) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: There are a correct amount of beats in the track.
    current = test_track->head;
    int count = 0;
    
    while (current != NULL) {
        current = current->next;
        count++;
    }
    
    if (count != 2) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void){
    Beat test_beat = create_beat();
    
    char *string;
    string = "1B";
    add_musical_note_to_beat(test_beat, string);
    
    string = "1A";
    add_musical_note_to_beat(test_beat, string);
    
    string = "20A";
    add_musical_note_to_beat(test_beat, string);
    
    string = "3Y";
    add_musical_note_to_beat(test_beat, string);

    // Test 1: Only add valid notes
    if (count_notes_in_octave(test_beat, 20) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: Don't add musically invalid notes
    if (count_notes_in_octave(test_beat, 3) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: Add notes out of order
    if (count_notes_in_octave(test_beat, 1) == 1) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 1) != 2) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}
