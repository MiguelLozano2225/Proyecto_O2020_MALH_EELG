/*
 * MFRC522.c
 *
 *  Created on: Feb 7, 2018
 *      Author: Edgar
 */

#include "MFRC522.h"
#include "SPI.h"

/* MFRC522 driver functions */
void MFRC522_Write (unsigned char Address, unsigned char Data, unsigned char inst)
{
	if(inst == 0)
	{
		CS0_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_WRITE_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_WRITE_MASK | Address) as the input parameter
		mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(Data));						//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (Data) as the input parameter
		CS0_HIGH;
	}
	else if (inst == 1)
	{
		CS1_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_WRITE_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_WRITE_MASK | Address) as the input parameter
		mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(Data));						//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (Data) as the input parameter
		CS1_HIGH;
	}
	else
	{
		CS2_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_WRITE_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_WRITE_MASK | Address) as the input parameter
		mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(Data));						//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (Data) as the input parameter
		CS2_HIGH;
	}
}

unsigned char MFRC522_Read (unsigned char Address, unsigned char inst)
{
	unsigned char ByteReturn;
	
	if(inst == 0)
	{
		CS0_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_READ_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_READ_MASK | Address) as the input parameter
		ByteReturn = mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(RFID_NOP));	//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (RFID_NOP) as the input parameter
		CS0_HIGH;
	}
	
	else if(inst == 1)
	{
		CS1_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_READ_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_READ_MASK | Address) as the input parameter
		ByteReturn = mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(RFID_NOP));	//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (RFID_NOP) as the input parameter
		CS1_HIGH;
	}
	
	else if(inst == 2)
	{
		CS2_LOW;
		mSPIMstrTxRx(SPI_DATA_LOAD(RFID_READ_MASK | Address));		//Ignore SPI_DATA_LOAD macro, this is used in my SPI driver to set the CS_LOW(). In your function for the ATMega 8, you should pass only (RFID_READ_MASK | Address) as the input parameter
		ByteReturn = mSPIMstrTxRx(SPI_EOTF_DATA_LOAD(RFID_NOP));	//Ignore SPI_EOTF_DATA_LOAD macro, this is used in my SPI driver to set the CS_HIGH(). In your function for the ATMega 8, you should pass only (RFID_NOP) as the input parameter
		CS2_HIGH;
	}
	return ByteReturn;
}

/* MFRC522 functions*/
void MFRC522_Init (unsigned char inst)
{
	unsigned char Temp;

	MFRC522_Write(CommandReg, SoftReset, inst);								//It performs a soft reset to ensure an initial reset
	while((MFRC522_Read(CommandReg,inst) & 0xF) != Idle);					//Wait to soft reset command ends

	Temp = MFRC522_Read(TxASKReg,inst);
	MFRC522_Write(TxASKReg, ((Temp & (~0x40))|(0x40 & (0x1<<6))),inst);		//Force to 100% ASK modulation

	MFRC522_Write(TModeReg, 0x8D,inst);										//Configure internal timer to generate an interruption when the transmission starts
	MFRC522_Write(TPrescalerReg, 0x01,inst);									//Establish a period trigger of 25ms
	MFRC522_Write(TReloadReg_MSB, 0x00,inst);								//
	MFRC522_Write(TReloadReg_LSB, 0x10,inst);								//

	Temp = MFRC522_Read(ModeReg,inst);
	MFRC522_Write(ModeReg, ((Temp & (~0x3)) | (0x3 & (0x1<<0))),inst);		//Define 6363h as preset value for the CRC processor

	Temp = MFRC522_Read(TxControlReg,inst);
	MFRC522_Write(TxControlReg,((Temp & (~0x3)) | (0x3 & (0x3<<0))),inst);	//Turn on the antenna
}

unsigned char MFRC522_Card_Request (unsigned char inst)
{
	unsigned char Request_Buff[1] = {REQA}, TempBuff[2];
	return MFRC522_Transference(Transceive, &Request_Buff[0], 1, &TempBuff[0],inst);
}

void MFRC522_Card_AnticollSelect (unsigned char *NUID,unsigned char inst)
{
	unsigned char Anticollision_Buff[2] = Anticoll_CL1, Select_Buff[9] = Select_CL1, TempBuff[3];
	unsigned char index;

	MFRC522_Transference(Transceive, &Anticollision_Buff[0], 2, NUID,inst);

	for(index = 2; index < 7; index++, NUID++)
	{
		Select_Buff[index] = *NUID;
	}

	MFRC522_CRC(&Select_Buff[0], 7,inst);
	MFRC522_Transference(Transceive, &Select_Buff[0], 9, &TempBuff[3],inst);
}

void MFRC522_Card_Read_Block (unsigned char Address, unsigned char* Data, unsigned char *NUID,unsigned char inst)
{
	unsigned char Authentication_Buff[12] = {Authent_KeyA, Address, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, Read_Buff[4] = {MIFARE_Read, Address}, TempBuff[2];
	unsigned char index;

	for(index = 8; index < 12; index++, NUID++)
	{
		Authentication_Buff[index] = *NUID;
	}

	MFRC522_Transference(MFAuthent, &Authentication_Buff[0], 12, &TempBuff[0],inst);
	MFRC522_CRC(&Read_Buff[0], 2,inst);
	MFRC522_Transference(Transceive, &Read_Buff[0], 4, Data,inst);
}

void MFRC522_Card_Write_Block (unsigned char Address, unsigned char* Data, unsigned char DataSize, unsigned char *NUID,unsigned char inst)
{
	unsigned char Authentication_Buff[12] = {Authent_KeyA, Address, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, Write_Buff[4] = {MIFARE_Write, Address}, TempBuff[2], WriteData_Buff[18];
	unsigned char index;

	for (index = 0; index < 18; index++)
	{
		if(index < DataSize)
		{
			WriteData_Buff[index] = *Data++;
		}
		else
		{
			WriteData_Buff[index] = 0;
		}
	}

	for(index = 8; index < 12; index++, NUID++)
	{
		Authentication_Buff[index] = *NUID;
	}

	MFRC522_Transference(MFAuthent, &Authentication_Buff[0], 12, &TempBuff[0],inst);
	MFRC522_CRC(&Write_Buff[0], 2,inst);
	MFRC522_Transference(Transceive, &Write_Buff[0], 4, &TempBuff[0],inst);
	MFRC522_CRC(&WriteData_Buff[0], 16,inst);
	MFRC522_Transference(Transceive, &WriteData_Buff[0], 18, &TempBuff[0],inst);
}

void MFRC522_Card_Halt (unsigned char inst)
{
	unsigned char Halt_Buff[4] = Halt;
	unsigned char Temp;

	Temp = MFRC522_Read(Status2Reg,inst);
	MFRC522_Write(Status2Reg, (Temp & ~0x8),inst);
	MFRC522_CRC(&Halt_Buff[0], 2,inst);
	MFRC522_Transference(Transmit, &Halt_Buff[0], 4, NULL,inst);
}

void MFRC522_CRC (unsigned char *Data_ptr, unsigned char Length, unsigned char inst)
{
	unsigned char index;

	MFRC522_Write(CommandReg, Idle,inst);														//Ensure an initial state of the module
	MFRC522_Write(ComIrqReg, (unsigned char)(~0x80),inst);												//Clear all interrupt requests
	MFRC522_Write(FIFOLevelReg, (0x1<<7),inst);													//Flush FIFO

	for(index = 0; index < Length; index++, Data_ptr++)										//Send data to FIFO
	{
		MFRC522_Write(FIFODataReg, *Data_ptr,inst);
	}

	MFRC522_Write(CommandReg, CalcCRC,inst);														//Execute "calculate CRC command"

	while(!(MFRC522_Read(DivIrqReg,inst) & 0x04));												//Wait CRC ends

	*(Data_ptr++) = MFRC522_Read(CRCResultReg_LSB,inst);											//Add CRC result to data pointer
	*(Data_ptr++) = MFRC522_Read(CRCResultReg_MSB,inst);
}

unsigned char MFRC522_Transference (unsigned char Command, unsigned char *Data_ptr, unsigned char Length, unsigned char *Request_ptr, unsigned char inst)
{
	unsigned char Temp, index, Frame_Size;

	MFRC522_Write(CommandReg, Idle,inst);														//Ensure an initial state of the module
	MFRC522_Write(ComIrqReg, (unsigned char)(~0x80),inst);												//Clear all interrupt requests
	MFRC522_Write(FIFOLevelReg, (0x1<<7),inst);													//Flush FIFO

	Frame_Size = (*Data_ptr == WUPA || *Data_ptr == REQA)? RFID_FRSZ_7 : RFID_FRSZ_8;		//Select frame size

	for(index=0; index < Length; index++, Data_ptr++)
	{
		MFRC522_Write(FIFODataReg, *Data_ptr,inst);
	}

	Temp = MFRC522_Read(BitFramingReg,inst);
	MFRC522_Write(BitFramingReg, ((Temp & ~0x7)| Frame_Size),inst);
	MFRC522_Write(CommandReg, Command,inst);

	if(Command == Transceive)
	{
		Temp = MFRC522_Read(BitFramingReg,inst);
		MFRC522_Write(BitFramingReg, (Temp | RFID_SEND),inst);

		while(!(MFRC522_Read(ComIrqReg,inst) & 0x21));

		Temp = MFRC522_Read(BitFramingReg,inst);
		MFRC522_Write(BitFramingReg, (Temp & ~RFID_SEND),inst);

		Temp = MFRC522_Read(FIFOLevelReg,inst);									//Read the number of bytes stored in FIFO

		for(index=0; index<Temp; index++, Request_ptr++)					//Store the received bytes in external buffer
		{
			*Request_ptr = MFRC522_Read(FIFODataReg,inst);
		}

		if(MFRC522_Read(ComIrqReg, inst) & 0x01)
		{
			return 0;
		}
		else if(MFRC522_Read(ComIrqReg, inst) & 0x20)
		{
			return 1;
		}
	}

	else if(Command == MFAuthent)
	{
		while(!(MFRC522_Read(Status2Reg, inst) & 0x08));
	}

	else if(Command == Transmit)
	{
		while(!(MFRC522_Read(ComIrqReg, inst) & 0x40));
	}

	return 0;
}

