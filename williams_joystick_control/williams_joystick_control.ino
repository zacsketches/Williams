#include <Filter.h>
#include <Packet_parser.h>
#include <Servo.h>


//#define DEBUG_CONFIG   //uncomment for debug print to debug serial monitor
//#define DEBUG_MOTOR    //uncomment for debug print to debug serial monitor

//from servo conf our right servo is centered at 91
//from servo conf our left servo is centered at 87

Packet_parser parser;
char lt_cmd[4] = {'0', '9', '0', '\0'};  //lt_cmd and rt_cmd are updated by the parser and read
char rt_cmd[4] = {'0', '9', '0', '\0'};  //by the motor_control function

Servo servo_right, servo_left;

const int servo_right_pin = 10;
const int servo_left_pin = 9;

const int servo_right_offset = 2;
const int servo_left_offset = -3;
const int servo_center = 90;
const int rot_spd = 15;

//declare two Moving Average objects to smooth bad
//reads from the Serial port listening for Joystick commands
Moving_average lt_servo_cmd(4, 90);
Moving_average rt_servo_cmd(4, 90);

void setup() {
  Serial.begin(57600);

  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
  
  //instruct parser to monitor for motor control commands.
  parser.add_packet(3, 'L');    //monitor L packet for a payload of three chars
  parser.add_packet(3, 'R');    //monitor R packet for a payload of three chars.

  #ifdef DEBUG_CONFIG
    parser.config();  
    Serial.print("lt_mtr is: ");
    Serial.print(lt_cmd);
    Serial.print("\trt_mtr is: ");
    Serial.print(rt_cmd);
    Serial.println();
  #endif /* DEBUG_CONFIG */
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
  
  mtr_lt = lt_servo_cmd.filter(mtr_lt);
  mtr_rt = rt_servo_cmd.filter(mtr_rt);

  servo_left.write( mtr_lt );
  servo_right.write( mtr_rt );
  
  #ifdef DEBUG_MOTOR
    Serial.print("lt string is: ");
    Serial.print(lt_cmd);
    Serial.print("\tlt int is: ");
    Serial.print( mtr_lt );
    Serial.print("\trt string is: ");
    Serial.print(rt_cmd);
    Serial.print("\trt int is: ");
    Serial.print( mtr_rt );
    Serial.println();
  #endif /* DEBUG_MOTOR */
}
