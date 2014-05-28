int find_speed(const int stick_y) {
// y_base_vals is int.  stick_y is read from joystick

  // mapped to plus or minus 100%
  int base_mag = map(stick_y, -512, 512, 100, -100);  
  
  int lt_y = base_mag * max_y / 100;
  
  return lt_y;  
}
