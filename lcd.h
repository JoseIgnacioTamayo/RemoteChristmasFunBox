
#define COLS  16
#define ROWS  2

#define TITLE_BLINK_TIME_MS  1000
#define TYPING_SPEED_MS  500

#define MAX_MSG_LEN 32


#define SPACE ' '
#define STAR '*'

// To be called at setup()
void setupLCD();

// To be called at inside the loop()
void showLCD();

//
void printTitle(char* msg , int len);

//
void lcdReset();
 
// External variables
extern char lcdMessage[];
extern byte lcdMessageLen;
