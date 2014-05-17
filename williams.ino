#include <Servo.h>

#define DEBUG_SERVO 1  //set to 1 for debug print to serial monitor

Servo servo_right, servo_left;

const int servo_right_pin = 9;
const int servo_left_pin = 10;

int incoming_byte = 0;
const int fwd_left = 7;
const int fwd_straight = 8;
const int fwd_right = 9;
const int spin_left = 4;
const int spin_right = 6;
const int stop_all = 5;
const int back_left = 1;
const int back_straight = 2;
const int back_right = 3;

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
          servo_right.write(120);
          servo_left.write(60);      
        break;
      default:
          servo_right.write(90);
          servo_left.write(90);
    }
  }
}


