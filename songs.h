


#define SECS_SILENCE_MS  3000
#define ONE_SEC_MS  1000

#define TONE_LENGTH_MS         1400
#define FAST_TONE_LENGTH_MS    1000
#define SILENCE_AFTER_TONE_MS  50         // silence after each tone [ms]
#define MAX_TWIST_DEGREES  60           // Max Degrees to turn at each dance twist

#define MAX_NOTE_LENGTH  8


// Note 1 is C3 at 440 Hz
// https://pages.mtu.edu/~suits/notefreqs.html

// The Structure is Note Number , NoteLength


const byte jingleBells[] = {  // Jingle Bells Data   https://create.arduino.cc/projecthub/ianabcumming/musical-fairy-lights-ed1445?ref=tag&ref_id=christmas&offset=1
  29, 2, 29, 2, 29, 4, 29, 2, 29, 2, 29, 4, 29, 2, 32, 2, 25, 3, 27, 1, 29, 8, 30, 2, 30, 2,
  30, 3, 30, 1, 30, 2, 29, 2, 29, 2, 29, 1, 29, 1, 29, 2, 27, 2, 27, 2, 29, 2, 27, 4, 32, 4,
  29, 2, 29, 2, 29, 4, 29, 2, 29, 2, 29, 4, 29, 2, 32, 2, 25, 3, 27, 1, 29, 8, 30, 2, 30, 2,
  30, 2, 30, 2, 30, 2, 29, 2, 29, 2, 29, 1, 29, 1, 32, 2, 32, 2, 30, 2, 27, 2, 25, 8
} ;

const byte weWishYou[] = {  // We wish you a Merry Christmas https://www.8notes.com/scores/11826.asp
  27, 4,  30, 4, 30, 2, 31, 2, 30, 2, 29, 2, 28, 4, 28, 4, 28, 4, 31, 4, 31, 2, 32, 2, 31, 2, 30, 2, 29, 4, 29, 4, 29, 4, 32, 4,
  31, 2,  32, 2, 32, 2, 32, 2, 31, 4, 30, 4, 28, 2, 27, 2, 27, 2, 28, 4, 31, 4, 29, 4, 30, 8, 27, 4, 30, 4, 30, 4, 30, 4, 29, 8,
  29, 4,  30, 4, 29, 4, 28, 4, 27, 8, 31, 4, 32, 4, 31, 2, 31, 2, 30, 2, 30, 2, 34, 4, 27, 4, 27, 2, 27, 2, 28, 4, 31, 4, 29, 4, 30, 8
} ;


const byte* songs[] = {jingleBells, weWishYou};
const byte songsT[] = {51, 54};

const byte numberOfSongs = 2;

