/* Implementation of loop functions. */

// SET SPEED //
int set_speed(const int spd_max) {
  //Query the parser and return the int value of the latest speed command the
  //parser has received.
  while(parser.listen());
  parser.query('S', speed_cmd);
  int speed = atoi(speed_cmd);
 
  //constrain speed
  if (speed < -spd_max) {
    speed = -spd_max;
  } else if (speed > spd_max) {
    speed = spd_max;
  } 
  
  return speed;
}

// SET PITCH //
float set_pitch() {
  int avrX = ma_AccX.filter (analogRead(analogInX));
  int avrY = ma_AccY.filter (analogRead(analogInY));
  int avrZ = ma_AccZ.filter (analogRead(analogInZ));
   
  float accX = float(map(avrX, 509-103, 509+103, -10000, 10000)) / 10000;
  float accY = float(map(avrY, 506-103, 506+103, -10000, 10000)) / 10000;
  float accZ = float(map(avrZ, 522-103, 522+103, -10000, 10000)) / 10000;

  float pitch = RAD_TO_DEG * atan2(accY, sgn(accX) * sqrt((accX * accX)+(accZ * accZ)));

  return pitch;
}

// SET FUZZY OUT //
int set_fuzzy(int rule, float pitch, int stab_max) {
  int output = 0;
  fuzzy->setInput(rule, pitch);
  fuzzy->fuzzify();
  output = fuzzy->defuzzify(rule);
  output -= 90; 
  
  //constrain fuzzy out to max input for stability control
  if (output < -stab_max) {
    output = -stab_max;
  } else if (output > stab_max) {
    output = stab_max;
  }
  
  return output;
}

// WRITE SERVO COMMANDS //
void write_servos(const int spd, const int max_spd, const int fuzzy_out) {
  //the two lines below are the original algorithm carset wrote, but
  //I think there is a potential divide by zero and that the final
  //write command could be out of range for the servos [0 : 180].
  //consider the case where fuzzy_out = 45 and spd = 1.
  // 89 + 40/1 * 45 + 1 = out_of_range
  //  left.write (left_center + (40/spd * fuzzy_out) + spd);
  //  right.write(right_center - (40/spd * fuzzy_out) - spd);
  
  //I think we want something like this
  int percent_spd = map(abs(spd), 0, max_spd, 0, 100);
  int percent_fuzz = 100 - percent_spd;
  int proportional_fuzz = fuzzy_out * percent_fuzz / 100;
  left.write (left_center  + proportional_fuzz + spd);
  right.write(right_center - proportional_fuzz - spd); 
}

// UPDATE BATTERY VOL and BATTERY UPDATE TIME//
float set_batt_vol(const float batt_vol, const long now, long& batt_time, const int interval) {
  //short circuit if the time inverval has not expired
  if (batt_time + interval > now){
    return batt_vol;
  }
  
  float update = float(map(analogRead(batt_pin), 0, 1023, 0, 496));
  batt_time = now;
  return update;
}

// HOLD SKETCH EXECUTION ON LOW BAT //
void block_on_low_batt(const float batt_vol, const float min_batt){
  boolean batt_state = HIGH;
  if(batt_vol <= min_batt) {
      batt_state = LOW;
  }
  
  while(! batt_state) {
    for (int counter = 0; counter <= 5; counter++) {
      digitalWrite(system_led, !digitalRead(system_led));
      delay(150);
    }
    delay(2500);
  }
} 

// PRINT STATUS EVERY 100MS //
void print_status(const long now, const float batt_vol, 
                  const float pitch, const int fuzzy_output) {
  
  Serial.print(now);
  
  Serial.print("\t");
  Serial.print(batt_vol/100, 2);  
  
  Serial.print("\t");      
  Serial.print(ma_AccX.current(), 3);    
  Serial.print("\t");      
  Serial.print(ma_AccY.current(), 3);    
  Serial.print("\t");      
  Serial.print(ma_AccZ.current(), 3);    

  Serial.print("\t");  
  Serial.print(pitch, 2);

  Serial.print("\t");
  Serial.print(fuzzy_output);

  Serial.println();   
} 

//*********************************************************************
//*                         HELPER FUNCTIONS
//*********************************************************************

static inline int8_t sgn(float val) {
  if (val < 0) return -1;
  return 1;
}

