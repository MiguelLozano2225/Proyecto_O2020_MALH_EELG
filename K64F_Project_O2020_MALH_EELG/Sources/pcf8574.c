/*
 * pcf8574.c
 *
 *  Created on: Oct 16, 2020
 *      Author: migue
 */
#include "pcf8574.h"
#include "i2c.h"

	/*Global variables*/
 uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t  _oled = 0;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
  
  
void  LCD_Init_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
  {
    _Addr = lcd_Addr;
    _cols = lcd_cols;
    _rows = lcd_rows;
    _backlightval = LCD_NOBACKLIGHT;
  }


  void init()
  {
  	//Wire.begin();
	  I2C_Start;
  	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  	begin(_cols, _rows);  
  }

  void begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  	if (lines > 1) {
  		_displayfunction |= LCD_2LINE;
  	}
  	_numlines = lines;

  	// for some 1 line displays you can select a 10 pixel high font
  	if ((dotsize != 0) && (lines == 1)) {
  		_displayfunction |= LCD_5x10DOTS;
  	}

  	delay_7u(50); 
    
  	// Now we pull both RS and R/W low to begin commands
  	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
  	delay_7u(1000);

    	//put the LCD into 4 bit mode
  	// this is according to the hitachi HD44780 datasheet
  	// figure 24, pg 46
  	
  	  // we start in 8bit mode, try to set 4 bit mode
     write4bits(0x03 << 4);
     delay_7u(600); // wait min 4.1ms
     
     // second try
     write4bits(0x03 << 4);
     delay_7u(600); // wait min 4.1ms
     
     // third go!
     write4bits(0x03 << 4); 
     delay_7u(25);
     
     // finally, set to 4-bit interface
     write4bits(0x02 << 4); 


  	// set # lines, font size, etc.
  	command(LCD_FUNCTIONSET | _displayfunction);  
  	
  	// turn the display on with no cursor or blinking default
  	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  	display();
  	
  	// clear it off
  	clear();
  	
  	// Initialize to default text direction (for roman languages)
  	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  	
  	// set the entry mode
  	command(LCD_ENTRYMODESET | _displaymode);
  	
  	home();
    
  }
  
 /*******************************************************************************************************/

  /********** high level commands, for the user! */
  void clear()
  {
  	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
  	delay_7u(400);  // this command takes a long time!
  }

  void home(){
  	command(LCD_RETURNHOME);  // set cursor position to zero
  	delay_7u(400);  // this command takes a long time!
  }

  void setCursor(uint8_t col, uint8_t row){
  	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  	if ( row > _numlines ) {
  		row = _numlines-1;    // we count rows starting w/0
  	}
  	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
  }

  // Turn the display on/off (quickly)
  void noDisplay() {
  	_displaycontrol &= ~LCD_DISPLAYON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }
  void display() {
  	_displaycontrol |= LCD_DISPLAYON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }

  // Turns the underline cursor on/off
  void noCursor() {
  	_displaycontrol &= ~LCD_CURSORON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }
  void cursor() {
  	_displaycontrol |= LCD_CURSORON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }

  // Turn on and off the blinking cursor
  void noBlink() {
  	_displaycontrol &= ~LCD_BLINKON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }
  void blink() {
  	_displaycontrol |= LCD_BLINKON;
  	command(LCD_DISPLAYCONTROL | _displaycontrol);
  }

  // These commands scroll the display without changing the RAM
  void scrollDisplayLeft(void) {
  	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
  }
  void scrollDisplayRight(void) {
  	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
  }

  // This is for text that flows Left to Right
  void leftToRight(void) {
  	_displaymode |= LCD_ENTRYLEFT;
  	command(LCD_ENTRYMODESET | _displaymode);
  }

  // This is for text that flows Right to Left
  void rightToLeft(void) {
  	_displaymode &= ~LCD_ENTRYLEFT;
  	command(LCD_ENTRYMODESET | _displaymode);
  }

  // This will 'right justify' text from the cursor
  void autoscroll(void) {
  	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
  	command(LCD_ENTRYMODESET | _displaymode);
  }

  // This will 'left justify' text from the cursor
  void noAutoscroll(void) {
  	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  	command(LCD_ENTRYMODESET | _displaymode);
  }


  // Turn the (optional) backlight off/on
  void noBacklight(void) {
  	_backlightval=LCD_NOBACKLIGHT;
  	expanderWrite(0);
  }

  void backlight(void) {
  	_backlightval=LCD_BACKLIGHT;
  	expanderWrite(0);
  }



  /*********** mid level commands, for sending data/cmds */
  
  void print_string(uint8_t *str)
  {
	  while((*str) != '\0')
	  {
		  print_character(*str);
		  str++;
	  }
  }
  inline void print_character(uint8_t value)
  {
	  send(value,1);
  }

  inline void command(uint8_t value) 
  {
  	send(value, 0);
  }


  /************ low level data pushing commands **********/

  // write either command or data
  void send(uint8_t value, uint8_t mode) 
  {
  	uint8_t highnib=value&0xf0;
  	uint8_t lownib=(value<<4)&0xf0;
    write4bits((highnib)|mode);
  	write4bits((lownib)|mode); 
  }

  void write4bits(uint8_t value) 
  {
  	expanderWrite(value);
  	pulseEnable(value);
  }

  void pulseEnable(uint8_t _data)
  {
	uint8_t caliz;
	caliz = (_data | (En));
	delay_7u(10);
  	expanderWrite(caliz);	// En high
  	delay_7u(7);		// enable pulse must be >450ns
  	
  	caliz = (_data  & (~En));
  	delay_7u(10);
  	expanderWrite(caliz);	// En low
  	delay_7u(15);		// commands need > 37us to settle
  } 

  void expanderWrite(uint8_t _data)
  {                
	uint8_t caliz;
  	I2C_Start;
  	I2C0_D = _Addr<<1 | WRITE;
  	I2C_Wait;
  	
  	I2C_Start;
  	caliz = _data | _backlightval;
  	I2C0_D = caliz;
  	I2C_Wait;
  	I2C_Stop;
  }

  // Alias functions

  void cursor_on(){
  	cursor();
  }

  void cursor_off(){
  	noCursor();
  }

  void blink_on(){
  	blink();
  }

  void blink_off(){
  	noBlink();
  }


  void Init_ALL_LCD(void)
  {
		LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
		init();
		backlight();
		setCursor(4,0);
		print_string("BIENVENIDOS!");
		setCursor(6,2);
		print_string("LOCKER A");
		
		LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
		init();
		backlight();
		setCursor(4,0);
		print_string("BIENVENIDOS!");
		setCursor(6,2);
		print_string("LOCKER B");
		
		LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
		init();
		backlight();
		setCursor(4,0);
		print_string("BIENVENIDOS!");
		setCursor(6,2);
		print_string("LOCKER C");
  }
  
  void regresar_material(uint8_t address)
  {
		LCD_Init_I2C(address, 20,4);
		init();
		backlight();
		setCursor(7,0);
		print_string("RETIRE");
		setCursor(9,1);
		print_string("SU");
		setCursor(3,2);
		print_string("TARJETA / TAG");
  }
