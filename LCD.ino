



/* ---------- Public Variables -----------*/
// Buffer where to have the LCD message to display
char lcdMessage[MAX_MSG_LEN] = "Merry Christmas Ho ho ho";
byte lcdMessageLen = 24;

/* -------- Variables ------------*/
unsigned long LCDstartTime;
byte msgIndex;
bool LCDblinking;

void setupLCD() {
  lcd.begin(COLS, ROWS)  ;
  lcdReset();
}



/* ----- State machine vars  ------ */
byte lcdState = 0, typingState;


void lcdReset() {
  lcd.clear();
  LCDstartTime = millis();
  lcdState = 0;
}

void showLCD() {
  switch (lcdState) {
    case 0: // Waiting for pause time
      if (hasTimePassed (LCDstartTime,  TITLE_BLINK_TIME_MS)) {

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
          if (hasTimePassed (LCDstartTime,  TYPING_SPEED_MS)) {
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

