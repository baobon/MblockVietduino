/*
    Khai bao Vietduino
    Vietduino_DCmotor LeftMotor(1);
    Vietduino_DCmotor RightMotor(2);

    ------------- Vitural Value -----------
    BackWard  --> 0
    ForWard   --> 1

    Left Motor (dir-7,pwm-6) --> Port I -> 2
    Right Motor (dir-4,pwm-5) --> Port II -> 3  
    
    

    
    ------------ Thuc thi -------------
    motor.forward(Speed)
    motor.backward(Speed)
    motor.stop();
    motor.write(Direction,Speed,Timeout);
*/

#include "Vietduino_DCmotor.h"
//#include "Vietduino_Manager_Priority.h"
#include "Vietduino_NewFunction.h"

Vietduino_DCmotor LeftMotor(1);
Vietduino_DCmotor RightMotor(2);

void setup() {
  // put your setup code here, to run once:
  //    LeftMotor.forward(255);
  LeftMotor.write(1,100,2000);
  
}

void loop() {
  VIETDUINO_UPDATE;
}


CREATE_FUNCTION(myVoid) {
//  LeftMotor.write(1,100,1000);
//  M_DELAY(2000);
//  LeftMotor.stop();
//  M_DELAY(2000);
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(myVoid2) {
  RightMotor.forward(80);
  M_DELAY(2000);
  RightMotor.stop();
  M_DELAY(2000);
  RightMotor.backward(80);
  M_DELAY(2000);
  RightMotor.stop();
  M_DELAY(2000);
  END_CREATE_FUNCTION
}
