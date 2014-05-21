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

/* INPUTS */
const int batt_pin = A0;
const int analogInX = A1;  // Analog input pin that the potentiometer is attached to
const int analogInY = A2;  // Analog input pin that the potentiometer is attached to
const int analogInZ = A3;  // Analog input pin that the potentiometer is attached to
const int pir_in = 2;      // PIR sensor input
const int sonic_echo = 3;  // echo signal of the sonic range finder

/* OUTPUTS */
const int sonic_trigger = 4; // trigger signal for sonic range finder (10 us signal required)
const int servo_left = 9;   // Analog output for left servo motor
const int servo_right = 10;  // Analog output for right servo motor
const int smile_left = 6;   // LED connected to digital pin 6
const int smile_right = 11; // LED connected to digital pin 11
const int system_led = 13; // system led

/* GLOBAL VARIABLES */
Fuzzy* fuzzy = new Fuzzy();

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
  pinMode(smile_left, OUTPUT);
  pinMode(smile_right, OUTPUT);
  pinMode(system_led, OUTPUT);

  FuzzyInput* fuz_pitch = new FuzzyInput(1);

  FuzzySet* positiv = new FuzzySet(15, 60, 90, 90);
  fuz_pitch->addFuzzySet(positiv); 
  FuzzySet* neutral = new FuzzySet(-45, -5, 5, 45);
  fuz_pitch->addFuzzySet(neutral); 
  FuzzySet* negativ = new FuzzySet(-90, -90, -60, -15);
  fuz_pitch->addFuzzySet(negativ);

  fuzzy->addFuzzyInput(fuz_pitch);

  FuzzyOutput* fuz_velocity = new FuzzyOutput(1);

  FuzzySet* foreward = new FuzzySet(90, 180, 180, 180); 
  fuz_velocity->addFuzzySet(foreward);
  FuzzySet* stop = new FuzzySet(65, 80, 100, 115);
  fuz_velocity->addFuzzySet(stop);
  FuzzySet* backward = new FuzzySet(0, 0, 0, 90);
  fuz_velocity->addFuzzySet(backward);

  fuzzy->addFuzzyOutput(fuz_velocity);

  // FuzzyRule 1
  FuzzyRuleAntecedent* ifFuz_pitchPositiv = new FuzzyRuleAntecedent(); 
  ifFuz_pitchPositiv->joinSingle(positiv); 
  FuzzyRuleConsequent* thenFuz_velocityForeward = new FuzzyRuleConsequent();
  thenFuz_velocityForeward->addOutput(foreward);

  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifFuz_pitchPositiv,  thenFuz_velocityForeward);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // FuzzyRule 2
  FuzzyRuleAntecedent* ifFuz_pitchNeutral = new FuzzyRuleAntecedent(); 
  ifFuz_pitchNeutral->joinSingle(neutral); 
  FuzzyRuleConsequent* thenFuz_velocityStop = new FuzzyRuleConsequent();
  thenFuz_velocityStop->addOutput(stop);

  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifFuz_pitchNeutral,  thenFuz_velocityStop);
  fuzzy->addFuzzyRule(fuzzyRule02);

  // FuzzyRule 3
  FuzzyRuleAntecedent* ifFuz_pitchNegativ = new FuzzyRuleAntecedent(); 
  ifFuz_pitchNegativ->joinSingle(negativ); 
  FuzzyRuleConsequent* thenFuz_velocityBackward = new FuzzyRuleConsequent();
  thenFuz_velocityBackward->addOutput(backward);

  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifFuz_pitchNegativ,  thenFuz_velocityBackward);
  fuzzy->addFuzzyRule(fuzzyRule03);


  Serial.begin(57600); 
}

void loop() {
  static long time = 0;
  static long time_batt = 0;
  static boolean batt_stat = HIGH;

  while (batt_stat == LOW) {
    for ( int counter = 0; counter <= 3; counter++) {
      digitalWrite(system_led, !digitalRead(system_led));
      delay(200);
    }
    delay(2000);
  }

  if (millis() > (time + 100)){
    time = millis();
    Serial.print(time);    
    if (time_batt + 30000 <= time){
      float batt_vol = float(map(analogRead(batt_pin), 0, 1023, 0, 496));
      Serial.print("\t Batt = ");
      Serial.print(batt_vol/100, 2);    
      Serial.print("\t");
      if (batt_vol <= 340) {
        Serial.println("Battery Low !");
        batt_stat = LOW;
      }
      time_batt = time;
    } 
    else {
      Serial.print("\t"); 
      Serial.print("\t");
    }

    float accX = float(map(analogRead(analogInX), 509-103, 509+103, -10000, 10000)) / 10000;
    float accY = float(map(analogRead(analogInY), 506-103, 506+103, -10000, 10000)) / 10000;
    float accZ = float(map(analogRead(analogInZ), 522-103, 522+103, -10000, 10000)) / 10000;
    Serial.print("\t X ");      
    Serial.print(accX, 3);    
    Serial.print("\t Y ");      
    Serial.print(accY, 3);    
    Serial.print("\t Z ");      
    Serial.print(accZ, 3);    
    Serial.print("\t");

    float pitch = RAD_TO_DEG * atan2(accY, sgn(accX) * sqrt((accX * accX)+(accZ * accZ)));
    float roll  = RAD_TO_DEG * atan2(accZ, sgn(accX) * sqrt((accX * accX)+(accY * accY)));
    Serial.print("\t P ");  
    Serial.print(pitch, 2);
    Serial.print("\t R ");   
    Serial.print(roll, 2);

    fuzzy->setInput(1, pitch);
    fuzzy->fuzzify();
    float output = fuzzy->defuzzify(1);
    Serial.print("\t F ");
    Serial.print(output);


    Serial.println();    
  }     
}


static inline int8_t sgn(float val) {
  if (val < 0) return -1;
  return 1;
}


