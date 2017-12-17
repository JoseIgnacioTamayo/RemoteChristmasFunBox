
/*

    This file has the songs.

    The Structure is an array of 'Note Number , NoteLength'.

*/


#include "tones.h"


// Note 1 is C3 at 440 Hz
// https://pages.mtu.edu/~suits/notefreqs.html

//const int jingleBells[] = {  // Jingle Bells Data   https://create.arduino.cc/projecthub/ianabcumming/musical-fairy-lights-ed1445?ref=tag&ref_id=christmas&offset=1
//  NOTE_F5, 2,   NOTE_F5, 2,   NOTE_F5, 4,  NOTE_F5, 2, NOTE_F5, 2, NOTE_F5, 4, 
// NOTE_F5, 2, NOTE_GS5, 2, NOTE_CS5, 3, NOTE_DS5, 1, NOTE_F5, 8, NOTE_FS5, 2, NOTE_FS5, 2,
//  NOTE_FS5, 3,  NOTE_FS5, 1,  NOTE_FS5, 2, NOTE_F5, 2, NOTE_F5, 2, NOTE_F5, 1, NOTE_F5, 1, NOTE_F5, 2,  NOTE_DS5, 2, NOTE_DS5, 2, NOTE_F5, 2, NOTE_DS5, 4, NOTE_GS5, 4,
//  NOTE_F5, 2,   NOTE_F5, 2,   NOTE_F5, 4,  NOTE_F5, 2, NOTE_F5, 2, NOTE_F5, 4, NOTE_F5, 2, NOTE_GS5, 2, NOTE_CS5, 3, NOTE_DS5, 1, NOTE_F5, 8, NOTE_FS5, 2, NOTE_FS5, 2,
//  NOTE_FS5, 2,  NOTE_FS5, 2,  NOTE_FS5, 2, NOTE_F5, 2, NOTE_F5, 2, NOTE_F5, 1, NOTE_F5, 1, NOTE_GS5, 2, NOTE_GS5, 2, NOTE_FS5, 2, NOTE_DS5, 2, NOTE_CS5, 8
//} ;
//
//const int weWishYou[] = {  // We wish you a Merry Christmas https://www.8notes.com/scores/11826.asp
//NOTE_D5, 4,   NOTE_G5, 4,   
//NOTE_G5, 2,  NOTE_A5, 2, NOTE_G5, 2, NOTE_FS5, 2, 
//NOTE_E5, 4, NOTE_C5, 4,NOTE_E5, 4, NOTE_A5, 4, 
//NOTE_A5, 2, NOTE_B5, 2, NOTE_A5, 2,  NOTE_G5, 2, 
//NOTE_FS5,2 , NOTE_D5, 4, NOTE_FS5, 4, NOTE_B5, 4, 
//NOTE_B5, 2, NOTE_C6, 2, NOTE_B5, 2,  NOTE_A5, 2, 
//NOTE_G5, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_E5, 4,
//NOTE_A5, 4,   NOTE_FS5, 4,   NOTE_G5, 8
//} ;


const char jingleBells[] = ":d=4,o=5,b=170:b,b,b,p,b,b,b,p,b,d6,g.,8a,2b.,8p,c6,c6,c6,c6,c6,b,b,8b,8b,b,a,a,b,2a,2d6";

const char weWishYou[] = ":d=8,o=5,b=140:4d,4g,g,a,g,f#,4e,4c,4e,4a,a,b,a,g,4f#,4d,4f#,4b,b,c6,b,a,4g,4e,4d,4e,4a,4f#,2g";

const char on12days[] = ":d=8,o=5,b=150:d,d,4g,g,g,4g,g,g,a,b,c6,a,4b.,p,4d6,a,b,c6,a,d6,d6,a,b,c6,a,4d6,4e6,4d.6,p,d6,c6,b,a,4g,a,b,4c6,4e,4e,4d,g,a,b,c6,4b,4a,2g.";

//const char star[] = ":d=8,o=5,b=160:16c.,16p,f,p,f,p,4f,4g,a,g,a,a#,c.6,16p,4a,4a#,a,16f.,g.,16p.,g,p,2f.,16c.,16p,f,p,f,p,4f,4g,a,g,a,a#,c.6,16p,4a,4a#,a,16f.,g.,16p,32p,g,p,2f.";

const char santa[] = ":d=4,o=5,b=200:g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,f,f,e,g,c,e,d,2f,b4,1c,p,c6,d6,c6,b,c6,a,2a,c6,d6,c6,b,c6,2a.,d6,e6,d6,c#6,d6,b,b,b,8b,8c6,d6,c6,b,a,g,p,g.,8g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,8f,2f,e,g,c,e,d,2f,d6,1c6.";

const char* songs[] = {jingleBells, weWishYou,on12days,santa};


const byte numberOfSongs = 4;

