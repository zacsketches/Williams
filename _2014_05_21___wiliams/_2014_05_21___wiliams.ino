/* LIBRARIES */
#include <math.h>
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <Servo.h> 
#include <Filter.h>

/* INPUTS */
const int batt_pin = A0;   // Analog input for battery power monitoring 
const int analogInX = A1;  // Analog input for accelerometer X axis
const int analogInY = A2;  // Analog input for accelerometer Y axis
const int analogInZ = A3;  // Analog input for accelerometer Z axis
const int pir_in = 2;      // Digital input for PIR sensor
const int sonic_echo = 3;  // Digital input for sonic range finder

/* OUTPUTS */
const int sonic_trigger = 4; // trigger signal for sonic range finder (10 us signal required)
const int servo_left = 9;    // Analog output for left servo motor (running foreward)
const int servo_right = 10;  // Analog output for right servo motor (running backward)
const int smile_left = 6;    // LED connected to digital pin 6
const int smile_right = 11;  // LED connected to digital pin 11
const int system_led = 13;   // system led

/* GLOBAL VARIABLES */
const int left_center = 85;
const int right_center = 89;
const int speed = 00;

Fuzzy* fuzzy = new Fuzzy();
Servo left, right;
Moving_average ma_AccX(3), ma_AccY(3), ma_AccZ(3);

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
}

void loop() {
  static long time = 0;
  static long time_batt = 0;
  static float batt_vol = 0;
  static boolean batt_stat = HIGH;

  while (batt_stat == LOW) {
    for ( int counter = 0; counter <= 5; counter++) {
      digitalWrite(system_led, !digitalRead(system_led));
      delay(100);
    }
    delay(2500);
  }

  if (millis() > (time + 100)){
    time = millis();
    Serial.print(time);    
    if (time_batt + 30000 <= time){
      batt_vol = float(map(analogRead(batt_pin), 0, 1023, 0, 496));
      if (batt_vol <= 340) {
//        Serial.println("Battery Low !");
        batt_stat = LOW;
      }
      time_batt = time;
    } 
    Serial.print("\t");
    Serial.print(batt_vol/100, 2);    

    int avrX = ma_AccX.filter (analogRead(analogInX));
    
    
    float accX = float(map(avrX, 509-103, 509+103, -10000, 10000)) / 10000;
    float accY = float(map(analogRead(analogInY), 506-103, 506+103, -10000, 10000)) / 10000;
    float accZ = float(map(analogRead(analogInZ), 522-103, 522+103, -10000, 10000)) / 10000;
    Serial.print("\t");      
    Serial.print(accX, 3);    
    Serial.print("\t");      
    Serial.print(accY, 3);    
    Serial.print("\t");      
    Serial.print(accZ, 3);    

    float pitch = RAD_TO_DEG * atan2(accY, sgn(accX) * sqrt((accX * accX)+(accZ * accZ)));
    float roll  = -RAD_TO_DEG * atan2(accZ, sgn(accX) * sqrt((accX * accX)+(accY * accY)));
    Serial.print("\t");  
    Serial.print(pitch, 2);
    Serial.print("\t");   
    Serial.print(roll, 2);

    fuzzy->setInput(1, pitch);
    fuzzy->fuzzify();
    int output = fuzzy->defuzzify(1);
    output -= 90;
    Serial.print("\t");
    Serial.print(output);

    left.write (left_center + output + speed);
    right.write(right_center - output - speed);

    Serial.println();    
  }     
}


static inline int8_t sgn(float val) {
  if (val < 0) return -1;
  return 1;
}


