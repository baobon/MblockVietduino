#include "Vietduino_LM35.h"


Vietduino_LM35::Vietduino_LM35(unsigned int b_port) {
  port = b_port;
  isRun = false;
  kalman_lm35 = new SimpleKalmanFilter(2, 2, 0.1);
}

bool Vietduino_LM35::begin() {
  isRun = true;
}
bool Vietduino_LM35::stop() {
  isRun = false;
}

float Vietduino_LM35::readTemperature() {
  if (isRun == true) {
    tempc = analogRead(port);
    tempc = (5.0 * tempc * 1000.0) / (1024 * 10);
    tempc = this -> kalman_lm35 -> updateEstimate(tempc);
  }
  else if (isRun == false) {
    tempc = 0;
  }
  return tempc;
}
