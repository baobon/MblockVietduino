/*
 * Ham Write(Time_ON,Time_OFF,_COUNT_,_DELAY); ---> Thời gian chạy lại !!!
 * Hàm isRunning() --> Kiểm tra running -> true khi chạy  -> false khi ngừng !!! 
 * Hàm start();
 * Hàm stop();
 * 
 */

#include "Vietduino_Led.h"
#include "Vietduino_NewFunction.h"

Vietduino_Led Led13(13, 1);


void setup() {
  Led13.write(100,100);  
}

void loop() {
  // DO NOT put your code here:
//   Led13.write(100,100);  
  VIETDUINO_UPDATE;
}

//CREATE_FUNCTION(start_stop_runing){
//  Led13.stop();
//  b_debug("Stop");
//  M_DELAY(5000);
//  Led13.start();
//  b_debug("Start");
//  M_DELAY(5000);
//  END_CREATE_FUNCTION
//}

//Test Start Stop Running
//CREATE_FUNCTION(start_stop_runing){
//  Led13.stop();
//  b_debug("Stop");
//  M_DELAY(5000);
//  Led13.start();
//  b_debug("Start");
//  M_DELAY(5000);
//  END_CREATE_FUNCTION
//}

////Test Running Led
//CREATE_FUNCTION(runingLed){
//  if(Led13.isRunning() == true){
//    Serial.println("Running");
//    M_DELAY(100);
//  }else if(Led13.isRunning() == true){
//    Serial.println("Not Running");
//     M_DELAY(100);
//  }
//  END_CREATE_FUNCTION
//}


void b_debug(String debug){
  Serial.println(debug);
}
