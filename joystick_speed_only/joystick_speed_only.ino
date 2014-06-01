/*  
    develop a proportional speed command for Williams based off joystick
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
const char speed_header = 'S';

//global variables set by the stick and read to find motor commands
int rgb[3];        //values of red, green and blue for onboard LED
                   
const int y_max = 40;  //max amount of servo power for y_stick response

void setup()
{
//  xBee.begin(9600);  //for talking to the test arduino
  xBee.begin(57600);
} 

void loop()
{
  static int speed = 0;

  //read joystick input
  int x_val = Esplora.readJoystickX();      // read the joystick's X position
  int y_val = Esplora.readJoystickY();      // read the joystick's Y position
  
  //make the RBG LED dance
  set_rgb(x_val, y_val);
  write_rgb(rgb);
  
  //transform joystick input into motor commands
  speed = find_speed(y_val, y_max);
  
  //send motor commands to Williams via software serial
  send_motor_cmds(speed);
  
  delay(10);                            
}

void send_motor_cmds(const int spd) {
    
  char spd_buf [4];
  sprintf (spd_buf, "%03d", spd); 
  
  xBee.print(packet_header);
  xBee.print(speed_header);
  xBee.print(spd_buf);
  delay(1);
  xBee.println();
}

int find_speed(const int stick_y, const int max_y) {
//stick_y is read from joystick, max_y is the maximum allowable
//return value for spd.

  // mapped to plus or minus 100%
  int base_mag = map(stick_y, -512, 512, 100, -100);  
  
  int spd = base_mag * max_y / 100;
  
  return spd;  
}


