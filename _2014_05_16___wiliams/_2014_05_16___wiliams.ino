#include <math.h>

const int batt_pin = A0;
const int analogInX = A1;  // Analog input pin that the potentiometer is attached to
const int analogInY = A2;  // Analog input pin that the potentiometer is attached to
const int analogInZ = A3;  // Analog input pin that the potentiometer is attached to
const int system_led = 13;
const int smile_led1 = 6;    // LED connected to digital pin 9
const int smile_led2 = 11;    // LED connected to digital pin 11

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(57600); 
}

void loop() {
  static long time = 0;
  static long time_batt = 0;
  static boolean batt_stat = HIGH;
  
  while (batt_stat == LOW) {
    for ( int counter = 0, counter < 3, counter++{
      digitalWrite(system_led, !digitalRead(system_led));
      delay(300);
    }
    delay(2000);
  }
  
  if (millis() > (time + 100)){
    time = millis();
    Serial.print(time);    
    Serial.print("\t");      

    if (time_batt + 30000 <= time){
    float batt_vol = float(map(analogRead(batt_pin), 0, 1023, 0, 496));
    Serial.print("Batt_V=");
    Serial.print(batt_vol/100, 2);    
    Serial.print("\t");
    if (batt_vol <= 330) {
        Serial.print("Batt_V=");
        batt_stat = LOW;
    }
    time_batt = time;
    } else {Serial.print("\t"); Serial.print("\t");}

    float accX = float(map(analogRead(analogInX), 509-103, 509+103, -10000, 10000)) / 10000;
    float accY = float(map(analogRead(analogInY), 506-103, 506+103, -10000, 10000)) / 10000;
    float accZ = float(map(analogRead(analogInZ), 522-103, 522+103, -10000, 10000)) / 10000;
    Serial.print(accX, 3);    
    Serial.print("\t");      
    Serial.print(accY, 3);    
    Serial.print("\t");      
    Serial.print(accZ, 3);    
    Serial.print("\t");

    float pitch = RAD_TO_DEG * atan2(accY, sgn(accX) * sqrt((accX * accX)+(accZ * accZ)));
    float roll  = RAD_TO_DEG * atan2(accZ, sgn(accX) * sqrt((accX * accX)+(accY * accY)));
    Serial.print("\t");  
    Serial.print(pitch, 2);
    Serial.print("\t");   
    Serial.print(roll, 2);
   
    int sum = map (pitch, -90, 90,    0, 255);
    int dif = map (roll , -90, 90, -127, 127);
//    Serial.print("\t");      
//    Serial.print(sum);    
//    Serial.print("\t");      
//    Serial.print(dif);
//    analogWrite(smile_led1, sum - dif );         
//    analogWrite(smile_led2, sum + dif);         
    
    Serial.println();    
  }     
}


static inline int8_t sgn(float val) {
  if (val < 0) return -1;
  return 1;
}

