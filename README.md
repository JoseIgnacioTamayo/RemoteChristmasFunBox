# RemoteChristmasFunBox

This project builds a FunBox for Christmas decoration, out of an Arduino Uno + EthernetShield.

The Box plays some Christmas music, makes a servo-motor dance, and shows a merry message on the LCD Screen.
It can play several songs, but we are limited now to the amount of memory available. In the future, music files could be read from the SD Card.
The Servo Motor performs left/right turns of up to 60 degrees, according to the music.
The 16x2 LCD Display shows a Title message on top, and a rolling message below.
There is also a little LED operating as function witness.

All these functions are controlled over TELNET.

> Based on several online projects and examples, where some code was taken (thanks you all for sharing your knowledge, experience and code).

## Circuit ##

 * This project uses the Ethernet Shield, [ref][https://www.arduino.cc/en/Guide/ArduinoEthernetShield#toc1]. The SDCard is disabled for this shield.
Pins 10, 11, 12, 13 and 4 are taken by the EtherShield.

 * A 16x2 LCD display is used. The 6 Analog pins are used as DigitalOutputs, to drive the LCD on 4-bits mode. An additional pin is used to drive the backlight, using a transitor.

 * The speaker used is a **, connected with a 470 Ohm resistor.

 * The witness 5nm LED is connected with a 470 Ohm resistor.

 * The Servo Motor is a **, driven through a BC547 transistor.
    
## TELNET Command ##

Telnet to the IP of this device (fixed IP and TCP Port, for now). You will get the banner in 'banner.h'.

The commands are:

```

  show          Shows the status of the Christmas
  [no] dance    Enables [or disables] the ServoMotor Dancing
  [no] music    Enables [or disables] the Speaker sound
  [no] fast     Accelerates the tempo
  song {1-n}    Plays the song, which ID is the parameter
  msg {message} Shows a message on the LCD

```

> Only one sessions at a time.
> Commands are CasE SenSiTIVe.

To easily change the names and messages, just change them in 'lcd.h'.

## References ##

* https://www.arduino.cc/en/Tutorial/ChatServer
* https://www.arduino.cc/en/Reference/Servo
* http://www.instructables.com/id/How-to-use-a-Buzzer-Arduino-Tutorial/
* https://create.arduino.cc/projecthub/ianabcumming/musical-fairy-lights-ed1445?ref=tag&ref_id=christmas&offset=1
* https://www.arduino.cc/en/Tutorial/LiquidCrystalAutoscroll
* https://create.arduino.cc/projecthub/sfrwmaker/5-minute-christmas-neopixel-led-strip-0abf33?ref=tag&ref_id=christmas&offset=2
