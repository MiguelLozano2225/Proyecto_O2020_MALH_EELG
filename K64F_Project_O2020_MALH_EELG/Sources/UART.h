/*
 * UART.h
 *
 *  Created on: Oct 29, 2020
 *      Author: migue
 */

#ifndef UART_H_
#define UART_H_
#include "derivative.h"

#define F_BUS      		20970000
#define BAUD_RATE	 	115200
#define SBR  			(F_BUS/(16*BAUD_RATE))
#define BRFA 			(32*((F_BUS/(16*BAUD_RATE)) - SBR))  


void Init_UART(void);
void UART_SendString (unsigned char* String, unsigned char Size);

#endif /* UART_H_ */
