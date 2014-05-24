/*  
    develop proportional motor commands for Williams based off joystick
    input and send via software serial.
    
    - The control strategy here is for a two wheeled robot driven by
    continuous rotation servos.
    - Servo commans must be in the range [0, 180].
    - Servo center is 90, plus a calibration offset of about 2 or 3
    degrees.
    - For clarity I send motor command on the y axis from [-70, 70]
    where a negative value indicates backward movement.
    - This sketch finds a base value for the y command to control 
    front-to-back movement then adds a modifier to each wheel in the
    range [-20, 20] in order to control steering.
    
    - Software Serial is necessary on D3 and D11 (tx & rx), via the 
    orange tinkerkit connections in order to keep the main pins for 
    the Esplora available for the tft display.  Connect D3 to DIN on
    the xBee for wireless comms.
    - Williams is running the Packet_parser class to read and parse
    incoming serial packets.  This parser requires a packet header
    and a single letter target followed by a and a consistent three 
    digit data packet. ( ||R120 )
 
*/
#include <Esplora.h>
#include <SoftwareSerial.h>

SoftwareSerial xBee(11, 3); // RX, TX

//packet headers necessary to form data packets to Williams.
const char packet_header[2] = {'|', '|'};
const char rt_servo_header = 'R';
const char lt_servo_header = 'L';

//global variables set by the stick and read to find motor commands
int rgb[3];        //values of red, green and blue for onboard LED

int y_base[2];     // array of motor servo commands { servo_left, servo_right }
                   // that represent the base value for each wheel's 
                   // front-to-back motion.  Range of [-70, 70]
                   
int dir_mod[2];    // array of motor servo commans that reflect a 
                   // differential between the two wheels that implements
                   // steering.  Range of [-20, 20]
                   
int motor_cmd[2];  //vector sum of y_base and dir_mod

const int max_y = 70;  //max amount of servo power for y_stick response
const int max_x = 20;  //max amount of servo power for x_stick response


void setup()
{
//  xBee.begin(9600);  //for talking to the test arduino
  xBee.begin(57600);
} 

void loop()
{
  //read joystick input
  int x_val = Esplora.readJoystickX();      // read the joystick's X position
  int y_val = Esplora.readJoystickY();      // read the joystick's Y position
  
  //make the RBG LED dance
  set_rgb(x_val, y_val);
  write_rgb(rgb);
  
  //transform joystick input into motor commands
  find_base(y_val, y_base);
  find_dir_mods(x_val, y_base, dir_mod);
  find_mtr_vec(y_base, dir_mod, motor_cmd);
  
  //send motor commands to Williams via software serial
  send_motor_cmds(motor_cmd);
  
  delay(10);                            
}

void send_motor_cmds(const int* mtr_cmd) {
  //mtr_cmd[2] is formeed {left_cmd, right_command}
  //motor commands stored in the mtr_cmd array are in the range [-90, 90].
  //in order to avoid dealing with the negative sign they are transmitted 
  //in the range [0, 180]
  
  int lt_val = mtr_cmd[0];
  int rt_val = mtr_cmd[1];
  lt_val = map(lt_val, -90, 90, 0, 180);
  rt_val = map(rt_val, -90, 90, 0, 180);
  
  char lt_buf [4];
  sprintf (lt_buf, "%03i", lt_val); 
  char rt_buf [4];
  sprintf (rt_buf, "%03i", rt_val);
  
  xBee.print(packet_header);
  xBee.print(lt_servo_header);
  xBee.print(lt_buf);
  delay(1);
  xBee.print(packet_header);
  xBee.print(rt_servo_header);
  xBee.print(rt_buf);
  delay(1);
  xBee.println();
  
}


