#ifndef I2CLED_H
#define I2CLED_H  1

#include "i2c/i2cmaster.h"

#define SET_DP 0x80

extern void SS_Init(uint8_t addr);
extern void SS_SetDigitRaw(uint8_t addr, uint8_t digit, uint8_t data);
extern void SS_BlankDigit(uint8_t addr, uint8_t digit);
extern void SS_SetDigit(uint8_t addr, uint8_t digit, uint8_t data);
extern void SS_SetColon(uint8_t addr, uint8_t data);
extern void SS_SetDigits(uint8_t addr, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t colon);
extern void SS_Integer(uint8_t addr, int data, uint8_t base);
extern void SS_SetBrightness(uint8_t addr, uint8_t brightness);  //setBrightness(brightness)

#endif // I2CLED_H
