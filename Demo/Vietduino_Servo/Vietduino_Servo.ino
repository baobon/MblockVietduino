#include "Vietduino_Manager_Priority.h"
#include "Vietduino_Servo.h"
#include "Vietduino_NewFunction.h"

Vietduino_Servo     myServo;

void setup() {
  myServo.begin(9);


}

void loop() {
  VIETDUINO_UPDATE;

}

CREATE_FUNCTION(myVoid3) {
  myServo.write_for(0,3000);
  M_DELAY(2000);
  myServo.write_for(180,3000);
  M_DELAY(2000);
  END_CREATE_FUNCTION
}
