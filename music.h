

#define SECS_SILENCE_MS       3000        // Time to wait after a song is finished

#define TONE_LENGTH_MS         1400
#define FAST_TONE_LENGTH_MS    1000

#define SILENCE_AFTER_TONE_MS  50           // silence after each tone [ms]

#define MAX_TWIST_DEGREES      60           // Max Degrees to turn at each dance twist



/* ---------- Public Variables -----------*/
extern bool enableDancing;
extern bool enableMusic;
extern   byte* currentSong  ;
extern   byte currentSongT  ;

extern const byte numberOfSongs;
extern const byte* songs[]  ;
extern const byte songsT[] ;


// To be called at setup()
void setupMusic();

// To be called at inside the loop()
void musicStateMachine();

// Either sets the singing tempo to fast, or to normal
void setFastMode(bool enable);

// Sets the Servo to central position
void resetDancer();

// Resets the State Machine, stops the music.
void musicReset();


