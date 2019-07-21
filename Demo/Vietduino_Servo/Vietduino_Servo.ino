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
  myServo.write(90, 10);
  M_DELAY(3000);
   myServo.write(180, 10);
  M_DELAY(3000);
  END_CREATE_FUNCTION
}
