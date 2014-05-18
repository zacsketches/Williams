#include <Esplora.h>

/*  Test sketch to ensure the Esplora hordware is working in accordance 
    the system description.  
    
    I simply want to establish Serial comms with the Arduino IDE 
    serial monitor to see values for the joystick inputs.
*/

void setup()
{
  Serial.begin(57600);       // initialize serial communication with your computer
} 

void loop()
{
  int xValue = Esplora.readJoystickX();        // read the joystick's X position
  int yValue = Esplora.readJoystickY();        // read the joystick's Y position
  Serial.print("Joystick X: ");                // print a label for the X value
  Serial.print(xValue);                        // print the X value
  Serial.print("\tY: ");                       // print a tab character and a label for the Y value
  Serial.println(yValue);                      // print the Y value
  
  delay(10);                                  // a short delay before moving again
}


