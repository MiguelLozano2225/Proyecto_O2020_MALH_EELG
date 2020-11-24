/*
 * pcf8574.h
 *
 *  Created on: Oct 16, 2020
 *      Author: migue
 */

#ifndef PCF8574_H_
#define PCF8574_H_

#include "derivative.h"

#define I2C_LCD_ADDR_0	0x27
#define I2C_LCD_ADDR_1	0x23
#define I2C_LCD_ADDR_2	0x26

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04 // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

typedef unsigned char u8;

void print_string(uint8_t *str);
void send(uint8_t value, uint8_t mode);
void write4bits(uint8_t value);
void pulseEnable(uint8_t data);
void expanderWrite(uint8_t data);
void clear(void);
void home(void);
void setCursor(uint8_t col, uint8_t row);
void noDisplay(void);
void display(void);
void noCursor(void);
void cursor(void) ;
void noBlink(void) ;
void blink(void) ;
void scrollDisplayLeft(void);
void scrollDisplayRight(void) ;
void leftToRight(void) ;
void rightToLeft(void) ;
void autoscroll(void) ;
void noAutoscroll(void);
void Init_ALL_LCD(void);



#endif /* PCF8574_H_ */
