/*
 see: https://github.com/udoklein/dcf77
*/

#include <dcf77.h>
#include <ht16k33.h>
#include <Adafruit_i2c_7seg_LED.h>
#include <OneButton.h>
#include "RN-utils.h"
#include "button.h"

const uint8_t dcf77_analog_sample_pin = 5;
const uint8_t dcf77_sample_pin = A5;
const uint8_t dcf77_inverted_samples = 1;
const uint8_t dcf77_analog_samples = 0;
const uint8_t dcf77_pull_up = 0;
const uint8_t dcf77_monitor_led = 13;

Adafruit_LED DISP1;
Adafruit_LED DISP2;

OneButton button(A1, true); // active low

uint8_t sample_input_pin() {
  if (!syncing) button.tick();
  const uint8_t sampled_data = dcf77_inverted_samples ^
    digitalRead(dcf77_sample_pin);
  digitalWrite(dcf77_monitor_led, sampled_data);
  return sampled_data;
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Leonardo: wait for serial monitor

  pinMode(dcf77_monitor_led, OUTPUT);
  pinMode(dcf77_sample_pin, INPUT);
  digitalWrite(dcf77_sample_pin, dcf77_pull_up);

  DCF77_Clock::set_input_provider(sample_input_pin);
  restart_timer_0();

  button.attachClick(click);
  button.attachDoubleClick(doubleclick);
  button.attachLongPressStart(longPressStart);
  button.attachLongPressStop(longPressStop);
  button.setPressTicks(1000);

  DISP1.begin(0x00);
  DISP2.begin(0x01);

  theMode = SHOW_DATE;
  syncing = true;
  alarmActive = false;
}

void paddedPrint(BCD::bcd_t n) {
  Serial.print(n.digit.hi);
  Serial.print(n.digit.lo);
}

void loop() {
  Clock::time_t now;
  uint8_t match, state;
  static bool setupNeeded = true;

  DCF77_Clock::get_current_time(now);
  state = DCF77_Clock::get_clock_state();

  using namespace Internal;
  typedef DCF77_Clock_Controller<Configuration,
    DCF77_Frequency_Control> Clock_Controller;

  if (syncing) {
    if (setupNeeded) {
      setupNeeded = false;
      DISP1.sleep();
      DISP2.sleep();
      Clock_Controller::Local_Clock.setup();
      DCF77_Clock::setup();
      Serial.println(F(" SYNCING "));
    } else if (state != Clock::useless && state != Clock::dirty) {
      DISP1.normal();
      DISP2.normal();
      setupNeeded = true;
      syncing = false;
    }
  } else {
    DISP1.setDigit(DIGIT_1, now.hour.digit.hi);
    DISP1.setDigit(DIGIT_2, now.hour.digit.lo);
    DISP1.setDigit(DIGIT_3, now.minute.digit.hi);
    DISP1.setDigit(DIGIT_4, now.minute.digit.lo);

    if (alarmActive) {
      DISP1.setPoint(POINT_UPPER_LEFT);
    } else {
      DISP1.clearPoint(POINT_UPPER_LEFT);
    }

    switch (theMode) {
      case SHOW_DATE:
        DISP1.togglePoint(COLON);
        DISP2.setDigit(DIGIT_1, now.day.digit.hi ? now.day.digit.hi : BLANK);
        DISP2.setDigit(DIGIT_2, now.day.digit.lo, true);
        DISP2.setDigit(DIGIT_3, now.month.digit.hi ? now.month.digit.hi : BLANK);
        DISP2.setDigit(DIGIT_4, now.month.digit.lo);
        break;
      case SHOW_SEC:
        DISP1.setPoint(COLON);
        DISP2.setDigit(DIGIT_1, BLANK);
        DISP2.setDigit(DIGIT_2, BLANK);
        DISP2.setDigit(DIGIT_3, now.second.digit.hi);
        DISP2.setDigit(DIGIT_4, now.second.digit.lo);
        break;
      case SHOW_QTY:
        match = DCF77_Clock::get_prediction_match();
        DISP2.setDigit(DIGIT_1, BLANK);
        DISP2.setDigit(DIGIT_2, match / 100 % 10 ? match / 100 % 10 : BLANK);
        DISP2.setDigit(DIGIT_3, match / 10  % 10 ? match / 10  % 10 : BLANK);
        DISP2.setDigit(DIGIT_4, match % 10);
    }
  }

  switch (state) {
    case Clock::useless:  Serial.print(F("useless"));  break;
    case Clock::dirty:    Serial.print(F("dirty"));    break;
    case Clock::free:     Serial.print(F("free"));     break;
    case Clock::unlocked: Serial.print(F("unlocked")); break;
    case Clock::locked:   Serial.print(F("locked"));   break;
    case Clock::synced:   Serial.print(F("synced"));   break;
    default:              Serial.print(F("undefined"));
  }

  Serial.print(' ');
  Serial.print(F("20"));
  paddedPrint(now.year);
  Serial.print('-');
  paddedPrint(now.month);
  Serial.print('-');
  paddedPrint(now.day);
  Serial.print(' ');

  paddedPrint(now.hour);
  Serial.print(':');
  paddedPrint(now.minute);
  Serial.print(':');
  paddedPrint(now.second);

  Serial.print("+0");
  Serial.print(now.uses_summertime? '2': '1');
  Serial.print(F("  "));
  Serial.print(millis());
  Serial.print(F("  "));
  Serial.print(DCF77_Clock::get_overall_quality_factor());
  Serial.print(F("  "));
  Serial.print(DCF77_Clock::get_prediction_match());
  Serial.print(F(" mod:"));
  Serial.print(theMode);
  Serial.println();
}

/*

https://github.com/t3db0t/Button
https://github.com/mathertel/OneButton !!!

https://github.com/adafruit/Adafruit-LED-Backpack-Library
https://learn.sparkfun.com/tutorials/using-the-serial-7-segment-display/example-3-i2c
https://github.com/lpaseen/ht16k33/blob/master/ht16k33.cpp

*/

