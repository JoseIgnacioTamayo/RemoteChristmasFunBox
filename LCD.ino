
/*

    This file handles the writing of messages on the LCD Screen.

*/


/* ---------- Public Variables -----------*/
char lcdMessage[MAX_MSG_LEN] = "Merry Christmas Ho ho ho";   // Buffer where to have the LCD message to display
byte lcdMessageLen = 24;

/* -------- Local Variables ------------*/
unsigned long LCDstartTime;
byte msgIndex;    // To follow the typing of the message 
bool LCDblinking; // Alternating, the Title message is sourrounded by STARs

/* ----- State machine vars  ------ */
byte lcdState = 0, typingState;



void setupLCD() {
  lcd.begin(COLS, ROWS)  ;
  lcdReset();
}

void lcdReset() {
  lcd.clear();
  LCDstartTime = millis();
  lcdState = 0;
}


void printTitle(char* msg , int len)
{
  lcd.noAutoscroll();
  lcd.leftToRight();
  lcd.clear();
  lcd.setCursor(0, 0);

  char buf[(COLS) + 1];
  memset(buf, STAR, (COLS) + 1);

  // Calculate margin
  byte margin = (COLS - len) >> 1;
  byte i, j;
  margin = (margin < 0) ? 0 : margin;

  for ( i = 0, j = 0; j < margin && i < (COLS); i++, j++) {
    (buf[i]) = (LCDblinking) ? STAR : SPACE;
  }
  for ( j = 0 ; j < len && i < (COLS); i++, j++) {
    (buf[i]) = (*msg++);
  }
  for ( j = 0; j < margin && i < (COLS); i++, j++) {
    (buf[i]) = (LCDblinking) ? STAR : SPACE;
  }
  buf[i] = '\0';
  lcd.print(buf);
  Serial.println("LCD title");
  Serial.println(buf);
  LCDblinking = !LCDblinking;

}


void lcdStateMachine() {
  switch (lcdState) {
    case 0: // Waiting for pause time
      if (hasTimePassed (LCDstartTime,  TITLE_RETAIN_TIME_MS)) {

        printTitle(LCD_TITLE, sizeof(LCD_TITLE));

        // set the display to automatically scroll:
        lcd.setCursor(0, 1);

        msgIndex = 0;
        lcdState = 1;
        typingState = 0;
      }

      break;
    case 1: // Typing
      switch (typingState) {
        case 0: // Type a char
          lcd.write(lcdMessage[msgIndex++]);
          LCDstartTime = millis();
          typingState = 1;
          break;
        case 1:   // Waiting for pause time
          if (hasTimePassed (LCDstartTime,  TYPING_INTERVAL_MS)) {
            if (msgIndex ==  COLS ) {
              lcd.autoscroll();
              lcd.leftToRight();
            }
            if (msgIndex < lcdMessageLen) typingState = 0;    //Keep typing
            else {
              lcdState = 0;    // Retype
              LCDstartTime = millis();
            }
          }
          break;
      }

      break;

  }
}



