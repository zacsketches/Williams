
//print stick and RBG values to Serial port
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

