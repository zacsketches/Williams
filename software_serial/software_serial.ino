//Software Serial Example with Esplora tinkerkit connections on D11 and D3.
//In order to keep the main pins for the Esplora available for the display I
//will need to implement xBee comms via software serial on pins D11 and D3.

//This sketch successfully transmitted on D3 to a directly connected Arduino 
//Uno.  Connect from the Pin 3 "orange" connection to the pin 0 on the 
//Uno.  You must also connect ground between the two arduinos.

//The next step is to connect this sketch to the xBee DIN and successfully
//transmit this Serial data via wireless comms.

//SUCCESS!! I'm connected from the Esplora through the Software Serial to the 
//receiving xBee and displaying data in a Putty session.

#include <SoftwareSerial.h>
#include <Esplora.h>

SoftwareSerial mySerial(11, 3); // RX, TX

int data = 1;

void setup()  {
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello software serial world?");
}

void loop() // run over and over
{
    mySerial.print(data);
    mySerial.println("\t 1");
    ++data;
    delay(1000);

}
