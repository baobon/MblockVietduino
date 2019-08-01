#include "Vietduino_LM35.h"

Vietduino_LM35 LM35(A0);

void setup(){  
  Serial.begin(115200);
//  LM35.begin();
}

void loop(){
  LM35.begin();
  float temp = LM35.readTemperature();
  Serial.print("Temperature in C = ");
  Serial.println(temp);
  delay(100);
}
