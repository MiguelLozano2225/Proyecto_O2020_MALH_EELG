/*
 * UART.c
 *
 *  Created on: Oct 29, 2020
 *      Author: migue
 */
#include "UART.h"
#include "pcf8574.h"


unsigned char Array_Name[20];

unsigned char counterA = 0;
unsigned char counterB = 0;
unsigned char counterC = 0;
unsigned char Array_password_A[4];
unsigned char Array_password_B[4];
unsigned char Array_password_C[4];
unsigned char Array_password_AA[4];
unsigned char Array_password_AB[4];
unsigned char Array_password_AC[4];
unsigned char Array_password_BA[4];
unsigned char Array_password_BB[4];
unsigned char Array_password_BC[4];
unsigned char Array_password_CA[4];
unsigned char Array_password_CB[4];
unsigned char Array_password_CC[4];

enum {
	mStateIdle = 0,
	mStateLockerOpen,
	mStateLockerReturn,
	mStateLockerReadMaterialTag
} mLockerStates;

unsigned char StatusLockerA = mStateIdle;
unsigned char StatusLockerB = mStateIdle;
unsigned char StatusLockerC = mStateIdle;

unsigned char *Array_ptr = &Array_Name[0];

void Init_UART (void)  
{
	UART3_BDH = UART_BDH_SBR((SBR>>8) & 0x1F);
	UART3_BDL = (SBR & 0xFF);
	UART3_C4 = UART_C4_BRFA(BRFA);
	
	UART3_C1 = 0;
	UART3_C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK; 	
}

void UART_SendString (unsigned char* String, unsigned char Size)
{
	unsigned char i;
	
	for(i=0; i<Size; i++, String++)
	{
		UART3_D = *String;				
		while(!(UART3_S1 & UART_S1_TC_MASK));						//Wait for transmission complete flag
	}
	
}


void UART3_Status_IRQHandler(void)
{
	GPIOB_PDOR &= ~(1<<22);
	unsigned char i;
	
	if(UART3_S1 & UART_S1_RDRF_MASK)
	{
		*Array_ptr = UART3_D;
		
		if(*Array_ptr == '\n')
		{
			*Array_ptr = '\0';
			Array_ptr = &Array_Name[0];
			/*1.0 Condition*/
			if(Array_Name[0] == '1')
			{
				if(Array_Name[1] == 'A')
				{
					LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
					init();
					backlight();
					setCursor(0,0);
					print_string(&Array_Name[3]);
					StatusLockerA = mStateLockerOpen;
				}
				
				else if(Array_Name[1] == 'B')
				{
					LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
					init();
					backlight();
					setCursor(0,0);
					print_string(&Array_Name[3]);
					StatusLockerB = mStateLockerOpen;
				}
				
				else /*AQUI CAMBIO*/  
				{
					LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
					init();
					backlight();
					setCursor(0,0);
					print_string(&Array_Name[3]);
					StatusLockerC = mStateLockerOpen;
				}
			}
			/*2.0 Condition*/
			else if(Array_Name[0] == '2')
			{
				if(Array_Name[1] == 'A')
				{
					for(i=3; i<7;i++)
					{
						Array_password_A[i-3] = Array_Name[i]; 
					}
				}
				
				else if(Array_Name[1] == 'B')
				{
					for(i=3; i<7;i++)
					{
						Array_password_B[i-3] = Array_Name[i]; 
					}
				}
				
				else
				{
					for(i=3; i<7;i++)
					{
						Array_password_C[i-3] = Array_Name[i]; 
					}
				}
			}
			/*3.0 Condition*/
			else if (Array_Name[0] == '3') 
			{
				/*3.1 Condition*/
				if(Array_Name[1] == 'A')
				{
					counterA++;
					if(counterA>3) {counterA = 1;}
					
					if(Array_Name[2] == 'A')
					{
						for(i=4;i<8;i++)
						{
							Array_password_AA[i-4] = Array_Name[i];/*0x12, 0x50, 0x45, 0x53*/
						}
					}
					
					else if(Array_Name[2] == 'B')
					{
						for(i=4;i<8;i++)
						{
							Array_password_AB[i-4] = Array_Name[i];
						}
					}
					
					else if(Array_Name[2] == 'C') 
					{
						for(i=4;i<8;i++)
						{
							Array_password_AC[i-4] = Array_Name[i];
						}
					}
				}
				/*3.2 Condition*/
				else if(Array_Name[1] == 'B')
				{
					counterB++;
					if(counterB>3) {counterB = 1;}
					if(Array_Name[2] == 'A')
					{
						for(i=4;i<8;i++)
						{
							Array_password_BA[i-4] = Array_Name[i];
						}
					}
					
					else if(Array_Name[2] == 'B')
					{
						for(i=4;i<8;i++)
						{
							Array_password_BB[i-4] = Array_Name[i];
						}
					}
					
					else if(Array_Name[2] == 'C')
					{
						for(i=4;i<8;i++)
						{
							Array_password_BC[i-4] = Array_Name[i];
						}
					}
				}
				/*3.3 Condition*/
				else if(Array_Name[1] == 'C')
				{
					counterC++;
					if(counterC>3) {counterC = 1;}
					
					if(Array_Name[2] == 'A')
					{
						for(i=4;i<8;i++)
						{
							Array_password_CA[i-4] = Array_Name[i];
						}
					}
					
					else if(Array_Name[2] == 'B')
					{
						for(i=4;i<8;i++)
						{
							Array_password_CB[i-4] = Array_Name[i];
						}
					}
					else if(Array_Name[2] == 'C')
					{
						for(i=4;i<8;i++)
						{
							Array_password_CC[i-4] = Array_Name[i];
						}
					}
				}
			}
		}
		else
		{
			Array_ptr++;
		}
		
	}
}
