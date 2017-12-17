
/*

    This file handles the playing of the song, the dancing Servo, and the witness LED

*/


#include "songs.h"


/* ---------- Public Variables -----------*/
bool enableDancing;
bool enableMusic ;
bool enableBlink;
bool enableFast;

char* currentSong = (char*) songs[0];

/* ----- local Variables  ------ */
float maxToneTime_Ms = TONE_LENGTH_MS;     // 
byte leds[N_LEDS] = {LED1_PIN, LED2_PIN, LED3_PIN};
byte currentLed = 0;

int  defaultToneLen = 4;
int  defaultScale = 5;
int songIndex = 0;

/* ======================= Local Functions ============================ */

/*lenghtToServoTwist

   Convert a note length to a proportinal degree value, for dancing twist

   Input len represents the fraction of a complete tone, the fraction of a dancing twist

   If the calculated twist is less than 10 degrees, output is 0.
*/
int lenghtToServoTwist(int len) {
  float twist = float(MAX_TWIST_DEGREES)  / len ;
  return int((twist >= 10) ? twist : 0);
}

void offLED() {
  digitalWrite(leds[currentLed], LOW);
}

void onNextLED() {
  currentLed = (currentLed == (N_LEDS - 1) ) ? 0 : (currentLed + 1);
  digitalWrite(leds[currentLed], HIGH);
}


char* initRTTTL(char* p) {

  int num;
  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while (*p != ':') p++;   // ignore name
  p++;                     // skip ':'

  // get default duration
  if (*p == 'd')
  {
    p++; p++;              // skip "d="
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
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if (num >= 3 && num <= 7) defaultScale = num;
    p++;                   // skip comma
  }

  // get BPM
  if (*p == 'b')
  {
    p++; p++;              // skip "b="
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

char* readRTTLNote(char* p, int* note, int* len_ms ) {
  // first, get note duration, if available
  int num = 0;
  byte scale = 0;
  while (isdigit(*p))
  {
    num = (num * 10) + (*p++ - '0');
  }

  if (num) *len_ms =  maxToneTime_Ms/num;
  else *len_ms =  maxToneTime_Ms/defaultToneLen;  // we will need to check if we are a dotted note after

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
    num++;
    p++;
  }

 

  // now, get optional '.' dotted note
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


/* ============================================= */


/* ----- State machine vars  ------ */
byte musicState = 0, musicPlayState;
unsigned long startTime;
int  toneLength_ms, servoPos;
bool leftRight, blinking;

void musicReset() {
  startTime = millis();

  noTone(SPEAKER_PIN);

  musicState = 0;
  offLED();
  resetDancer();
}


void setupMusic() {
  pinMode(SPEAKER_PIN, OUTPUT); // Set buzzer
  for (int i = 0; i < N_LEDS; i++)  pinMode(leds[i], OUTPUT);
  musicReset();
  offDancer();
}


void musicStateMachine() {

  switch (musicState) {
    case 0: // Waiting for Setup time
      if (hasTimePassed (startTime,  SECS_SILENCE_MS)) {
      


        // Load current song
        if (songIndex >= (numberOfSongs ) ) songIndex = 0;
        currentSong = (char*) songs[songIndex];
        songIndex++;

        currentSong = initRTTTL(currentSong);
        if (enableFast) maxToneTime_Ms = (float)(maxToneTime_Ms)*0.8;
        
        musicState = 1;
        musicPlayState = 0;
        
      }

      break;
    case 1: // Playing

      switch (musicPlayState) {
        case 0:
          int  note, toneLength;

         currentSong = readRTTLNote(currentSong, &note, &toneLength_ms);

         toneLength  = maxToneTime_Ms / toneLength_ms ;

          if (enableMusic) {
            // Play tone
            if (note) tone(SPEAKER_PIN, note,  (toneLength_ms - SILENCE_AFTER_TONE_MS));

          }

          if (enableDancing) {



            //Turn the servo
            servoPos += lenghtToServoTwist(toneLength) * (leftRight ? -1 : 1);

            // The Servo cannot go out of [0;180] degrees. If it tries, just invert the move
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

          offLED();
          onNextLED();


          startTime = millis();
          musicPlayState = 1;

          //Serial.print("t");

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
              //Serial.println("Silence after song");
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


void resetDancer() {
  servoMain.attach(SERVO_PIN); // Set servo
  servoMain.write(90);
}


void offDancer() {
  servoMain.write(90);
  servoMain.detach();
}





