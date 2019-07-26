#include "Vietduino_Button.h"
#include "Vietduino_NewFunction.h"

Vietduino_Button button8;

void setup() {
  Serial.begin(115200);
  Serial.println("Test Vietduino_Button Start!");
  button8.begin(8);
}

void loop() {
  VIETDUINO_UPDATE;

  if (button8.available(_CLICKED_)) {
    Serial.println("_CLICKED_");
  }

  if (button8.available(_DOUBLE_CLICKED_)) {
    Serial.println("_DOUBLE_CLICKED_");
  }

  if (button8.available(_PRESSED_)) {
    Serial.println("_PRESSED_");
  }

  if (button8.available(_LONG_CLICKED_)) {
    Serial.println("_LONG_CLICKED_");
  }

    if (button8.available(_DURING_LONG_PRESSED_)) {
    Serial.println("_DURING_LONG_CLICKED_");
  }

}
