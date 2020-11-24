/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "clocks_and_pin.h"
#include "derivative.h" /* include peripheral declarations */
#include "i2c.h"
#include "MFRC522.h"
#include "NVIC.h"
#include "pcf8574.h"
#include "SPI.h"
#include "UART.h"

extern unsigned char counterA;
extern unsigned char counterB;
extern unsigned char counterC;
extern unsigned char Array_password_A[4];
extern unsigned char Array_password_B[4];
extern unsigned char Array_password_C[4];
extern unsigned char Array_password_AA[4];
extern unsigned char Array_password_AB[4];
extern unsigned char Array_password_AC[4];
extern unsigned char Array_password_BA[4];
extern unsigned char Array_password_BB[4];
extern unsigned char Array_password_BC[4];
extern unsigned char Array_password_CA[4];
extern unsigned char Array_password_CB[4];
extern unsigned char Array_password_CC[4];

extern unsigned char StatusLockerA;
extern unsigned char StatusLockerB;
extern unsigned char StatusLockerC;

enum {
	mStateIdle = 0,
	mStateLockerOpen,
	mStateLockerReturn,
	mStateLockerReadMaterialTag
} mLockerStates;

#define 	MIFARE_NAME_ADDR		(1)		//Number of the block to be written
#define 	MIFARE_REGISTER_ADDR	(2)		//Number of the block to be written
#define 	MIFARE_KEY_ADDR			(4)		//Number of the block to be written
#define 	MIFARE_TAG_MATERIAL		(5)		//Number of the block to be written

unsigned char CardNUID[5];			//Card identifier, do not modify the size of this buffer
unsigned char CardRead[18];			//Output buffer, first 16 bytes are the contains the data of the block read   

void delay(unsigned short int time_ms);
unsigned char Array_compare(unsigned char *Array1, unsigned char *Array2, unsigned char size);

int main(void)
{
	unsigned char cntA, cntB, cntC;
	unsigned char fns_LockerA[4] = {"4A\n"}; 
	unsigned char fns_LockerB[4] = {"4B\n"};
	unsigned char fns_LockerC[4] = {"4C\n"};
	Init_pin_clocks();
	SPI_Init();
	Enable_IRQ(INT_SPI0);
	Init_I2C();
	Init_UART();
	Enable_IRQ(INT_UART3_RX_TX);
	Init_ALL_LCD();  
	MFRC522_Init(RC522_0);
	MFRC522_Init(RC522_1);
	MFRC522_Init(RC522_2);
	
	for(;;)
	{	
		/******************************************* Locker A *************************************************/
		if(StatusLockerA == mStateLockerOpen)
		{
			if(MFRC522_Card_Request(RC522_0))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
				//GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_0);
				//Read MIFARE KEY ADDR and wait to remove the tag
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_0);
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_0);
				MFRC522_Card_Halt(RC522_0);
				delay(1000);
				while(MFRC522_Card_Request(RC522_0))
				{
					MFRC522_Card_Halt(RC522_0);
					delay(1000);
				}
				
				//If the user have the correct key, then open the door
				if(OK == Array_compare(&CardRead[0], &Array_password_A[0], 4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("LOCKER");
					setCursor(7,2);
					print_string("ABIERTO");
					OPEN_LOCKER_A;
					delay(3000);
					CLOSE_LOCKER_A;
					StatusLockerA = mStateLockerReturn;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}
		}
		else if(StatusLockerA == mStateLockerReturn)
		{
			if(MFRC522_Card_Request(RC522_0))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
//				GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_0);
				
				//Read MIFARE KEY ADDR and wait to remove the tag
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_0);
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_0);
				MFRC522_Card_Halt(RC522_0);
				delay(1000);
				while(MFRC522_Card_Request(RC522_0))
				{
					MFRC522_Card_Halt(RC522_0);
					delay(1000);
				}
				
				//If the user have the correct key, then open the door
				if(OK == Array_compare(&CardRead[0], &Array_password_A[0], 4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("REGRESAR");
					setCursor(7,2);
					print_string("MATERIAL");
					OPEN_LOCKER_A;
					delay(3000);
					CLOSE_LOCKER_A;
					StatusLockerA = mStateLockerReadMaterialTag;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}	
		}
		
		else if (StatusLockerA == mStateLockerReadMaterialTag)
		{
			cntA = counterA;
			while(cntA)
			{
				if(MFRC522_Card_Request(RC522_0))
				{
//					GPIOB_PDOR |= (1<<22);
					LED_W_OFF;
					regresar_material(I2C_LCD_ADDR_0);
					
					//Read MIFARE KEY ADDR and wait to remove the tag
					MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_0);
					MFRC522_Card_Read_Block(MIFARE_TAG_MATERIAL, &CardRead[0], &CardNUID[0],RC522_0);
					MFRC522_Card_Halt(RC522_0);
					delay(1000);
					while(MFRC522_Card_Request(RC522_0))
					{
						MFRC522_Card_Halt(RC522_0);
						delay(1000);
					}
					if(Array_compare(&CardRead[0], &Array_password_AA[0],4)== OK)
						{
							cntA--;
							LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
							init();
							backlight();
							setCursor(3,3);
							print_string("Restante ");
							setCursor(13,3);
							print_character(cntA+'0');
						}
					
					else if(Array_compare(&CardRead[0], &Array_password_AB[0],4)== OK)
					{
						cntA--;
						LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntA+'0');
					}
					
					else if(Array_compare(&CardRead[0], &Array_password_AC[0],4)== OK)
					{
						cntA--;
						LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntA+'0');
					}
				}
				delay(1000);
//				GPIOB_PDOR &= ~(1<<22);
				LED_W_ON;
			}
			LCD_Init_I2C(I2C_LCD_ADDR_0, 20,4);
			init();
			backlight();
			setCursor(4,0);
			print_string("BIENVENIDOS!");
			setCursor(6,2);
			print_string("LOCKER A");
			StatusLockerA = mStateIdle;
			UART_SendString(&fns_LockerA[0],3);
		}
		
		/******************************************************************************************************/
		
		/******************************************* Locker B *************************************************/		
		if (StatusLockerB == mStateLockerOpen)
		{
			if(MFRC522_Card_Request(RC522_1))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
//				GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_1);

				
				//Read MIFARE KEY ADDR and wait to remove the 
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_1);								
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_1);
				MFRC522_Card_Halt(RC522_1);
				delay(1000);
				while(MFRC522_Card_Request(RC522_1))
				{
					MFRC522_Card_Halt(RC522_1);
					delay(1000);
				}
				
				//If the user have the correct key, then open the door			
				if(OK == Array_compare(&CardRead[0],&Array_password_B[0],4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("LOCKER");
					setCursor(7,2);
					print_string("ABIERTO");
					OPEN_LOCKER_B;
					delay(3000);
					CLOSE_LOCKER_B;
					StatusLockerB = mStateLockerReturn;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}
		}
		else if (StatusLockerB == mStateLockerReturn)
		{
			if(MFRC522_Card_Request(RC522_1))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
//				GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_1);

				
				//Read MIFARE KEY ADDR and wait to remove the tag
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_1);								
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_1);
				MFRC522_Card_Halt(RC522_1);
				delay(1000);
				while(MFRC522_Card_Request(RC522_1))
				{
					MFRC522_Card_Halt(RC522_1);
					delay(1000);
				}
				
				//If the user have the correct key, then open the door			
				if(OK == Array_compare(&CardRead[0],&Array_password_B[0],4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("REGRESAR");
					setCursor(7,2);
					print_string("MATERIAL");
					OPEN_LOCKER_B;
					delay(3000);
					CLOSE_LOCKER_B;
					StatusLockerB = mStateLockerReadMaterialTag;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}			
		}
		
		else if (StatusLockerB == mStateLockerReadMaterialTag)
		{
			cntB = counterB;
			while(cntB)
			{
				if(MFRC522_Card_Request(RC522_1))
				{
//					GPIOB_PDOR |= (1<<22);
					LED_W_OFF;
					regresar_material(I2C_LCD_ADDR_1);
					
					//Read MIFARE KEY ADDR and wait to remove the tag
					MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_1);
					MFRC522_Card_Read_Block(MIFARE_TAG_MATERIAL, &CardRead[0], &CardNUID[0],RC522_1);
					MFRC522_Card_Halt(RC522_1);
					delay(1000);
					while(MFRC522_Card_Request(RC522_1))
					{
						MFRC522_Card_Halt(RC522_1);
						delay(1000);
					}
					if(Array_compare(&CardRead[0], &Array_password_BA[0],4)== OK)
						{
							cntB--;
							LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
							init();
							backlight();
							setCursor(3,3);
							print_string("Restante ");
							setCursor(13,3);
							print_character(cntB+'0');
						}
					
					else if(Array_compare(&CardRead[0], &Array_password_BB[0],4)== OK)
					{
						cntB--;
						LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntB+'0');
					}
					
					else if(Array_compare(&CardRead[0], &Array_password_BC[0],4)== OK)
					{
						cntB--;
						LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntB+'0');
					}
				}
				delay(1000);
//				GPIOB_PDOR &= ~(1<<22);
				LED_W_ON;
			}
			LCD_Init_I2C(I2C_LCD_ADDR_1, 20,4);
			init();
			backlight();
			setCursor(4,0);
			print_string("BIENVENIDOS!");
			setCursor(6,2);
			print_string("LOCKER B");
			StatusLockerB = mStateIdle;
			UART_SendString(&fns_LockerB[0],3);
		}
		/******************************************************************************************************/
		
		/******************************************* Locker C *************************************************/
		if (StatusLockerC == mStateLockerOpen)
		{
			if(MFRC522_Card_Request(RC522_2))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
//				GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_2);
				
				//Read MIFARE KEY ADDR and wait to remove the tag
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_2);	
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_2);
				MFRC522_Card_Halt(RC522_2);
				delay(1000);
				while(MFRC522_Card_Request(RC522_2))
				{
					MFRC522_Card_Halt(RC522_2);
					delay(1000);
				}
				
				if(OK == Array_compare(&CardRead[0],&Array_password_C[0],4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("LOCKER");
					setCursor(7,2);
					print_string("ABIERTO");
					OPEN_LOCKER_C;
					delay(3000);
					CLOSE_LOCKER_C;
					StatusLockerC = mStateLockerReturn;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}	 
		}
		else if (StatusLockerC == mStateLockerReturn)
		{
			if(MFRC522_Card_Request(RC522_2))		//If this function returns true, the MFRC522 module has detected a MIFARE card											
			{
//				GPIOB_PDOR |= (1<<22);
				LED_W_OFF;
				regresar_material(I2C_LCD_ADDR_2);

				
				//Read MIFARE KEY ADDR and wait to remove the tag
				MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_2);	
				MFRC522_Card_Read_Block(MIFARE_KEY_ADDR, &CardRead[0], &CardNUID[0],RC522_2);
				MFRC522_Card_Halt(RC522_2);
				delay(1000);
				while(MFRC522_Card_Request(RC522_2))
				{
					MFRC522_Card_Halt(RC522_2);
					delay(1000);
				}
				
				if(OK == Array_compare(&CardRead[0],&Array_password_C[0],4))
				{		
					LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("REGRESAR");
					setCursor(7,2);
					print_string("MATERIAL");
					OPEN_LOCKER_C;
					delay(3000);
					CLOSE_LOCKER_C;
					StatusLockerC = mStateLockerReadMaterialTag;
				}
				else
				{
					LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
					init();
					backlight();
					setCursor(7,0);
					print_string("ERROR!");
					setCursor(6,1);
					print_string("Usuario");
					setCursor(4,2);
					print_string("NO reconocido");
				}
			}				
		}
		/*FASDASDASD**/
		else if (StatusLockerC == mStateLockerReadMaterialTag)
		{
			cntC = counterC;
			while(cntC)
			{
				if(MFRC522_Card_Request(RC522_2))
				{
//					GPIOB_PDOR |= (1<<22);
					LED_W_OFF;
					regresar_material(I2C_LCD_ADDR_2);
					//Read MIFARE KEY ADDR and wait to remove the tag
					MFRC522_Card_AnticollSelect(&CardNUID[0],RC522_2);
					MFRC522_Card_Read_Block(MIFARE_TAG_MATERIAL, &CardRead[0], &CardNUID[0],RC522_2);
					MFRC522_Card_Halt(RC522_2);
					delay(1000);
					while(MFRC522_Card_Request(RC522_2))
					{
						MFRC522_Card_Halt(RC522_2);
						delay(1000);
					}
					if(Array_compare(&CardRead[0], &Array_password_CA[0],4)== OK)
						{
							cntC--;
							LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
							init();
							backlight();
							setCursor(3,3);
							print_string("Restante ");
							setCursor(13,3);
							print_character(cntC+'0');
						}
					
					else if(Array_compare(&CardRead[0], &Array_password_CB[0],4)== OK)
					{
						cntC--;
						LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntC + '0');
					}
					
					else if(Array_compare(&CardRead[0], &Array_password_CC[0],4)== OK)
					{
						cntC--;
						LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
						init();
						backlight();
						setCursor(3,3);
						print_string("Restante ");
						setCursor(13,3);
						print_character(cntC+'0');
					}
				}
				delay(1000);
//				GPIOB_PDOR &= ~(1<<22);
				LED_W_ON;
			}
			LCD_Init_I2C(I2C_LCD_ADDR_2, 20,4);
			init();
			backlight();
			setCursor(4,0);
			print_string("BIENVENIDOS!");
			setCursor(6,2);
			print_string("LOCKER C");
			StatusLockerC = mStateIdle;
			UART_SendString(&fns_LockerC[0],3);
		}
		
		/******************************************************************************************************/
		delay(1000);
//		GPIOB_PDOR &= ~(1<<22);
		LED_W_ON;
	}
	
	return 0;
}

s_rfid Array_compare(unsigned char *Array1,unsigned char *Array2,unsigned char size)
{
	unsigned char i;
	
	for(i=0; i<size;i++, Array1++, Array2++)
	{
		if(*Array1 != *Array2)
		{	return Error;	}
	}		
	
		return OK;
}

void delay(unsigned short int time_ms)
{
  LPTMR0_CSR = 0;
  LPTMR0_CSR &= ~LPTMR_CSR_TMS_MASK;
  LPTMR0_PSR = LPTMR_PSR_PBYP_MASK | LPTMR_PSR_PCS(1);
  LPTMR0_CMR = LPTMR_CMR_COMPARE(time_ms);
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
  
  while((LPTMR0_CSR & LPTMR_CSR_TCF_MASK) == 0);
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; 
}
