#include <Esplora.h>
#include <Servo.h>

class Cont_rot_servo : public Servo {
private:
  int ctr;
  int rot;

public:
  Cont_rot_servo() {
    ctr = 90;
    rot = 1;
  }
  
  int center() { return ctr; }
  int rotation() { return rot; }
};

Cont_rot_servo rt;

void setup() {
  rt.attach(3);

  Serial.begin(9600);

  while(!Serial) {
    delay(10);
  }

  
}

void loop() {
  Serial.print("ctr is: ");
  Serial.print(rt.center());
  Serial.print("\trot is: ");
  Serial.print(rt.rotation());
  Serial.println();
  
  delay(10000);
}
