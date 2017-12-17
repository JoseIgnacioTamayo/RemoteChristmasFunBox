

#define SECS_SILENCE_MS       3000        // Time to wait after a song is finished

#define TONE_LENGTH_MS         1800
#define FAST_TONE_LENGTH_MS     800

#define SILENCE_AFTER_TONE_MS  50           // silence after each tone [ms]

#define MAX_TWIST_DEGREES      60           // Max Degrees to turn at each dance twist


/* ---------- Public Variables -----------*/
extern bool enableDancing;
extern bool enableMusic;
extern bool enableBlink;
extern bool enableFast;
extern   char* currentSong  ;

extern const byte numberOfSongs;
extern const char* songs[]  ;


// To be called at setup()
void setupMusic();

// To be called at inside the loop()
void musicStateMachine();

// Sets the Servo to central position
void resetDancer();

// Resets the State Machine, stops the music.
void musicReset();


void offDancer();


