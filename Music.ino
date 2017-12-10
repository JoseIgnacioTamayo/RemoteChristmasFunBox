
/*

    This file handles the playing of the song, the dancing Servo, and the witness LED

*/


#include "songs.h"


/* ---------- Public Variables -----------*/
bool enableDancing;
bool enableMusic = true;
bool enableFast;
byte* currentSong = (byte*) songs[0];
byte currentSongT = (byte) songsT[0];

/* ----- local Variables  ------ */
float toneTime_Ms = TONE_LENGTH_MS;     // a note of length MAX_NOTE_LENGTH will play for toneTime_Ms time. Float to make divisions

/* --------- Local Functions ---------- */

/*noteToHz

   Convert a Note Nr. to Frequency
   https://pages.mtu.edu/~suits/notefreqs.html

   Input note is given in the interval [1,255], as read from the song text
*/
int noteToHz(int note) {                                 //
  float freq = 440 * (pow(1.059463094359, (float) note - 21));    // -21 gives you note 1 at C3
  return int(freq);                                       // Results are accurate to 1hz
}

/*lenghtToServoTwist

   Convert a note length to a proportinal degree value, for dancing twist

   Input len is given in the interval [1,MAX_NOTE_LENGTH], as read from the song text

   If the calculated twist is less than 10 degrees, output is 0.
*/
int lenghtToServoTwist(int len) {
  float twist = MAX_TWIST_DEGREES * (float(len ) / MAX_NOTE_LENGTH);
  return int((twist >= 10) ? twist : 0);
}

/* ----- State machine vars  ------ */
byte musicState = 0, musicPlayState;
unsigned long startTime;
int t, note, toneLength, toneLength_ms, servoPos;
bool leftRight, blinking;

void musicReset() {
  startTime = millis();
  servoMain.write(90);
  noTone(SPEAKER_PIN);
  digitalWrite(LED_PIN, LOW);
  musicState = 0;

}


void setupMusic() {
  pinMode(SPEAKER_PIN, OUTPUT); // Set buzzer
  pinMode(LED_PIN, OUTPUT); // Set buzzer
  servoMain.attach(SERVO_PIN); // Set servo
  musicReset();

}


void musicStateMachine() {

  switch (musicState) {
    case 0: // Waiting for Setup time
      if (hasTimePassed (startTime,  SECS_SILENCE_MS)) {
        musicState = 1;
        musicPlayState = 0;
        t = 0;
        Serial.println("Music pause");
      }

      break;
    case 1: // Playing

      switch (musicPlayState) {
        case 0:


          note = noteToHz(currentSong[t * 2]);
          toneLength = currentSong[(t * 2) + 1] ;
          if ((toneLength < 1) || (toneLength > MAX_NOTE_LENGTH))  toneLength = MAX_NOTE_LENGTH;
          toneLength_ms = (int)((toneLength * toneTime_Ms) / MAX_NOTE_LENGTH) ;

          if (enableMusic) {
            // Play tone
            tone(SPEAKER_PIN, note,  (toneLength_ms - SILENCE_AFTER_TONE_MS));

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

          startTime = millis();
          musicPlayState = 1;
          digitalWrite(LED_PIN, (blinking) ? HIGH : LOW);  blinking = !blinking;
          Serial.print("t");

          break;
        case 1:  // Wait Tone length

          if (hasTimePassed (startTime, toneLength_ms)) {

            // Still notes to play, keep playing
            if (t < (currentSongT - 1)) {
              musicPlayState = 0;
              t++;
            } else {

              // No more tones in song
              startTime = millis();
              servoMain.write(90);
              noTone(SPEAKER_PIN);
              musicState = 0;
              Serial.println("Silence after song");
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

void setFastMode(bool enableAction) {
  toneTime_Ms = enableAction ? FAST_TONE_LENGTH_MS : TONE_LENGTH_MS ;
}

void resetDancer() {
  servoMain.write(90);

}

