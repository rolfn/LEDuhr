#ifndef RN_UTILS_H
#define RN_UTILS_H  1

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

extern void restart_timer_0();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RN_UTILS_H
