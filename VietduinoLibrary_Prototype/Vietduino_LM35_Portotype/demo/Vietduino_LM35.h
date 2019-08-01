#ifndef _VIETDUINO_BUZZER_H_
#define _VIETDUINO_BUZZER_H_

#include <inttypes.h>
#include "Arduino.h"
#include "SimpleKalmanFilter.h"

class Vietduino_LM35{
  public:
  Vietduino_LM35(unsigned int port);
  virtual ~Vietduino_LM35() {
    if(kalman_lm35){
      delete kalman_lm35;
    }
  }
  bool isRun;
  byte port;
  float readTemperature();
  bool begin();
  bool stop();
  float tempc; 
  private :
    SimpleKalmanFilter *kalman_lm35;
};

#endif
