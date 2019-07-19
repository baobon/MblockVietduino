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
