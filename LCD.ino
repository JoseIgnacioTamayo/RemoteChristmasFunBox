/*

    This file handles the writing of messages on the LCD Screen.

*/

// LCD size
#define COLS  16
#define ROWS  2

#define MSG_RETAIN_TIME_MS     4000   // Time before deleting the message

#define TITLE_RETAIN_TIME_MS     4000   // Time to retain the Title on screen
#define TYPING_INTERVAL_MS       500    // Time between LCD typing letters for the message

#define MAX_MSG_LEN 32     // Max length of the message to show. It will autoscroll on the screen

#define SPACE     ' '
#define STAR      '*'

char LCD_TITLE[] = "CNS@Paris";
char LCD_MSG[] = "Merry Christmas";

/* -------- Local Variables ------------*/
char lcdMessage[MAX_MSG_LEN] ;   // Buffer where to have the LCD message to display
byte lcdMessageLen ;
unsigned long LCDstartTime;
byte msgIndex;                    // To follow the typing of the message 
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_DATA4_PIN, LCD_DATA5_PIN, LCD_DATA6_PIN, LCD_DATA7_PIN);



/* ----- State machine vars  ------ */
byte lcdState = 0, typingState;

/* lcdReset()
 *  
 *  Prints the Title on the LCD, backlight on, and puts the state machine on idle.
 *  This will leave the Title on the screen
*/
void lcdReset() {
 
  LCDstartTime = millis();
  printTitle();
  lcdBackLight(1);

  msgIndex = 0;
  lcdState = 8;
  typingState = 0;
  
}

/*
 * Turns on and off the LCD backlight.
 * 
 * 0 is off, <>0 is on.
 * 
*/
void lcdBackLight(byte onOff){
    digitalWrite(LCD_BACKLIGHT_PIN, (onOff)?HIGH:LOW);
  }


/*
 * To be called at setup()
 * 
*/
void setupLCD() {
  lcd.begin(COLS, ROWS)  ;
  pinMode(LCD_BACKLIGHT_PIN,OUTPUT);
  lcdBackLight(0);
  lcd.clear();
  lcdReset();
}

/*
 * Clears the LCD, turns off the backlight and puts the state machine to idle.
 * 
*/
void lcdOff() {
  lcd.clear();
  lcdBackLight(0);

  msgIndex = 0;
  lcdState = 9;
  typingState = 0;
  
}

/*
 * Given a message of a certain len, it will write it on the LCD centered, putting margins or cutting text as needed to assure the centered layout
 * 
*/
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


/*
 * Clears the LCD and prints these 2 lines, centered:
 * 
 *    TITLE
 *     MSG
 * 
*/
void printTitle()
{
  lcd.noAutoscroll();
  lcd.clear();
  lcd.setCursor(0, 0);
  printCentered(LCD_TITLE, sizeof(LCD_TITLE));
  lcd.setCursor(0, 1);
  printCentered(LCD_MSG, sizeof(LCD_MSG));
 
}


/* startMesage
 *  
 *  It will clear the screen and type the message msg.
 *  If the msg is larger than the screen, it will autoscroll.
 *  
 *  This starts the state machine, so call only when it is on Idle state, call lcdIsIdle() before.
 *  
*/
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

/* lcdStateMachine()
 *  
 *  To be called on loop()
 *  
    The state machine displays the Title on the screen and goes into idle.
    To kick it out of idle, a message must be sent with startMesage(). Then it will retain the message, then show and retain the Title, and back to idle.
    
*/
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

/*

  Code released under MIT license

  Copyrigh 2017 Jose Ignacio Tamayo Segarra (jose.ignacio.tamayo@gmail.com) and Victor Boglea (bvictor@viking-tech.ro)

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

