#include <SoftwareSerial.h>
#include <Esplora.h>

SoftwareSerial soft_serial(11, 3); // RX, TX

int rgb[3];
int cols = 0;
unsigned long data =0;


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

void setup()
{
  Serial.begin(9600);       // initialize serial communication with your computer
  // set the data rate for the SoftwareSerial port
  soft_serial.begin(9600);
  soft_serial.println("Hello software serial world?");
} 

void loop()
{
  int x_val = Esplora.readJoystickX();        // read the joystick's X position
  int y_val = Esplora.readJoystickY();        // read the joystick's Y position
  
  set_rgb(x_val, y_val);
  write_rgb(rgb);
  
   soft_serial.print(data);
   soft_serial.print(' ');
   ++data;
   ++cols;
   if(cols == 10) {
     cols = 0;
     soft_serial.println();
     if(data % 200 == 0) 
       soft_serial.println("Software Serial active.");
   }
    
  delay(10);                                  // a short delay before moving again
}

