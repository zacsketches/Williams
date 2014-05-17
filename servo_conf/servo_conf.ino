/*  Sketch to configure the center point for a pair of continuous rotation
    servos connected on pins 9 and 10.  
    
    This sketch writes '90' to each servo to begin.
    
    Enter '1' to lower the value written to the left servo.
    Enter '7' to raise the value written to the left servo.
   
    Enter '3' to lower the value written to the right servo.
    Enter '9' to raise the value written to the right servo.
   
    Open the serial monitor to view the commands written to each servo.
    
    Make note of the value where the motors stop and use this value
    in other code for the servo pair.
*/

#include <Servo.h>

#define DEBUG_SERVO 1  //set to 1 for debug print to serial monitor

Servo servo_right, servo_left;

const int servo_right_pin = 9;
const int servo_left_pin = 10;

int left_val = 90;
int right_val = 90;

char incoming_byte = '0';
const char left_raise = '7';
const char right_raise = '9';
const char left_lower = '1';
const char right_lower = '3';

void setup() {
  Serial.begin(57600);
  
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);

}

void loop() {
  
  servo_right.write(right_val);
  servo_left.write(left_val);
  
  Serial.print("right: ");
  Serial.print(right_val);
  Serial.print("\tleft: ");
  Serial.println(left_val);

  if (Serial.available() > 0) {
    incoming_byte = Serial.read();
    if(DEBUG_SERVO) {
      Serial.print("incoming byte is: ");
      Serial.println(incoming_byte);  
    }
    
    switch(incoming_byte){
      case left_raise:
        ++left_val;
        break;
      case left_lower:
        --left_val;      
        break;
      case right_raise:
        ++right_val;
        break;
      case right_lower:
        --right_val;
    }
  }
}

