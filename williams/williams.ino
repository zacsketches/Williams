#include <Servo.h>

#define DEBUG_SERVO 1  //set to 1 for debug print to serial monitor

//from servo conf our right servo is centered at 92
//from servo conf our left servo is centered at 88

Servo servo_right, servo_left;

const int servo_right_pin = 10;
const int servo_left_pin = 9;

char incoming_byte = 0;
const char fwd_left = '7';
const char fwd_straight = '8';
const char fwd_right = '9';
const char spin_left = '4';
const char spin_right = '6';
const char stop_all = '5';
const char back_left = '1';
const char back_straight = '2';
const char back_right = '3';

void setup() {
  Serial.begin(57600);
  
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);

}

void loop() {
  if (Serial.available() > 0) {
    incoming_byte = Serial.read();
    if(DEBUG_SERVO) {
      Serial.print("incoming byte is: ");
      Serial.println(incoming_byte);  
    }
    
    switch(incoming_byte){
      case fwd_straight:
          servo_right.write(120);
          servo_left.write(60);
        break;
      case back_straight:
          servo_right.write(60);
          servo_left.write(120);      
        break;
      default:
 //         servo_right.write(90);
 //         servo_left.write(90);
    ;}
  }
}


