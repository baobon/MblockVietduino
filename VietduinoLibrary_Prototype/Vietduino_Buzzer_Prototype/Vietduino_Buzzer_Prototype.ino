//---> Vietduino_Buzzer buzzer;
//---> Vietduino_Buzzer buzzer(3);
//Normal buzzer in Pin 3;
//tone = 3000;
//buzzer.write(timeOn,timeOff,count,timeRepeat)

#include "Vietduino_Buzzer.h"
#include "Vietduino_NewFunction.h"

Vietduino_Buzzer buzzer;

void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:
  VIETDUINO_UPDATE;
}

CREATE_FUNCTION(runLed)
{ 
  buzzer.write(2000);
  M_DELAY(3000);
  buzzer.write(300, 300, 4);
  M_DELAY(5000);
  buzzer.write(100, 100, 5, 1000);
  M_DELAY(7000);

  while(1){
    M_DELAY(1);
    buzzer.stop();
  }

  END_CREATE_FUNCTION
}

CREATE_FUNCTION(deBug)
{
  Serial.begin(115200);

  while (1)
  {
    Serial.println(millis());
    M_DELAY(500);
  }

  END_CREATE_FUNCTION
}
