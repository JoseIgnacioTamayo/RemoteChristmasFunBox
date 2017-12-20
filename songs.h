
/*

    This file has the songs in RTTL format

    http://mines.lumpylumpy.com/Electronics/Computers/Software/Cpp/MFC/RingTones.RTTTL

*/


#include "tones.h"

const char jingleBells[] = ":d=4,o=5,b=170:b,b,b,p,b,b,b,p,b,d6,g.,8a,2b.,8p,c6,c6,c6,c6,c6,b,b,8b,8b,b,a,a,b,2a,2d6";

const char weWishYou[] = ":d=8,o=5,b=140:4d,4g,g,a,g,f#,4e,4c,4e,4a,a,b,a,g,4f#,4d,4f#,4b,b,c6,b,a,4g,4e,4d,4e,4a,4f#,2g";

const char on12days[] = ":d=8,o=5,b=150:d,d,4g,g,g,4g,g,g,a,b,c6,a,4b.,p,4d6,a,b,c6,a,d6,d6,a,b,c6,a,4d6,4e6,4d.6,p,d6,c6,b,a,4g,a,b,4c6,4e,4e,4d,g,a,b,c6,4b,4a,2g.";

//const char star[] = ":d=8,o=5,b=160:16c.,16p,f,p,f,p,4f,4g,a,g,a,a#,c.6,16p,4a,4a#,a,16f.,g.,16p.,g,p,2f.,16c.,16p,f,p,f,p,4f,4g,a,g,a,a#,c.6,16p,4a,4a#,a,16f.,g.,16p,32p,g,p,2f.";

const char santa[] = ":d=4,o=5,b=200:g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,f,f,e,g,c,e,d,2f,b4,1c,p,c6,d6,c6,b,c6,a,2a,c6,d6,c6,b,c6,2a.,d6,e6,d6,c#6,d6,b,b,b,8b,8c6,d6,c6,b,a,g,p,g.,8g,8e,8f,g,g.,8g,8a,8b,c6,2c6,8e,8f,g,g,g,8a,8g,8f,2f,e,g,c,e,d,2f,d6,1c6.";

const char* songs[] = {jingleBells, weWishYou,on12days,santa};

const byte numberOfSongs = 4;

