# IR-remote-controlled-LEDs

## Introduction
This project is based on IR sensors. The objective of this project was to light up a series of LEDs in a particular order using an IR remote controller used at home. 

## Scenario
* We were expected to develop an LED light that can be remotely operated using an IRremote controller used at home (example: TV Remote Controller). The Power for the LED lights should be provided using batteries. The Controller for the light should be an Arduino UNO.
* Volume up and volume down buttons of IR remote should be used to light up and light down the LEDs.
* LEDs should be light on in order when up button is pressed while down button light down the LEDs in reverse order.

## Required components
* Uno board (ATmega328P)
* Bread board
* IR sensor
* 4 - LEDs
* 1 - RGB LED
* 7 - 220ohm resisters
* 9V battery
* Jumper cables

## Circuit Design
![image](https://user-images.githubusercontent.com/87204411/142451161-6463fb85-51c0-4c6d-be00-b404e978c120.png)

## IR sensor frequency
Following code can be used with any IR sensor to detect the frequency values of each button. So each frequency can be used for any button activity.
````
#include <IRremote.h> 

int receiver = 11; 
IRrecv irrecv(receiver); 
decode_results results;

void setup() {
	Serial.begin(9600);
	irrecv.enableIRIn(); 
}

void loop() {
	if (irrecv.decode(&results)) { 
	Serial.println (results.value, HEX);  
	irrecv.resume(); //next value
 	}
}
````

## Project Code
Code this in Arduino IDE and upload to UNO board

````
#include <IRremote.h> //include the library

int receiver = 13; //initialize pin 13 as recevier pin/ sensor pin.
uint32_t Previous; 
IRrecv irrecv(receiver); //create a new instance of receiver
decode_results results; //creating a object in decode_results class

int count =0;
int newcount;  //count the inputs (button presses)

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); //start the receiver
  DDRD |= 0x10 ; // Set D4 as output
  DDRD |= 0x20 ; // Set D5 as output
  DDRD |= 0x40 ; // Set D6 as output
  DDRD |= 0x80 ; // Set D7 as output
  DDRB |= 0x01 ; // Set B1 as output
  DDRB |= 0x02 ; // Set B2 as output
  DDRB |= 0x04 ; // Set B3 as output
}

void loop() {
 if (irrecv.decode(&results)) { //if we have received an IR signal
  if (results.value==0xFFFFFFFF) { //if the signal is received 
      results.value=Previous;
      }
    if(results.value==0xC1CC42BD){ //if the received signal is equal to volume up button
      newcount = count+1;
      if(newcount != count){
        switch(newcount){
          case 1 : PORTD |= 0x10; break;
          case 2 : PORTD |= 0x20; break;
          case 3 : PORTD |= 0x40; break;
          case 4 : PORTD |= 0x80; break;
          case 5 : PORTB |= 0x01; break;
          case 6 : PORTB |= 0x02; PORTB &=~ 0x01; break;
          case 7 : PORTB |= 0x04; PORTB &=~ 0x02; break;
          default : PORTD |= 0x80;
                    PORTD |= 0x40;
                    PORTD |= 0x20;
                    PORTD |= 0x10;
                    PORTB &=~ 0x01;
                    PORTB &=~ 0x02;
                    PORTB |= 0x04;
                    newcount = count;
                    break;
        }
        count = newcount;
      }
      delay(100);
    }
    if(results.value==0xC1CCC23D){ //if the received signal is equal to volume down button
      newcount = count;
      if(newcount = count){
        switch(newcount){
          case 1 : PORTD &=~ 0x10; break;
          case 2 : PORTD &=~ 0x20; break;
          case 3 : PORTD &=~ 0x40; break;
          case 4 : PORTD &=~ 0x80; break;
          case 5 : PORTB &=~ 0x01; break;
          case 6 : PORTB &=~ 0x02; PORTB |= 0x01; break;
          case 7 : PORTB &=~ 0x04; PORTB |= 0x02; break;
          default : PORTD &=~ 0x80;
                    PORTD &=~ 0x40;
                    PORTD &=~ 0x20;
                    PORTD &=~ 0x10;
                    PORTB &=~ 0x01;
                    PORTB &=~ 0x02;
                    PORTB &=~ 0x04;
                    newcount = count;
                    break;
        }
        count = newcount-1;
      }
      delay(100);
    }
      
 Serial.println (results.value, HEX); //display HEX results 
 irrecv.resume(); //next value
 }
 Previous=results.value;
 }
````

## Code Explanation 
1. When the input is equal to the Volume Up button frequency (in my case 0xC1CC42BD), the ‘newcount’ is increased by 1 to show the pressing (count+1, initially count=0).
2. Then when newcount is NOT EQUAL to count, a switch condition with newcount as variable is implemented. Here 7 cases plus default are coded.
3. Here when port 9 which is RGB LEDs second color light up, I have turned off the previous port, port 8 to turn off its color. So RGB color doesn’t mixed up. Same thing has been done in case 7 too.  
````
case 6 : PORTB |= 0x02; PORTB &=~ 0x01; break;
case 7 : PORTB |= 0x04; PORTB &=~ 0x02; break;
````
4. In default, I made newcount equals to count (newcount=count). Because after last RGB color, the code should not be run furthermore.
5. When the input is equal to the Volume Down button frequency (in my case 0xC1CCC23D), another if condition is coded while newcount = count
6. Here, each bulb is turned off one by one when volume down button is pressed. Here also in case 6 and 7, the next bulb is made to be on when the current bulb is off.
````
case 6 : PORTB &=~ 0x02; PORTB |= 0x01; break;
case 7 : PORTB &=~ 0x04; PORTB |= 0x02; break;
````
7. Default is coded as all LEDs are down
8. At the end of second if, made newcount = count -1 so in next condition, the next LED is turned off in order.

## Physical Design
![image](https://user-images.githubusercontent.com/87204411/142451370-598f312b-8d43-4842-b0fb-e9bfca341b35.png)



