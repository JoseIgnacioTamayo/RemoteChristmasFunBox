
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

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_DATA3_PIN, LCD_DATA2_PIN, LCD_DATA1_PIN, LCD_DATA0_PIN);

/* ----- State machine vars  ------ */
byte lcdState = 0, typingState;

void lcdReset() {
 
  LCDstartTime = millis();
  printTitle();
  lcdBackLight(1);

  msgIndex = 0;
  lcdState = 8;
  typingState = 0;
  
}


void lcdBackLight(byte onOff){
    digitalWrite(LCD_BACKLIGHT_PIN, (onOff)?HIGH:LOW);
  }


void setupLCD() {
  lcd.begin(COLS, ROWS)  ;
  pinMode(LCD_BACKLIGHT_PIN,OUTPUT);
  lcdBackLight(0);
  lcd.clear();
  lcdReset();
}


void lcdOff() {
  lcd.clear();
  lcdBackLight(0);

  msgIndex = 0;
  lcdState = 9;
  typingState = 0;
  
}
void printCentered(char* msg, byte len){

  char buf[ (COLS) ];
  memset(buf,  SPACE  , (COLS) );

  byte margin = (COLS - len) >> 1;
  byte i, j;

  // If margin is negative, center the Text
  if (margin < 0){
    msg-=margin;
    margin = 0;
    } 

  for ( i = margin,j = 0 ; j < len && i < (COLS); i++, j++) {
    (buf[i]) = (*msg++);
  }

  lcd.print(buf);
}


void printTitle()
{
  lcd.noAutoscroll();
  lcd.clear();
  lcd.setCursor(0, 0);
  printCentered(LCD_TITLE, sizeof(LCD_TITLE));
  lcd.setCursor(0, 1);
  printCentered(LCD_MSG, sizeof(LCD_MSG));
 
}

void startMesage(char* msg){
  int i;
   memset(lcdMessage, SPACE, MAX_MSG_LEN);
    
    for (i = 0; (i < (MAX_MSG_LEN) && (*msg >= ' ')); i++ ) lcdMessage[i] = *msg++;
    lcdMessageLen = i;
    
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.noAutoscroll();
  lcdState = 1;
  typingState = 0;
   lcdBackLight(1);
  }


void lcdStateMachine() {
  switch (lcdState) {
    case 0: // Waiting for pause time
      if (hasTimePassed (LCDstartTime,  MSG_RETAIN_TIME_MS)) {

        printTitle();
        lcdBackLight(1);

        msgIndex = 0;
        lcdState = 8;
        LCDstartTime = millis();
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
              lcdState = 0;    // Retain
              LCDstartTime = millis();
            }
          }
          break;
      }

      break;
      case 8: // Waiting for pause time
      if (hasTimePassed (LCDstartTime,  TITLE_RETAIN_TIME_MS)) {

        lcdBackLight(0);

        LCDstartTime = millis();
        lcdState = 9;
        
      }

      break;

  }
}

bool lcdIsIdle(){
  return (lcdState == 9);
  }

