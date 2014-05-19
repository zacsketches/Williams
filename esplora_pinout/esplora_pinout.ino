//before implementing software serial on Esplora I need to be sure about
//the pinout.  I've found a few online refs, but this sketch verifies I 
//can blink an externally conneted led via the "orange" connections
//through D3 and D11 respectively.

#include <Esplora.h>

int led13 = 13;
int led11 = 11;
int led3 = 3;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led13, OUTPUT);     
  pinMode(led11, OUTPUT);     
  pinMode(led3, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led13, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led11, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led13, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led11, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led3, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
}
