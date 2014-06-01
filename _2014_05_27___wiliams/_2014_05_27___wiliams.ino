/* LIBRARIES */
#include <math.h>
#include <FuzzyRule.h>      //https://github.com/zerokol/eFLL.git
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <Servo.h> 
#include <Filter.h>          //https://github.com/zacsketches/Filter.git
#include <Packet_parser.h>   //https://github.com/zacsketches/Packet_parser.git

/* INPUT PINS */
const int batt_pin = A0;   // Analog input for battery power monitoring 
const int analogInX = A1;  // Analog input for accelerometer X axis
const int analogInY = A2;  // Analog input for accelerometer Y axis
const int analogInZ = A3;  // Analog input for accelerometer Z axis
const int pir_in = 2;      // Digital input for PIR sensor
const int sonic_echo = 3;  // Digital input for sonic range finder

/* OUTPUT PINS */
const int sonic_trigger = 4; // trigger signal for sonic range finder (10 us signal required)
const int servo_left = 9;    // Analog output for left servo motor (running foreward)
const int servo_right = 10;  // Analog output for right servo motor (running backward)
const int smile_left = 6;    // LED connected to digital pin 6
const int smile_right = 11;  // LED connected to digital pin 11
const int system_led = 13;   // system led

/* GLOBAL VARIABLES */
const float pitch_center = -15;
const int left_center = 87;
const int right_center = 91;
const int max_speed = 40;      //absolute value of max speed input to servo command
const int max_stability = 45;  //absolute value of max stability input to servo command
const int filter_len = 12;     //number of historical points in the moving averages
const int batt_interval = 3000;//time in ms between battery reads
const float min_batt = 350;      //batt charge in mv that the sketch shuts down 

/* SERIAL COMM SUPPORT */
Packet_parser parser;
char speed_cmd[4] = {'0', '0', '0', '\0'};    //buffer for storing speed command

/* LIBRARY OBJECTS */
Servo left, right;
Fuzzy* fuzzy = new Fuzzy();

Moving_average ma_AccX(filter_len, 500), 
               ma_AccY(filter_len, 500), 
               ma_AccZ(filter_len, 500);

void setup() {
  analogReference(EXTERNAL);

  pinMode(batt_pin, INPUT);
  pinMode(analogInX, INPUT);
  pinMode(analogInY, INPUT);
  pinMode(analogInZ, INPUT);
  pinMode(pir_in, INPUT);
  pinMode(sonic_echo, INPUT);

  pinMode(sonic_trigger, OUTPUT);
  pinMode(servo_left, OUTPUT);
  pinMode(servo_right, OUTPUT);
  pinMode(smile_left, OUTPUT); digitalWrite(smile_left, HIGH);
  pinMode(smile_right, OUTPUT); digitalWrite(smile_right, HIGH);
  pinMode(system_led, OUTPUT);

  fuzzy_rules();
  
  left.attach(servo_left);
  left.write(left_center);
  right.attach(servo_right);
  right.write(right_center);

  Serial.begin(57600); 

  parser.add_packet(3, 'S');    //monitor S packet for a payload of three chars
  
  //fill the moving averages with data
  for(int i = 0; i < filter_len; ++i) {
    ma_AccX.filter (analogRead(analogInX));
    ma_AccY.filter (analogRead(analogInY));
    ma_AccZ.filter (analogRead(analogInZ)); 
  }
  
}

void loop() {
  static int speed = 0;
  static float pitch = 0.0;
  static int fuzzy_output = 90;
  static float batt_vol = 0;
  static long batt_time = 0;
  static long time = 0;
  
  speed = set_speed(max_speed);
  
  pitch = set_pitch();

//  control strategy does not implement a roll requirement yet  
//  float roll  = -RAD_TO_DEG * atan2(accZ, sgn(accX) * sqrt((accX * accX)+(accY * accY)));
  
  fuzzy_output = set_fuzzy(1, pitch, max_stability);
  
  write_servos(speed, max_speed, fuzzy_output);

  long now = millis();
  batt_vol = set_batt_vol(batt_vol, now, batt_time, batt_interval);  //Updates batt_time if req'd
  
  block_on_low_batt(batt_vol, min_batt); 
  
  if(now > (time + 100) ){
    time = now;
    print_status(now, batt_vol, pitch, fuzzy_output);
  }  
}





