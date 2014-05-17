#include <Servo.h>

#define DEBUG_SERVO 0  //set to 1 for debug print to serial monitor

//from servo conf our right servo is centered at 91
//from servo conf our left servo is centered at 87

Servo servo_right, servo_left;

const int servo_right_pin = 10;
const int servo_left_pin = 9;

const int servo_right_offset = 2;
const int servo_left_offset = -3;
const int r_mult = -10;
const int l_mult = 10;
const int servo_center = 90;

int r_val = servo_center + servo_right_offset;
int l_val = servo_center + servo_left_offset;

char incoming_byte =       '0';
const char fwd_left =      '7';
const char fwd_straight =  '8';
const char fwd_right =     '9';
const char spin_left =     '4';
const char spin_right =    '6';
const char stop_all =      '5';
const char back_left =     '1';
const char back_straight = '2';
const char back_right =    '3';

void setup() {
  Serial.begin(57600);
  
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);

}

char read_serial() {
   //read the serial monitor and return any incoming char
  char res = '0';
  if (Serial.available() > 0) {
    res = Serial.read();
    if(DEBUG_SERVO) {
      Serial.print("incoming byte is: ");
      Serial.println(incoming_byte);  
    }
  }
  return res;  
}

void loop() {
    
    servo_right.write(r_val);
    servo_left.write(l_val);

    incoming_byte = read_serial();
    
    switch(incoming_byte){
      case fwd_straight:
          r_val = 3 * r_mult + servo_center + servo_right_offset;
          l_val = 3 * l_mult + servo_center + servo_left_offset;
        break;
      case back_straight:
          r_val = -3 * r_mult + servo_center + servo_right_offset;
          l_val = -3 * l_mult + servo_center + servo_left_offset;
        break;
      case stop_all:
          r_val = servo_center + servo_right_offset;
          l_val = servo_center + servo_left_offset;
        break;
    }

    if(DEBUG_SERVO) {
      Serial.print("left: ");
      Serial.print(l_val);  
      Serial.print("\t right: ");
      Serial.println(r_val);  
    }

	delay(10);
}


