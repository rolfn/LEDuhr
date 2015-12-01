
#include <stdlib.h>
#include <string.h>
#include "i2cled.h"

/*
    For Adafruit 7-segment 1.2"/0.56" LED displays (HT16K33);
    see: https://learn.adafruit.com/adafruit-led-backpack/
    Code based on http://w8bh.net/avr/AvrSSD1.pdf changed to work
    with Peter Fleury's I2C Library
      http://homepage.hispeed.ch/peterfleury/avr-software.html
    Rolf Niepraschk, Rolf.Niepraschk@gmx.de, 9/2014
*/

void I2C_WriteByte(uint8_t busAddr, uint8_t data) {
    i2c_start_wait(busAddr+I2C_WRITE);
    i2c_write(data);
    i2c_stop();
}

void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data) {
    i2c_start_wait(busAddr+I2C_WRITE);
    i2c_write(deviceRegister);
    i2c_write(data);
    i2c_stop();
}

uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister) {
    uint8_t data = 0;
    i2c_start_wait(busAddr+I2C_WRITE);
    i2c_write(deviceRegister);
    i2c_start_wait(busAddr+I2C_READ);
    data = i2c_readNak();
    i2c_stop();
    return data;
}


//        ---------------------------------------------------------------------------
//        7-SEGMENT BACKPACK (HT16K33) ROUTINES
//
//   The HT16K33 driver contains 16 bytes of display memory, mapped to 16 row x 8 column output
//   Each column can drive an individual 7-segment display; only 0-4 are used for this device.
//   Each row drives a segment of the display; only rows 0-6 are used.
//
//         1               For example, to display the number 7, we need to light up segments
//      -------            0, 1, 2, 3, and 6. This would be binary 0100.1111 or 0x4F.
//   32 |     | 2
//      | 64  |              Mapping to the   display address memory:
//      -------              0x00   Digit   0 (left most digit)
//   16 |     | 4            0x02   Digit   1
//      | 8   |              0x04   colon   ":" on bit1
//      ------- * 128        0x06   Digit   2
//                           0x08   Digit   4 (right-most digit)
//

#define   HT16K33_ON           0x21      //   turn device oscillator on
#define   HT16K33_STANDBY      0x20      //   turn device oscillator off
#define   HT16K33_DISPLAYON    0x81      //   turn on output pins
#define   HT16K33_DISPLAYOFF   0x80      //   turn off output pins
#define   HT16K33_BLINKON      0x85      //   blink rate 1 Hz (-2 for 2 Hz)
#define   HT16K33_BLINKOFF     0x81      //   same as display on
#define   HT16K33_DIM          0xE0      //   add level (15=max) to byte

static const uint8_t numberTable[] =     // convert number to lit-segments
{
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    0x00, //<blank>
};

void SS_Init(uint8_t addr) {
    I2C_WriteByte(addr, HT16K33_ON);             // turn on device oscillator
    I2C_WriteByte(addr, HT16K33_DISPLAYON);      // turn on display, no blink
    I2C_WriteByte(addr, HT16K33_DIM + 15);       // set max brightness
}

void SS_SetDigitRaw(uint8_t addr, uint8_t digit, uint8_t data) {
// digits (L-to-R) are 0,1,2,3
// Send segment-data to specified digit (0-3) on LED display
  if (digit>4) return;                        // only digits 0-4
  if (digit>1) digit++;                       // skip over colon @ position 2
  digit <<= 1;                                // multiply by 2
  I2C_WriteRegister(addr, digit, data);      // send segment-data to display
}
void SS_BlankDigit(uint8_t addr, uint8_t digit) {
// Blanks out specified digit (0-3) on LED display
  SS_SetDigitRaw(addr,digit,0x00);         // turn off all segments on specified digit
}


void SS_SetDigit(uint8_t addr, uint8_t digit, uint8_t data) {
// display data value (0-F) on specified digit (0-3) of LED display
// set dp if data value > 128
  uint8_t d = data & 0x0F;
  SS_SetDigitRaw(addr, digit, numberTable[d] | (data & 0x80));
  // show value on display
}

void SS_SetColon(uint8_t addr, uint8_t data) {
// 0=off, 1=on
// the colon is represented by bit1 at address 0x04. There are three other single LED
// "decimal points" on the display, which are at the following bit positions
// bit2=top left, bit3=bottom left, bit4=top right

    I2C_WriteRegister(addr,0x04,data<<1);
}

void SS_SetDigits(uint8_t addr, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
  uint8_t colon) {
    SS_SetDigit(addr,0,d0);
    SS_SetDigit(addr,1,d1);
    SS_SetDigit(addr,2,d2);
    SS_SetDigit(addr,3,d3);
    SS_SetColon(addr,colon);
}

void SS_Integer(uint8_t addr, int data, uint8_t base) {
    char st[5]="";
    itoa(data,st,base);                         // convert to string
    uint8_t len = strlen(st);
    if (len>4) return;
    uint8_t blanks = 4-len;                    // number of blanks
    for (uint8_t digit=0; digit<4; digit++)        // for all 4 digits
    {
        if (digit<blanks)                       // right-justify display
          //SS_SetDigit(addr,digit,0x10);           // padding with blanks
          SS_SetDigit(addr,digit,0x0);           // padding with zeros
        else
        {
             char ch = st[digit-blanks];        //   get char for this digit
             if (ch>='a') ch-=87;               //   correct for hex digits
             else ch-='0';                      //   ascii -> numeric value
             SS_SetDigit(addr,digit,ch);             //   display digit
        }
    }
}

void SS_SetBrightness(uint8_t addr, uint8_t brightness) {
  I2C_WriteByte(addr, HT16K33_DIM + ((brightness>0x0F) ? 0x0F : brightness));
}
