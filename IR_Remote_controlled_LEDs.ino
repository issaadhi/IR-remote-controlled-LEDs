#include <IRremote.h> //include the library

int receiver = 13; //initialize pin 13 as recevier pin.
uint32_t Previous; 
IRrecv irrecv(receiver); //create a new instance of receiver
decode_results results;

int count =0;
int newcount;

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
  if (results.value==0xFFFFFFFF) { 
      results.value=Previous;
      }
    if(results.value==0xC1CC42BD){
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
    if(results.value==0xC1CCC23D){
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
