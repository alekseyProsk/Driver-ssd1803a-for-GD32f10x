#ifndef LCD_H
#define LCD_H

#include "gd32f10x.h"

#define LCD_DISPLAY_ADDRESS										0x3C//////SET YOUR ADRESS FOR DISPLAY


#define COMMAND_CLEAR_DISPLAY									0x01
#define COMMAND_RETURN_HOME										0x02
#define COMMAND_ENTRY_MODE_SET								0x04
	#define ENTRY_MODE_LEFT_TO_RIGHT              0x02
	#define ENTRY_MODE_SHIFT_INCREMENT						0x01
#define COMMAND_CURSORSHIFT										0x10
	#define CURSORSHIFT_MOVERIGHT									0x04
	#define CURSORSHIFT_MOVELEFT									0x00

#define COMMAND_ADDRESS_DDRAM									0x80
#define COMMAND_ADDRESS_CGRAM                 0x40

#define COMMAND_8BIT_4LINES_NORMAL_RE1_IS0			0x3A  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_REVERSE_RE1_IS0			0x3B  //Extended command access RE = 1
#define COMMAND_8BIT_4LINES_RE0_IS1							0x39  //Extended command access IS = 1
#define COMMAND_8BIT_4LINES_RE0_IS0							0x38  //Normal mode...


//DATAROM SELECTION
#define COMMAND_WRITING_DATA_ROM_SELECTION 			0x72

#define	COMMAND_CHANGE_ROM_C										0x208
#define COMMAND_CHANGE_ROM_A										0x200
#define COMMAND_CHANGE_ROM_B										0x204


//Command from extended set (RE = 1, IS = 0)
#define COMMAND_BS1_1												0x1E
#define COMMAND_POWER_DOWN_DISABLE					0x02
#define COMMAND_POWER_DOWN_ENABLE						0x03
#define COMMAND_SEGMENT_NORMAL_WAY					0x05
#define COMMAND_NW													0x09
#define COMMAND_TWO_STRING_MODE							0x1B



//Command from extended set (RE = 0, IS = 1)
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_ON			0x0F
#define COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_OFF		0x0E
#define COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF		0x0C
#define COMMAND_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF	0x08
#define COMMAND_BS0_1															0x1C
#define COMMAND_INTERNAL_DIVIDER_540									0x13
#define COMMAND_INTERNAL_DIVIDER_680									0x17
#define COMMAND_CONTRAST													0x77
#define COMMAND_POWER_ICON_CONTRAST								0x5C
#define COMMAND_FOLLOWER_CONTROL									0x6E


void LCD_begin(void);

void LCD_i2c_send_command(uint32_t cmd, uint32_t addr); // send command for Display

void i2c_write(uint8_t pdata, uint32_t x, uint32_t y);// write byte int adress

void LCD_i2c_writeByte(uint8_t pdata);//write byte to start display

void LCD_CGRAM_writeByte( uint8_t pdata, uint8_t location); //Write Byte in address CGRAM

void LCD_setBacklight(bool onOF);

void LCD_home(void);// return home

void LCD_SetPos(uint8_t x, uint8_t y); //set position cursor

void LCD_setContrast(uint8_t contrast); //Set contrast display

void LCD_i2c_sendData(uint8_t data[]);//send uint8 array

void LCD_i2c_sendString(char* str);//send string

void LCD_i2cInit(void);//initialization Display

void LCD_i2cConfig(void);//config i2c for LCD

void LCD_createChar(uint8_t location, uint8_t charmap[]);//create custom symbol;

#endif
