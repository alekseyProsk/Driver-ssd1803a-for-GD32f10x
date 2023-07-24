#include "gd32f10x_i2c.h"
#include "LCD.h"
#include <stdio.h>
#include <time.h>
#include <stddef.h>


void LCD_begin()
{
	LCD_i2cConfig();

	LCD_i2cInit();
	
	LCD_home();
	
	LCD_i2c_send_command(COMMAND_ENTRY_MODE_SET | ENTRY_MODE_LEFT_TO_RIGHT, LCD_DISPLAY_ADDRESS);
}

void LCD_i2c_send_command(uint32_t cmd, uint32_t addr)
{
		while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
		
	
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)); 
	
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, addr << 1 , I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
		
		uint32_t k = 0;
		while (!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) 
		{
        if (i2c_flag_get(I2C1, I2C_FLAG_AERR)) 
				{
            i2c_flag_clear(I2C1, I2C_FLAG_AERR);
            i2c_stop_on_bus(I2C1);
            return;
        }
        // in case no bus is connected
        if (k++ > 1000 * 1000)
				{
            i2c_stop_on_bus(I2C1);
            return;
        }
    };
		i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);////must be
		while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));//
		
		i2c_ack_config(I2C1, I2C_ACK_ENABLE);
		
    i2c_data_transmit(I2C1, 0x00);
        /* wait until the TBE bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
		
		i2c_data_transmit(I2C1, cmd);
		
		//while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
		i2c_ack_config(I2C1, I2C_ACK_DISABLE);
		
		i2c_stop_on_bus(I2C1);
    /* wait until stop condition generate */ 
	
		while (I2C_CTL0(I2C1) & I2C_CTL0_STOP);  
		
		//delay_1ms(250);
		
		
		
}

void LCD_home()
{
	LCD_i2c_send_command(COMMAND_RETURN_HOME,LCD_DISPLAY_ADDRESS);
}


void LCD_i2cInit()
{

	
	LCD_i2c_send_command(COMMAND_CLEAR_DISPLAY, LCD_DISPLAY_ADDRESS); //Clear display
	//RE = 1  |  IS = 0
	LCD_i2c_send_command(COMMAND_8BIT_4LINES_NORMAL_RE1_IS0, LCD_DISPLAY_ADDRESS);   //Function set : 8bit 4line RE = 1, IS = 0, Not reverse
	//LCD_i2c_send_command(COMMAND_8BIT_4LINES_REVERSE_RE1_IS0, LCD_DISPLAY_ADDRESS); //Function set : RE = 1, REVERSE
	LCD_i2c_send_command(COMMAND_BS1_1, LCD_DISPLAY_ADDRESS);  //Set BS1 = 1 (1/6 bias)
	LCD_i2c_send_command(COMMAND_POWER_DOWN_DISABLE, LCD_DISPLAY_ADDRESS); //Power down disable
	LCD_i2c_send_command(COMMAND_SEGMENT_NORMAL_WAY, LCD_DISPLAY_ADDRESS); // Segment bidirection : in the right way
	LCD_i2c_send_command(COMMAND_NW, LCD_DISPLAY_ADDRESS); //NW = 1

	


	//RE = 0  |  IS = 1
		LCD_i2c_send_command(COMMAND_8BIT_4LINES_RE0_IS1, LCD_DISPLAY_ADDRESS);  //Function set : RE = 0, IS = 1
		LCD_i2c_send_command(COMMAND_DISPLAY_ON_CURSOR_OFF_BLINK_OFF, LCD_DISPLAY_ADDRESS);  //Display ON (without cursor...)
		LCD_i2c_send_command(COMMAND_BS0_1, LCD_DISPLAY_ADDRESS);  //Set BS0 = 1 (1/6 bias) / Internal frequency..
		LCD_i2c_send_command(COMMAND_INTERNAL_DIVIDER_680, LCD_DISPLAY_ADDRESS); //Internal divider
		LCD_i2c_send_command(COMMAND_CONTRAST, LCD_DISPLAY_ADDRESS); //Constrast
		LCD_i2c_send_command(COMMAND_POWER_ICON_CONTRAST, LCD_DISPLAY_ADDRESS);  //Power Icon control contrast
		LCD_i2c_send_command(COMMAND_FOLLOWER_CONTROL, LCD_DISPLAY_ADDRESS); //Follower Control
		
	//RE = 0  | IS = 0
	//LCD_i2c_send_command(COMMAND_8BIT_4LINES_RE0_IS0, LCD_DISPLAY_ADDRESS);
}

void LCD_SetPos(uint8_t x, uint8_t y)
{
	switch(y)
	{
		case 0:
			LCD_i2c_send_command(x|COMMAND_ADDRESS_DDRAM, LCD_DISPLAY_ADDRESS);
			delay_1ms(1);
			break;
		case 1:
			LCD_i2c_send_command((0x20+x)|COMMAND_ADDRESS_DDRAM, LCD_DISPLAY_ADDRESS);
			delay_1ms(1);
			break;
		case 2:
			LCD_i2c_send_command((0x41+x)|COMMAND_ADDRESS_DDRAM, LCD_DISPLAY_ADDRESS);
			delay_1ms(1);
			break;
		case 3:
			LCD_i2c_send_command((0x61+x)|COMMAND_ADDRESS_DDRAM, LCD_DISPLAY_ADDRESS);
			delay_1ms(1);
			break;
	}
}

void LCD_setContrast(uint8_t contrast)
{
	LCD_i2c_send_command(COMMAND_8BIT_4LINES_RE0_IS1, LCD_DISPLAY_ADDRESS );  //Function set : RE = 0, IS = 1
	LCD_i2c_send_command((0x70 | (contrast & 0x0F)), LCD_DISPLAY_ADDRESS);     //Contrast
	LCD_i2c_send_command((COMMAND_POWER_ICON_CONTRAST | ((contrast >> 4) & 0x03)), LCD_DISPLAY_ADDRESS); //Power Icon control contrast
	LCD_i2c_send_command(COMMAND_8BIT_4LINES_RE0_IS0, LCD_DISPLAY_ADDRESS);  //Function set : RE = 0, IS = 0
}
// Function for write array byte

void LCD_i2c_writeByte( uint8_t pdata)
{

    /* wait until I2C bus is idle */ 
    while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
	
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)); 
	
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, LCD_DISPLAY_ADDRESS << 1, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */


		uint32_t k = 0;
		while (!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) 
		{
        if (i2c_flag_get(I2C1, I2C_FLAG_AERR)) 
				{
            i2c_flag_clear(I2C1, I2C_FLAG_AERR);
            i2c_stop_on_bus(I2C1);
            return;
        }
        // in case no bus is connected
        if (k++ > 1000 * 1000)
				{
            i2c_stop_on_bus(I2C1);
            return;
        }
    };
		
		i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);/////must be
		while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));///////
		
		 i2c_ack_config(I2C1, I2C_ACK_ENABLE);
		
    i2c_data_transmit(I2C1, 0x40);
        /* wait until the TBE bit is set */
    //while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
		
		i2c_data_transmit(I2C1, pdata);
			
		while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
			
   
    /* send a stop condition to I2C bus */
		i2c_ack_config(I2C1, I2C_ACK_DISABLE);
		
    i2c_stop_on_bus(I2C1);
    /* wait until stop condition generate */ 

		while(I2C_CTL0(I2C1) & I2C_CTL0_STOP);  
		

}



// Write byte in CGRAM display
//
void LCD_CGRAM_writeByte( uint8_t pdata, uint8_t location)
{

    // wait until I2C bus is idle */ 
    while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
	
    // send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
	
    // wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND)); 
	
    // send slave address to I2C bus */
    i2c_master_addressing(I2C1, LCD_DISPLAY_ADDRESS << 1, I2C_TRANSMITTER);
    // wait until ADDSEND bit is set */


	uint32_t k = 0;
		while (!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)) 
		{
        if (i2c_flag_get(I2C1, I2C_FLAG_AERR)) 
				{
            i2c_flag_clear(I2C1, I2C_FLAG_AERR);
            i2c_stop_on_bus(I2C1);
            return;
        }
        // in case no bus is connected
        if (k++ > 1000 * 1000)
				{
            i2c_stop_on_bus(I2C1);
            return;
        }
    };
		
		 i2c_ack_config(I2C1, I2C_ACK_ENABLE);
		
    i2c_data_transmit(I2C1, location);
        /* wait until the TBE bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
		
		i2c_data_transmit(I2C1, pdata);
			
		while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
			
    /* send a stop condition to I2C bus */
		
		i2c_ack_config(I2C1, I2C_ACK_DISABLE);

    i2c_stop_on_bus(I2C1);
    /* wait until stop condition generate */ 

		while(I2C_CTL0(I2C1) & I2C_CTL0_STOP);  
		
}



//set display backlight
void LCD_setBacklight(bool onOf)
{
	//for display Led
	if(onOf == TRUE)
	{
		
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_9);
	gpio_bit_set(GPIOC, GPIO_PIN_9);
	}
	else
	{
		gpio_bit_reset(GPIOC, GPIO_PIN_9);
	}
}
//Function for send string array
//
void LCD_i2c_sendString(char* str)
{
	uint8_t i=0;

  while(str[i]!=0)
  {

		LCD_i2c_writeByte(str[i]);
		i++;

	}
}

//Function for send data array
//
void LCD_i2c_sendData(uint8_t data[])
{
	uint8_t i=0;
	 while(i!= sizeof(data))
  {

		LCD_i2c_writeByte(data[i]);
		i++;

	}
	
}
//Function for create Custom symbol in CGRAM memory display
//
void LCD_createChar(uint8_t location, uint8_t charmap[])
{

	LCD_i2c_send_command(COMMAND_8BIT_4LINES_RE0_IS0, LCD_DISPLAY_ADDRESS);//!!!Very important RE must be 0, IS must be 0!!!!

	location &= 0x7;
	LCD_i2c_send_command(COMMAND_ADDRESS_CGRAM | (location << 3), LCD_DISPLAY_ADDRESS);

	for(uint8_t i = 0; i < 8; i++)
			LCD_i2c_writeByte(charmap[i]);
			

}

//Function for start i2c config
void LCD_i2cConfig(void)
{
		i2c_deinit(I2C1);
		i2c_clock_config(I2C1, 400000, I2C_DTCY_2);
		i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_10BITS, LCD_DISPLAY_ADDRESS);
    i2c_enable(I2C1);
		i2c_ack_config(I2C1, I2C_ACK_DISABLE);
}

