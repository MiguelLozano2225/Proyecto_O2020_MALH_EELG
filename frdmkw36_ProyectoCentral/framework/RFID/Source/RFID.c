/*
 * RFID.c
 *
 *  Created on: 30 jul. 2020
 *      Author: Edgar
 */


#include "RFID.h"
#include "fsl_os_abstraction.h"
#include "fsl_dspi.h"
#include "TimersManager.h"

/* SPI Data formats */
dspi_command_data_config_t dspi_command_data_byte =
{
	.isPcsContinuous = true,
	.whichCtar = kDSPI_Ctar0,
	.whichPcs= kDSPI_Pcs0,
	.isEndOfQueue = false,
	.clearTransferCount = false
};

dspi_command_data_config_t dspi_command_data_lastbyte =
{
	.isPcsContinuous = false,
	.whichCtar = kDSPI_Ctar0,
	.whichPcs= kDSPI_Pcs0,
	.isEndOfQueue = false,
	.clearTransferCount = false
};


/* Application variables */
typedef enum rfid_state_tag
{
	RFID_ReadUserNameAndRegister = 0,
	RFID_WriteUserNameAndRegister,
	RFID_WriteLockerUserKey,
	RFID_WriteLockerMaterialKey,
	RFID_RemoveCard
} rfid_state_t;

uint8_t RFID_State = (uint8_t)RFID_ReadUserNameAndRegister;

extern tmrTimerID_t mRfidUartTimerId;
extern void RfidUartTimerCallback (void * pParam);
extern bool_t isKeyAvailable;
extern uint8_t mRFIDName[18];			//16 bytes
extern uint8_t mRFIDRegister[18];		//8 bytes
extern uint8_t mLockerKey[4];
extern uint8_t mMaterialKey[4];


/* MFRC522 SPI Tx-Rx driver function */
uint8_t mSPIMstrTxRx (uint8_t Byte, dspi_command_data_config_t *command)
{
	uint8_t mSPIRxData;

	DSPI_MasterWriteData(SPI1, command, Byte);
	while(!(kDSPI_RxFifoDrainRequestFlag & DSPI_GetStatusFlags(SPI1)));
	mSPIRxData = (uint8_t)DSPI_ReadData(SPI1);
	DSPI_ClearStatusFlags(SPI1, kDSPI_RxFifoDrainRequestFlag);

	return mSPIRxData;
}

/* MFRC522 driver functions */
void MFRC522_Write (uint8_t Address, uint8_t Data)
{
	mSPIMstrTxRx((RFID_WRITE_MASK | Address), &dspi_command_data_byte);
	mSPIMstrTxRx(Data, &dspi_command_data_lastbyte);
}

uint8_t MFRC522_Read (uint8_t Address)
{
	mSPIMstrTxRx((RFID_READ_MASK | Address), &dspi_command_data_byte);
	return mSPIMstrTxRx(RFID_NOP, &dspi_command_data_lastbyte);
}

/* MFRC522 functions*/
void MFRC522_Init (void)
{
	uint8_t Temp;

	MFRC522_Write(CommandReg, SoftReset);								//It performs a soft reset to ensure an initial reset
	while((MFRC522_Read(CommandReg) & 0xF) != Idle);					//Wait to soft reset command ends

	Temp = MFRC522_Read(TxASKReg);
	MFRC522_Write(TxASKReg, ((Temp & (~0x40))|(0x40 & (0x1<<6))));		//Force to 100% ASK modulation

	MFRC522_Write(TModeReg, 0x8D);										//Configure internal timer to generate an interruption when the transmission starts
	MFRC522_Write(TPrescalerReg, 0x01);									//Establish a period trigger of 25ms
	MFRC522_Write(TReloadReg_MSB, 0x00);								//
	MFRC522_Write(TReloadReg_LSB, 0x10);								//

	Temp = MFRC522_Read(ModeReg);
	MFRC522_Write(ModeReg, ((Temp & (~0x3)) | (0x3 & (0x1<<0))));		//Define 6363h as preset value for the CRC processor

	Temp = MFRC522_Read(TxControlReg);
	MFRC522_Write(TxControlReg,((Temp & (~0x3)) | (0x3 & (0x3<<0))));	//Turn on the antenna
}

bool_t MFRC522_Card_Request (void)
{
	uint8_t Request_Buff[1] = {REQA}, TempBuff[2];
	return MFRC522_Transference(Transceive, &Request_Buff[0], 1, &TempBuff[0]);
}

void MFRC522_Card_AnticollSelect (uint8_t *NUID)
{
	uint8_t Anticollision_Buff[2] = Anticoll_CL1, Select_Buff[9] = Select_CL1, TempBuff[3];
	uint8_t index;

	MFRC522_Transference(Transceive, &Anticollision_Buff[0], 2, NUID);

	for(index = 2; index < 7; index++, NUID++)
	{
		Select_Buff[index] = *NUID;
	}

	MFRC522_CRC(&Select_Buff[0], 7);
	MFRC522_Transference(Transceive, &Select_Buff[0], 9, &TempBuff[3]);
}

void MFRC522_Card_Read_Block (uint8_t Address, uint8_t* Data, uint8_t *NUID)
{
	uint8_t Authentication_Buff[12] = {Authent_KeyA, Address, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, Read_Buff[4] = {MIFARE_Read, Address}, TempBuff[2];
	uint8_t index;

	for(index = 8; index < 12; index++, NUID++)
	{
		Authentication_Buff[index] = *NUID;
	}

	MFRC522_Transference(MFAuthent, &Authentication_Buff[0], 12, &TempBuff[0]);
	MFRC522_CRC(&Read_Buff[0], 2);
	MFRC522_Transference(Transceive, &Read_Buff[0], 4, Data);
}

void MFRC522_Card_Write_Block (uint8_t Address, uint8_t* Data, uint8_t DataSize, uint8_t *NUID)
{
	uint8_t Authentication_Buff[12] = {Authent_KeyA, Address, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, Write_Buff[4] = {MIFARE_Write, Address}, TempBuff[2], WriteData_Buff[18];
	uint8_t index;

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

	MFRC522_Transference(MFAuthent, &Authentication_Buff[0], 12, &TempBuff[0]);
	MFRC522_CRC(&Write_Buff[0], 2);
	MFRC522_Transference(Transceive, &Write_Buff[0], 4, &TempBuff[0]);
	MFRC522_CRC(&WriteData_Buff[0], 16);
	MFRC522_Transference(Transceive, &WriteData_Buff[0], 18, &TempBuff[0]);
}

void MFRC522_Card_Halt (void)
{
	uint8_t Halt_Buff[4] = Halt;
	uint8_t Temp;

	Temp = MFRC522_Read(Status2Reg);
	MFRC522_Write(Status2Reg, (Temp & ~0x8));
	MFRC522_CRC(&Halt_Buff[0], 2);
	MFRC522_Transference(Transmit, &Halt_Buff[0], 4, NULL);
}

void MFRC522_CRC (uint8_t *Data_ptr, uint8_t Length)
{
	uint8_t index;

	MFRC522_Write(CommandReg, Idle);														//Ensure an initial state of the module
	MFRC522_Write(ComIrqReg, (uint8_t)(~0x80));												//Clear all interrupt requests
	MFRC522_Write(FIFOLevelReg, (0x1<<7));													//Flush FIFO

	for(index = 0; index < Length; index++, Data_ptr++)										//Send data to FIFO
	{
		MFRC522_Write(FIFODataReg, *Data_ptr);
	}

	MFRC522_Write(CommandReg, CalcCRC);														//Execute "calculate CRC command"

	while(!(MFRC522_Read(DivIrqReg) & 0x04));												//Wait CRC ends

	*(Data_ptr++) = MFRC522_Read(CRCResultReg_LSB);											//Add CRC result to data pointer
	*(Data_ptr++) = MFRC522_Read(CRCResultReg_MSB);
}

bool_t MFRC522_Transference (uint8_t Command, uint8_t *Data_ptr, uint8_t Length, uint8_t *Request_ptr)
{
	uint8_t Temp, index, Frame_Size;

	MFRC522_Write(CommandReg, Idle);														//Ensure an initial state of the module
	MFRC522_Write(ComIrqReg, (uint8_t)(~0x80));												//Clear all interrupt requests
	MFRC522_Write(FIFOLevelReg, (0x1<<7));													//Flush FIFO

	Frame_Size = (*Data_ptr == WUPA || *Data_ptr == REQA)? RFID_FRSZ_7 : RFID_FRSZ_8;		//Select frame size

	for(index=0; index < Length; index++, Data_ptr++)
	{
		MFRC522_Write(FIFODataReg, *Data_ptr);
	}

	Temp = MFRC522_Read(BitFramingReg);
	MFRC522_Write(BitFramingReg, ((Temp & ~0x7)| Frame_Size));
	MFRC522_Write(CommandReg, Command);

	if(Command == Transceive)
	{
		Temp = MFRC522_Read(BitFramingReg);
		MFRC522_Write(BitFramingReg, (Temp | RFID_SEND));

		while(!(MFRC522_Read(ComIrqReg) & 0x21));

		Temp = MFRC522_Read(BitFramingReg);
		MFRC522_Write(BitFramingReg, (Temp & ~RFID_SEND));

		Temp = MFRC522_Read(FIFOLevelReg);									//Read the number of bytes stored in FIFO

		for(index=0; index<Temp; index++, Request_ptr++)					//Store the received bytes in external buffer
		{
			*Request_ptr = MFRC522_Read(FIFODataReg);
		}

		if(MFRC522_Read(ComIrqReg) & 0x01)
		{
			return false;
		}
		else if(MFRC522_Read(ComIrqReg) & 0x20)
		{
			return true;
		}
	}

	else if(Command == MFAuthent)
	{
		while(!(MFRC522_Read(Status2Reg) & 0x08));
	}

	else if(Command == Transmit)
	{
		while(!(MFRC522_Read(ComIrqReg) & 0x40));
	}

	return false;
}

/* RFID task definitions */
void RFID_Task (osaTaskParam_t param);
OSA_TASK_DEFINE( RFID_Task, gRFIDTaskPriority_c, 1, gRFIDTaskStackSize_c, FALSE );

void RFID_Init (void)
{
	osaTaskId_t RFIDThreadId;
	dspi_master_config_t masterConfig;

    /* Master configuration */
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = 10000000U;
    masterConfig.ctarConfig.bitsPerFrame = 8U;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / 10000000U;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / 10000000U;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / 10000000U;
    masterConfig.whichPcs = kDSPI_Pcs0;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    masterConfig.enableContinuousSCK = false;
    masterConfig.enableRxFifoOverWrite = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    /* Init pins, SPI, and MFRC522_Init */
    BOARD_InitSPI();
    DSPI_MasterInit(SPI1, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
    MFRC522_Init();

    /* Create RFID task */
	RFIDThreadId = OSA_TaskCreate(OSA_TASK(RFID_Task), NULL);

	if(NULL == RFIDThreadId)
	{
		while(1);
	}
}


void RFID_Task (osaTaskParam_t param)
{

	uint8_t CardNUID[5];

	while (1)
	{
		if (RFID_ReadUserNameAndRegister == RFID_State)
		{
		    if (MFRC522_Card_Request())
		    {
		    	MFRC522_Card_AnticollSelect(&CardNUID[0]);
		    	MFRC522_Card_Read_Block(MIFARE_NameAddress, &mRFIDName[0], &CardNUID[0]);
		    	MFRC522_Card_Read_Block(MIFARE_RegisterAddress, &mRFIDRegister[0], &CardNUID[0]);
		    	MFRC522_Card_Halt();
		    	TMR_StartLowPowerTimer(mRfidUartTimerId, gTmrLowPowerIntervalMillisTimer_c, TmrMilliseconds(250), RfidUartTimerCallback, NULL);
		    	RFID_State = (uint8_t)RFID_RemoveCard;
		    }
		}
		else if (RFID_WriteUserNameAndRegister == RFID_State)
		{
			GPIOB->PSOR = 1U << 3U;

		    if (MFRC522_Card_Request())
		    {
		    	MFRC522_Card_AnticollSelect(&CardNUID[0]);
		    	MFRC522_Card_Write_Block(MIFARE_NameAddress, &mRFIDName[0], 16,  &CardNUID[0]);
		    	MFRC522_Card_Write_Block(MIFARE_RegisterAddress, &mRFIDRegister[0], 8,  &CardNUID[0]);
		    	MFRC522_Card_Halt();
		    	GPIOB->PCOR = 1U << 3U;
		    	RFID_State = (uint8_t)RFID_RemoveCard;
		    }
		}
		else if (isKeyAvailable && RFID_WriteLockerUserKey == RFID_State)
		{
			GPIOB->PSOR = 1U << 3U;

		    if (MFRC522_Card_Request())
		    {

		    	MFRC522_Card_AnticollSelect(&CardNUID[0]);
		    	MFRC522_Card_Write_Block(MIFARE_KeyAddress, &mLockerKey[0], 4,  &CardNUID[0]);
		    	MFRC522_Card_Halt();
		    	GPIOB->PCOR = 1U << 3U;
		    	RFID_State = (uint8_t)RFID_RemoveCard;
		    	isKeyAvailable = FALSE;
		    }
		}
		else if (RFID_WriteLockerMaterialKey == RFID_State)
		{
			GPIOB->PSOR = 1U << 3U;

		    if (MFRC522_Card_Request())
		    {
		    	MFRC522_Card_AnticollSelect(&CardNUID[0]);
		    	MFRC522_Card_Write_Block(MIFARE_TagMaterial, &mMaterialKey[0], 4,  &CardNUID[0]);
		    	MFRC522_Card_Halt();
		    	GPIOB->PCOR = 1U << 3U;
		    	RFID_State = (uint8_t)RFID_RemoveCard;
		    	isKeyAvailable = FALSE;
		    }
		}
		else if (RFID_RemoveCard == RFID_State)
		{
		    if (MFRC522_Card_Request())
		    {
		    	MFRC522_Card_Halt();
		    }
		    else
		    {
		    	RFID_State = (uint8_t)RFID_ReadUserNameAndRegister;
		    }
		}

		OSA_TimeDelay(1000);
	}
}
