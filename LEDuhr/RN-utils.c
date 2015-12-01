
#include "wiring_private.h"

void restart_timer_0() {
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



