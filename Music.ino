
/*

    This file handles the playing of the song, the dancing Servo, and the 3 LEDs

*/



#define SECS_SILENCE_MS       3000        // Time to wait after a song is finished

#define FAST_MULTIPLIER     0.6           // In Fast mode, the tones lengths are affected by this multiplier.

#define SILENCE_AFTER_TONE_MS  50           // silence after each tone [ms]

#define MAX_TWIST_DEGREES      60           // Max Degrees to turn at each dance twist
#define MIN_TWIST_DEGREES      10            // Min Degrees to turn at each dance twist

#include "songs.h"


/* ---------- Public Variables -----------*/
bool enableDancing;
bool enableMusic ;
bool enableBlink;
bool enableFast;



/* ----- local Variables  ------ */
float maxToneTime_Ms = TONE_LENGTH_MS;     //
byte leds[N_LEDS] = {LED1_PIN, LED2_PIN, LED3_PIN};
byte currentLed = 0;
char* currentSong = (char*) songs[0];
int  defaultToneLen = 4;
int  defaultScale = 5;
int songIndex = 0;
Servo servoMain;


/* ======================= Local Functions ============================ */

/* lenghtToServoTwist()

   Convert a note length to a proportinal degree value, for dancing twist

   Input len represents the fraction of a complete tone, as 8 means an 8th of tone length.

   If the calculated twist is less than 10 degrees, output is 0.
*/
int lenghtToServoTwist(int len) {
  float twist = float(MAX_TWIST_DEGREES)  / len ;
  return int((twist >= MIN_TWIST_DEGREES) ? twist : 0);
}

/* offLED()

   Turn off the last led that was turned on
*/
void offLED() {
  digitalWrite(leds[currentLed], LOW);
}

/* onNextLED()

   Turn on the next led on the small led array.

   It cycles around the LEDs.
*/
void onNextLED() {
  currentLed = (currentLed == (N_LEDS - 1) ) ? 0 : (currentLed + 1);
  digitalWrite(leds[currentLed], HIGH);
}

/* initRTTTL()

   Reads the header part of a song in RTTTL format

   p is the pointer to the begining of the RTTTL string.

   It returns the position on the RTTTL string where the song notes start, to be used with readRTTLNote()

   Taken from https://github.com/flickerfly/Music_Box/blob/master/play_rtttl.ino

*/
char* initRTTTL(char* p) {

  int num;
  // format: Title:d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while (*p != ':') p++;   // ignore name
  p++;                     // skip ':'

  // get default duration
  if (*p == 'd')
  {
    p += 2;            // skip "d="
    num = 0;
    while (isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if (num > 0) defaultToneLen = num;
    p++;                   // skip comma
  }

  // get default octave
  if (*p == 'o')
  {
    p += 2;            // skip "o="
    num = *p++ - '0';
    if (num >= 3 && num <= 7) defaultScale = num;
    p++;                   // skip comma
  }

  // get BPM
  if (*p == 'b')
  {
    p += 2;            // skip "b="
    num = 0;
    while (isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    maxToneTime_Ms = (60 * 1000L / num) * 4;  // this is the time for whole note (in milliseconds)
    p++;                   // skip colon
  }

  return p;

}

/* readRTTLNote()

   Reads a note of a song in RTTTL format

   p is the pointer to the note to read int the RTTTL string.

   It returns the position on the RTTTL string where the next notes start, to be used with readRTTLNote()

   Taken from https://github.com/flickerfly/Music_Box/blob/master/play_rtttl.ino

*/
char* readRTTLNote(char* p, int* note, int* len_ms ) {

  // first, get note duration, if available
  int num = 0;
  byte scale = 0;
  while (isdigit(*p))
  {
    num = (num * 10) + (*p++ - '0');
  }

  if (num) *len_ms =  maxToneTime_Ms / num;
  else *len_ms =  maxToneTime_Ms / defaultToneLen; // we will need to check if we are a dotted note after

  // now get the note
  switch (*p)
  {
    case 'c':
      num = 1;
      break;
    case 'd':
      num = 3;
      break;
    case 'e':
      num = 5;
      break;
    case 'f':
      num = 6;
      break;
    case 'g':
      num = 8;
      break;
    case 'a':
      num = 10;
      break;
    case 'b':
      num = 12;
      break;
    case 'p':
    default:
      num = 0;
  }
  p++;

  // now, get optional '#' sharp
  if (*p == '#')
  {
    num++;    p++;
  }



  // now, get optional '.' dotted note, which add 50% to the tone duration
  if (*p == '.')
  {
    *len_ms += *len_ms / 2;
    p++;
  }


  // now, get scale
  if (isdigit(*p))
  {
    scale = *p - '0';
    p++;
  }
  else
  {
    scale = defaultScale;
  }

  if (num) *note = notes[(scale - 4) * 12 + num];
  else *note = 0;

  if (*p == ',')
    p++;       // skip comma for next note (or we may be at the end)

  return p;

}


/*  musicReset()
 *   
 *   This restart the music playing state machine, turns off the LEDs, and sets the dancer to central position if it is enabled
*/
void musicReset() {
  startTime = millis();

  noTone(SPEAKER_PIN);

  musicState = 0;
  offLED();
  if (enableDancing )resetDancer();
}

/*  setupMusic()
 *   
 *   to be called at setup()
*/
void setupMusic() {
  pinMode(SPEAKER_PIN, OUTPUT); // Set buzzer
  for (int i = 0; i < N_LEDS; i++)  pinMode(leds[i], OUTPUT);
  musicReset();
  offDancer();
}


/*  resetDancer()
 *   
 *   It attaches the Servo to the control pin, and sets it to central position
 *   
 *   From then on, the servo is under control signal.
 *   
*/
void resetDancer() {
  servoMain.attach(SERVO_PIN); // Set servo
  servoMain.write(90);
}

/*  offDancer()
 *   
 *   Sets the servo to central position and detaches it. This leaves the servo without any control signal
*/
void offDancer() {
  servoMain.write(90);
  servoMain.detach();
}


/* ============================================= */


/* ----- State machine vars  ------ */
byte musicState = 0, musicPlayState;
unsigned long startTime;
int  toneLength_ms, servoPos;
bool leftRight, blinking;

/*  musicStateMachine()
 *   
 *   The state machine plays continouslly all the songs in the array songs[].
 *   It alternates between tones and waiting, until all notes are played.
 *   The Servo goes dancing left/right alternating, according to the lenght of the note.
 *   Each of the LEDs in the array lights up for the duration of the note
 *   
*/
void musicStateMachine() {

  switch (musicState) {
    case 0:                   // Waiting for Setup time
      if (hasTimePassed (startTime,  SECS_SILENCE_MS)) {

        // Load song
        if (songIndex >= (numberOfSongs ) ) songIndex = 0;
        currentSong = (char*) songs[songIndex];
        songIndex++;

        // Read the RTTL definitions and point to the first note.
        currentSong = initRTTTL(currentSong);

        // Accelerate tempo on Fast Mode
        if (enableFast) maxToneTime_Ms = (float)(maxToneTime_Ms) * 0.8;

        musicState = 1;
        musicPlayState = 0;

      }

      break;
      
    case 1:           // Playing

      switch (musicPlayState) {
        case 0:       // Setup the tone to play
        
          int  note, toneLength;

          currentSong = readRTTLNote(currentSong, &note, &toneLength_ms);

          toneLength  = maxToneTime_Ms / toneLength_ms ;     // Get the fraction of the full note that corresponds
  
          // Play tone
          if (enableMusic) {
              if (note) tone(SPEAKER_PIN, note,  (toneLength_ms - SILENCE_AFTER_TONE_MS));
          }

          if (enableDancing) {

            //Turn the servo
            servoPos += lenghtToServoTwist(toneLength) * (leftRight ? -1 : 1);

            // The Servo cannot go out of [20;160] degrees. If it tries, just invert the move
            servoPos = (servoPos >= 160) ? servoPos - (lenghtToServoTwist(toneLength)  * 2) : servoPos;
            servoPos = (servoPos <= 20) ?  servoPos + (lenghtToServoTwist(toneLength)  * 2) : servoPos;
            servoPos = (servoPos >= 0 ) ? ( (servoPos <= 180) ? servoPos : 180) : 0;

            servoMain.write(  servoPos );
            leftRight = !leftRight;
          }

          if (enableBlink && (! enableDancing) && (lcdIsIdle())) {
            lcdBackLight(blinking);
            blinking = !blinking;
          }

          offLED(); onNextLED();

          startTime = millis();
          musicPlayState = 1;

          break;
        case 1:  // Wait Tone length

          if (hasTimePassed (startTime, toneLength_ms)) {

            // Still notes to play, keep playing
            if (*currentSong) {
              musicPlayState = 0;

            } else {
              
              // No more tones in song
              startTime = millis();
              noTone(SPEAKER_PIN);
              musicState = 0;
              offLED();
            }
          }

          break;
      }

      break;
      default:
      musicState = 0;
      break;
  }
}






/*

  Code released under MIT license

  Copyrigh 2017 Jose Ignacio Tamayo Segarra (jose.ignacio.tamayo@gmail.com) and Victor Boglea (bvictor@viking-tech.ro)

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

