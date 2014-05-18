#include <Esplora.h>

/*  Test sketch to ensure the Esplora hordware is working in accordance 
    the system description.  For commit history see repo at
    https://github.com/zacsketches/Williams.git
    
    1. Establish Serial comms with the Arduino IDE 
    serial monitor to see values for the joystick inputs.
      -d1d84a9a48be937a5a92a558cfc36d8b96e44099
      
    2. Give RGB led feedback for the direction and intensity of the joystick
    command.
     -f4e52b3f2aef5df9102da435b6b7ca03f8c6dd37
*/

int rgb[3];

int* set_rgb(const int x_val, const int y_val) {
  //set values in the rgb[] array based on joystick feedback
  /*  calculate RBG LED color and intensity based off stick direction
      to provid on-board feedback for joystick input.
     
                           Red
                            ^ 
                     Blue <   > Yellow
                            |
                          Green
  */
  int red = 0;
  int green = 0;
  int blue = 0;
  int yellow = 0;
  
  if(y_val < 0) {
    red = map(y_val, -512, 0, 255, 0);
  }
  if(y_val >=0) {
    green = map(y_val, 0, 512, 0, 255);
  }
  if(x_val >=0) {
    blue = map(x_val, 0, 512, 0, 255);  
  }
  if(x_val < 0) {
    //trickier case of activating yellow because it is a blend of
    //primary red and green light.
    yellow = map(x_val, -512, 0, 255, 0);
    if(yellow > green)
      green = yellow;
    if(yellow > red)
      red = yellow;
  }
  rgb[0] = red;
  rgb[1] = green;
  rgb[2] = blue;
  
  return rgb;
}

void write_rgb(const int* rgb_val){
   Esplora.writeRGB(rgb_val[0], rgb_val[1], rgb_val[2]); 
}

void stick_print(const int x_val, const int y_val, const int* rgb_val) {
  Serial.print("Joystick X: ");                // print a label for the X value
  Serial.print(x_val);                        // print the X value
  Serial.print("\tY: ");                       // print a tab character and a label for the Y value
  Serial.print(y_val);                      // print the Y value  
  Serial.print("\tRGB led: r,");                // print a label for the X value
  Serial.print(rgb_val[0]);                        // print the X value
  Serial.print("\tg,");                       // print a tab character and a label for the Y value
  Serial.print(rgb_val[1]);                      // print the Y value  
  Serial.print("\tb,");                       // print a tab character and a label for the Y value
  Serial.println(rgb_val[2]);                      // print the Y value  


}

void setup()
{
  Serial.begin(57600);       // initialize serial communication with your computer
} 

void loop()
{
  int x_val = Esplora.readJoystickX();        // read the joystick's X position
  int y_val = Esplora.readJoystickY();        // read the joystick's Y position
  
  set_rgb(x_val, y_val);
  write_rgb(rgb);
  
  stick_print(x_val, y_val, rgb);
  
  delay(10);                                  // a short delay before moving again
}


