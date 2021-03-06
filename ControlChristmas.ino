/*

    Telnet to the Arduino to command ChristmasBox

    Based on several online projects and examples, from where some code was taken (thanks you all for sharing your knowledge, experience and code).

    This project is about playing some Christmas music, making a servo-motor dance, and showing a message on the LCD Screen.
    All these functions controlled over TELNET

    The whole project uses state machines, there is no single delay() instruction to allow the program to run *parallel* tasks.
    >> Well, the libraries used have some small delays, but as the state machines work at the milisecond level, there is the appeareance of parallelism.

*/

#include <Ethernet.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#include "telnet.h"
#include "music.h"
#include "lcd.h"


/* ---------- Global Variables------------- */
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(172, 20, 1, 111);
IPAddress gw(172, 20, 1, 1);
IPAddress netmask(255, 255, 255, 0);
#define TELNET_PORT    2222

/*  --------- Circuit Pins Definitions --------*/

#define SPEAKER_PIN    3
#define SERVO_PIN      8

#define LED1_PIN        5
#define LED2_PIN        6
#define LED3_PIN        7
#define N_LEDS          3

#define SDCARD_SS_PIN  4
//The Ethernet Shield already uses pins  10, 11, 12, 13 and 4

#define LCD_DATA7_PIN   14
#define LCD_DATA6_PIN   15
#define LCD_DATA5_PIN   16
#define LCD_DATA4_PIN   17
#define LCD_EN_PIN      18
#define LCD_RS_PIN      19
#define LCD_BACKLIGHT_PIN      9


/* --------- Initialize Hardware ------------- */
EthernetServer server(TELNET_PORT);
EthernetClient client;


/* ---------- Local Functions ---------*/

/* hasTimePassed()
 *  Macro to easily test for time delays
 *  True if B milisecs have passed since A, where A is the result of millis() called previously
*/
#define hasTimePassed(A,B)   (millis() - (A))>=(B)

/* strcmpn

    Compares 2 strins. Returns true if they are equal.
    This compares up to n chars, or until one of the strings finishes with '\0'

*/
boolean strcmpn(char* strA, char* strB, byte n) {
  byte index = 0;
  while ( (index < n) && (strA != '\0') && (strB != '\0') ) {
    if (*strA != *strB) return false;
    strA++; strB++; index++;
  }
  return true;
}

/* printShow

   Replies to the TELNET Client the status of the Christmas variables

*/
void printShow() {

  client.print(" * Music = ");
  if (enableMusic) client.println( "YES" );
  else client.println( "NO" );

  client.print(" * Dancing = ");
  if (enableDancing) client.println( "YES" );
  else client.println( "NO" );

  client.print(" * Blinking = ");
  if (enableBlink) client.println( "YES" );
  else client.println( "NO" );

}

/* doAction

    Gets the command text, just as input from the TELNET client.
    This is where the command is checked, and the actions taken.

    Input args is the input command, as read from TELNET session

    Returns 1 if an action was taken, and control continues.
    Returns 9 is byebye.
*/
byte doAction(char* args) {

  /* ---------- TELNET COMMANDS ------------- */
  char DANCE_COMMAND[] = "dance";
  char MUSIC_COMMAND[] = "music";
  char FAST_COMMAND[] = "fast";
  char BLINK_COMMAND[] = "blink";
  char MSG_COMMAND[] = "msg ";    //notice the 1 space after
  char NO_COMMAND[] = "no ";    //notice the 1 space after
  char BYE_COMMAND[] = "exit";
  char SHOW_COMMAND[] = "show";

  bool enableAction = true;  //By default, positive commands
  byte actionTaken = 0;      //By defaultm, no action was taken

  //Negative commands start by NO_COMMAND
  if  (  strcmpn(NO_COMMAND, args,  sizeof(NO_COMMAND) - 1) ) {
    ////Serial.println("Found negative command");
    // Shift the negative part of the commands
    args += sizeof(NO_COMMAND) - 1 ;
    enableAction = false;
  }
  // Set the enable actions
  if (strcmpn(DANCE_COMMAND, args, sizeof(DANCE_COMMAND) - 1 ))     {
    enableDancing = enableAction ;
    ////Serial.println("Dancing command");
    if (enableDancing ) {
      lcdOff();
      resetDancer();
    } else {
      offDancer();
      lcdReset();
      lcdBackLight(1);
    }
    actionTaken = 1;
  }
  if ( strcmpn(MUSIC_COMMAND, args, sizeof(MUSIC_COMMAND)  - 1 ) )     {
    enableMusic = enableAction ;
    ////Serial.println("Music command");
    actionTaken = 1;
  }
  if (strcmpn(FAST_COMMAND, args , sizeof(FAST_COMMAND)  - 1 ))     {
    enableFast = enableAction ;

    actionTaken = 1;
  }
  if (strcmpn(BLINK_COMMAND, args , sizeof(BLINK_COMMAND)  - 1 ))     {
    enableBlink = enableAction;
    if (! enableBlink) lcdBackLight(0);
    actionTaken = 1;
  }

  // Bye command, is only positive
  if ( (enableAction) && (strcmpn(BYE_COMMAND, args , sizeof(BYE_COMMAND)  - 1 )))     {
    ////Serial.println("Client bye");
    actionTaken = 9;
  }

  // Show command is only positive
  if ( (enableAction) && (strcmpn(SHOW_COMMAND, args , sizeof(SHOW_COMMAND)  - 1 )))    {
    ////Serial.println("Show command");
    printShow();
    actionTaken = 1;
  }


  // Message command is only positive
  if ( (enableAction) && (strcmpn(MSG_COMMAND, args , sizeof(MSG_COMMAND)  - 1 )))     {
    byte i;
    ////Serial.println("Msg command");
    args += sizeof(MSG_COMMAND) - 1 ;
    if (lcdIsIdle()) startMesage(args);
    actionTaken = 1;
  }

  return actionTaken;

}


/* ---------- TELNET State machine ------------- */
#define TCP_BUFFER_SIZE  50
byte connectionState = 0;
byte bufferPointer = 0;
char bufferIn[TCP_BUFFER_SIZE];


void telnetStateMachine()
{
  switch (connectionState) {

    case 0:  // listen for incoming clients

      client = server.available();
      if (client) {
        connectionState = 1;
        client.flush();
        //Serial.println("We have a new client");

        client.println(BANNER);
        client.print(PROMPT);
        //Serial.println("Say Hello, expect command");

        connectionState = 2;

      }
      break;

    case 2: //Expecting any input

      if (client.connected()) {
        if (client.available()) {

          // Input detected, there is something to read to the buffer
          connectionState = 3;
          //Serial.println("Reading Data...");

          //Clearing the buffer
          bufferPointer = 0;
          memset(bufferIn, 0, TCP_BUFFER_SIZE);
        }
      } else {
        connectionState = 9;
      }
      break;

    case 3: // Reading the data from the client, interpreting and doing
      char c ;
      if (client.available() && (bufferPointer < (TCP_BUFFER_SIZE - 1 )) ) {
        c  = client.read();
        bufferIn[bufferPointer++] = c;
        //Serial.print(c);
      } 
      
      if (c == '\n'){     // Done reading into buffer

        //Serial.println("\nReceived command");
        switch ( doAction(bufferIn) ) {
          case 1:
            //Serial.println("Done something");
            client.println(OK_MSG);
            connectionState = 2;
            client.print(PROMPT);
            break;
          case 9:
            client.println(BYE_MSG);
            client.stop();
            //Serial.println("Client out!");

            //Clearing the buffer
            bufferPointer = 0;
            memset(bufferIn, 0, TCP_BUFFER_SIZE);

            connectionState = 0;
            break;
          default:
            //Serial.println("Done nothing");
            client.println(ERROR_MSG);
            client.print(PROMPT);
            connectionState = 2;
            break;
        }
      }

      break;


    default:
      connectionState = 0;
  }
}


void setup() {

  // start the Ethernet connection:
  Ethernet.begin(mac, ip, gw, netmask) ;
  server.begin();

  //Avoid the SD Card reader
  pinMode(SDCARD_SS_PIN, OUTPUT);
  digitalWrite(SDCARD_SS_PIN, HIGH);

  setupMusic();
  setupLCD();

  // Open serial communications and wait for port to open:
  // Serial.begin(9600);
  //while (!Serial) ;   // wait for serial port to connect. Needed for native USB port only
  // Serial.print("Control Christmas address:");
  // Serial.println(Ethernet.localIP());

}

void loop() {
  telnetStateMachine();
  musicStateMachine();
  lcdStateMachine();
}

/*

  Code released under MIT license

  Copyrigh 2017 Jose Ignacio Tamayo Segarra (jose.ignacio.tamayo@gmail.com) and Victor Boglea (bvictor@viking-tech.ro)

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
