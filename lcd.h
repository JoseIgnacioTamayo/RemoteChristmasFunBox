
#define COLS  16
#define ROWS  2

#define TITLE_RETAIN_TIME_MS     1000   // Time before refreshing the Title
#define TYPING_INTERVAL_MS       500    // Time between LCD typing letters for the message

#define MAX_MSG_LEN 32     

#define SPACE     ' '
#define STAR      '*'

/* ---------- Public Variables -----------*/
extern char lcdMessage[];
extern byte lcdMessageLen;

// To be called at setup()
void setupLCD();

// To be called at inside the loop()
void lcdStateMachine();

// Clears the LCD Display, and prints the Title msg, of length len, in the center of the upper line
void printTitle(char* msg , int len);

// Clears the LCD and resets the StateMachine
void lcdReset();
 

