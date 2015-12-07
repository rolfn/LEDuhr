
#include "RN-utils.h"
#include <avr/wdt.h>
#include <Arduino.h>

void restart_timer_0(void) {
// enable timer 0 overflow interrupt
// --> wiring.c
#if defined(TIMSK) && defined(TOIE0)
  sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
  sbi(TIMSK0, TOIE0);
#else
  #error  Timer 0 overflow interrupt not set correctly
#endif
}

//void(* resetFunc)(void) = 0;
void reboot(void) {
  wdt_enable(WDTO_15MS);
  noInterrupts();
  while(1); // wait to die and be reborn...
}




