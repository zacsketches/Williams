#include <Packet_vector.h>
#include <Packet_parser.h>
#include <Servo.h>


#define DEBUG_MOTOR 0  //set to 1 for debug print to debug serial monitor

#ifdef DEBUG_MOTOR
	
  #include <SoftwareSerial.h>   // RX, TX - only when using the UNO to 
  SoftwareSerial debug(10,11);	// test data coming from the joystick

#endif /* DEBUG_MOTOR */

//from servo conf our right servo is centered at 91
//from servo conf our left servo is centered at 87

Packet_parser parser;
char lt_cmd[3] = {'0', '9', '0'};  //lt_cmd and rt_cmd are updated by the parser and read
char rt_cmd[3] = {'0', '9', '0'};  //by the motor_control function

Servo servo_right, servo_left;

const int servo_right_pin = 10;
const int servo_left_pin = 9;

const int servo_right_offset = 2;
const int servo_left_offset = -3;
const int servo_center = 90;
const int rot_spd = 15;

/* not sure if I need this
int r_val = servo_center + servo_right_offset;
int l_val = servo_center + servo_left_offset;
*/

void setup() {
  Serial.begin(57600);

  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
  
  //instruct parser to monitor for motor control commands.
  parser.add_packet(3, 'R');    //monitor R packet for a payload of three chars.
  parser.add_packet(3, 'L');    //monitor L packet for a poaylot of three chars

  #ifdef DEBUG_MOTOR
    debug.begin(57600);
  #endif /* DEBUG_MOTOR */

}


void loop() {

  smile();
  
  while(parser.listen());
  
  motor_control();

  delay(10);
}

void motor_control() {
  parser.query('L', lt_cmd);
  int mtr_lt = atoi(lt_cmd);
  
  parser.query('R', rt_cmd);
  int mtr_rt = atoi(rt_cmd);
  
  servo_left.write(mtr_lt);
  servo_right.write(mtr_rt);
  
  #ifdef DEBUG_MOTOR
    debug.print("lt_mtr is: ");
    debug.print(mtr_lt);
    debug.print("\trt_mtr is: ");
    debug.println(mtr_rt);
  #endif /* DEBUG_MOTOR */
}

