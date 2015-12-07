
#include "RN-utils.h"
#include "Arduino.h"
#include "button.h"

/*
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
https://github.com/mathertel/OneButton
*/

#define VERYLONG_WAIT 3000

uint16_t static volatile longPressMillis;

// ----- button 1 callback functions

// This function will be called when the button was pressed 1 time (and no 2. button press followed).
void click() {
  // Serial.println("Button click.");
  alarmActive = !alarmActive;
} // click


// This function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
  // Serial.println("Button doubleclick.");
} // doubleclick


// This function will be called once, when the button is pressed for a long time.
void longPressStart() {
  // Serial.println("Button longPress start");
  noInterrupts();
  longPressMillis = millis();
  interrupts();
  // noInterrupts();
} // longPressStart


// This function will be called often, while the button is pressed for a long time.
void longPress() {
  // Serial.println("Button longPress...");
  if (millis() - longPressMillis > VERYLONG_WAIT) {
    reboot();
  }
} // longPress


// This function will be called once, when the button is released after beeing pressed for a long time.
void longPressStop() {
  // interrupts();
  // Serial.println("Button longPress stop");
  noInterrupts();
  uint16_t m = millis();
  if (m - longPressMillis > VERYLONG_WAIT) {
    // Serial.println("VERYLONG");
    reboot();
  } else {
    // Serial.println("LONG");
    showDate = !showDate;
  }
  interrupts();
  //longPressMillis = 0; // ???
} // longPressStop
