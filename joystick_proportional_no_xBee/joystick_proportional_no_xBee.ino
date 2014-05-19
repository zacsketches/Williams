#include <Esplora.h>

/*  develop proportional motor commands for Williams based off joystick
    input.  Output the control solution to serial for debugging.

*/

int rgb[3];        //values of red, green and blue for onboard LED

int y_base[2];     //vector of motor servo commands { servo_left, servo_right }
int dir_mod[2];    //directional modifiers to base values to implement steering
int motor_cmd[2];  //vector sum of y_base and dir_mod

const int max_y = 70;  //70/90 = max amount of servo power for y movement

int* find_base(const int stick_y, int* y_base_vals) {
//motor vals is int[2].  stick_y is read from joystick
//returns pointer to motor_vals with motor_vals[0] = servo_left

  int base_mag = map(stick_y, -512, 512, 100, -100);
  
  short r_mult = -1;  //servos are mounted opposite one another so
                      //we have to multiply by -1 for the right servo
                      //to match the left servo's direction
  short l_mult =  1;
  
  int lt_y = l_mult * base_mag * 70 / 100;
  int rt_y = r_mult * base_mag * 70 / 100;
  
  y_base_vals[0] = lt_y;
  y_base_vals[1] = rt_y;
  
  return y_base_vals;
}

int* find_dir_mods(const int stick_x, const int* y_mag, int* dir_mods){
  short lt_dir = 0;
  lt_dir = (y_mag[0] >=0 )  ? 1 : -1; 
  
  short rt_dir = 0;
  rt_dir = (y_mag[1] >=0 ) ? 1 : -1;
}

void stick_print(const int x_val, const int y_val, const int* rgb_val,
                 const int l_mag, const int r_mag) {
  Serial.print("Joystick X: ");                // print a label for the X value
  Serial.print(x_val);                        // print the X value
  Serial.print("\tY: ");                       // print a tab character and a label for the Y value
  Serial.print(y_val);                      // print the Y value  
  Serial.print("\tl_mag: ");
  Serial.print(l_mag);
  Serial.print("\tr_mag: ");
  Serial.print(r_mag);
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
  
  find_base(y_val, y_base);
  find_dir_mods(x_val, y_base, dir_mod);
  
  stick_print(x_val, y_val, rgb, y_base[0], y_base[1]);
  
  delay(10);                                  // a short delay before moving again
}


