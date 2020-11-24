/*
 * i2c.c
 *
 *  Created on: Oct 23, 2019
 *      Author: migue
 */
#include "derivative.h"
#include "i2c.h"
#define WRITE 0
#define READ  1


void Init_I2C(void)
{
	I2C0_C1 &= ~I2C_C1_IICEN_MASK; //Disable the I2C0		
	I2C0_F |= (I2C_F_MULT(2)) | (I2C_F_ICR(1));
	I2C0_C1 |= (I2C_C1_MST_MASK) //Enable the master mode 
			| (I2C_C1_TX_MASK);  //Enable the transmit mode
	
	I2C0_C1 |= I2C_C1_IICEN_MASK; //Enable the I2C	
}

void Transmit_data(uint8_t address)
{
	uint8_t counter = 16;

	
	while(counter)
	{
		I2C_Start;
		I2C0_D = address | WRITE;
		I2C_Wait;
		address++;
		counter--;
		I2C_Stop;
	}
	
}

void Read_data(uint8_t address,uint16_t mem_add2, uint8_t *rx_data)
{
	uint8_t counter=64;
	
	I2C_Start;
	I2C0_D = (address<<1) | READ;
	I2C_Wait;
	
	I2C_EnterRxMode;
	I2C_EnableAck;
	
	I2C0_S |= I2C_S_SRW_MASK;
	
	while(counter--)
	{
		*rx_data = I2C0_D;
    	rx_data++;
	    I2C_Wait;
	}
	I2C_DisableAck;
	I2C_Stop;
}


void delay_7u(uint16_t a)
{
	a*= 10;
	while(a--);
}
