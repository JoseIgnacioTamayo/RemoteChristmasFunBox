
// To be called at setup()
void setupMusic();

// To be called at inside the loop()
void playMusic();

//
void setFastMode(bool enable);

//
void resetDancer();

//
void musicReset();

// External variables
extern bool enableDancing;
extern bool enableMusic;
extern bool enableFast;
extern   byte* currentSong  ;
extern   byte currentSongT  ;

extern const byte numberOfSongs;
extern const byte* songs[]  ;
extern const byte songsT[] ;
