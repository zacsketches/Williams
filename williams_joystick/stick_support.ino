void find_base(const int stick_y, int* y_base_vals) {
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
  
}

void find_dir_mods(const int stick_x, const int* y_mag, int* dir_mods){
  
  short lt_mod = 0;
  short rt_mod = 0;

  // take the sign of the direction and speed up each wheel
  // from the y_mag passed in  
  short lt_dir = (y_mag[0] >=0 )  ? 1 : -1; 
  short rt_dir = (y_mag[1] >=0 ) ? 1 : -1;
    
  if( stick_x >= 0 ) {    //left stick input then speed up rt wheel
    rt_mod = map(abs(stick_x), 0, 512, 0, 20);    
  } else {                //right stick input then speed up lt wheel
    lt_mod = map(abs(stick_x), 0, 512, 0, 20);
  }
  
  //adust for direction
  lt_mod *= lt_dir;
  rt_mod *= rt_dir;
  
  //set the global array
  dir_mods[0] = lt_mod;
  dir_mods[1] = rt_mod;
}

void find_mtr_vec(int* base, int* mods, int* mtr_cont) {
  //all pointers are two element arrays
  mtr_cont[0] = base[0] + mods[0];
  mtr_cont[1] = base[1] + mods[1];
}
