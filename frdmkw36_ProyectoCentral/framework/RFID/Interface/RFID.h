/*
 * RFID.h
 *
 *  Created on: 30 jul. 2020
 *      Author: Edgar
 */

#ifndef RFID_INTERFACE_RFID_H_
#define RFID_INTERFACE_RFID_H_

#include "EmbeddedTypes.h"
#include "fsl_dspi.h"
#include "pin_mux.h"

/* MFRC522 driver definitions */
#define RFID_WRITE_MASK 		(0x00)
#define RFID_READ_MASK			(0x80)
#define RFID_NOP				(0x00)
#define RFID_FRSZ_7				(0x07)
#define RFID_FRSZ_8				(0x00)
#define RFID_SEND				(0x80)

/* MFRC522 SPI Tx-Rx driver function */
uint8_t mSPIMstrTxRx (uint8_t Byte, dspi_command_data_config_t *command);

/* MFRC522 driver functions */
void MFRC522_Write (uint8_t Address, uint8_t Data);
uint8_t MFRC522_Read (uint8_t Address);

/* MFRC522 functions*/
void MFRC522_Init (void);
bool_t MFRC522_Card_Request (void);
void MFRC522_Card_AnticollSelect (uint8_t *NUID);
void MFRC522_Card_Read_Block (uint8_t Address, uint8_t* Data, uint8_t *NUID);
void MFRC522_Card_Write_Block (uint8_t Address, uint8_t* Data, uint8_t DataSize, uint8_t *NUID);
void MFRC522_Card_Halt (void);
void MFRC522_CRC (uint8_t *Data_ptr, uint8_t Length);
bool_t MFRC522_Transference (uint8_t Command, uint8_t *Data_ptr, uint8_t Length, uint8_t *Request_ptr);

/* MFRC522 Registers */
#define Reserved_1 			(0x00<<1)
#define CommandReg 			(0x01<<1)
#define ComIEnReg			(0x02<<1)
#define DivIEnReg			(0x03<<1)
#define ComIrqReg			(0x04<<1)
#define DivIrqReg			(0x05<<1)
#define	ErrorReg			(0x06<<1)
#define Status1Reg			(0x07<<1)
#define Status2Reg			(0x08<<1)
#define FIFODataReg         (0x09<<1)
#define FIFOLevelReg		(0x0A<<1)
#define WaterLevelReg		(0x0B<<1)
#define ControlReg			(0x0C<<1)
#define BitFramingReg		(0x0D<<1)
#define CollReg				(0x0E<<1)
#define Reserved_2			(0x0F<<1)
#define Reserved_3			(0x10<<1)
#define	ModeReg				(0x11<<1)
#define TxModeReg			(0x12<<1)
#define RxModeReg			(0x13<<1)
#define TxControlReg		(0x14<<1)
#define TxASKReg			(0x15<<1)
#define TxSelReg			(0x17<<1)
#define RxThresholdReg		(0x18<<1)
#define DemodReg			(0x19<<1)
#define Reserved_4			(0x1A<<1)
#define Reserved_5			(0x1B<<1)
#define MfTxReg				(0x1C<<1)
#define MfRxReg				(0x1D<<1)
#define Reserved_6			(0x1E<<1)
#define SerialSpeedReg		(0x1F<<1)
#define Reserved_7			(0x20<<1)
#define CRCResultReg_MSB	(0x21<<1)
#define CRCResultReg_LSB	(0x22<<1)
#define Reserved_8			(0x23<<1)
#define ModWidthReg			(0x24<<1)
#define Reserved_9			(0x25<<1)
#define RFCfgReg			(0x26<<1)
#define GsNReg				(0x27<<1)
#define CWGsPReg			(0x28<<1)
#define ModGsPReg			(0x29<<1)
#define TModeReg			(0x2A<<1)
#define TPrescalerReg		(0x2B<<1)
#define TReloadReg_MSB		(0x2C<<1)
#define TReloadReg_LSB		(0x2D<<1)
#define TCounterValReg_MSB	(0x2E<<1)
#define TCounterValReg_LSB	(0x2F<<1)
#define Reserved_10			(0x30<<1)
#define	TestSel1Reg			(0x31<<1)
#define TestSel2Reg			(0x32<<1)
#define TestPinEnReg		(0x33<<1)
#define TestPinValueReg		(0x34<<1)
#define TestBusReg			(0x35<<1)
#define AutoTestReg			(0x36<<1)
#define VersionReg			(0x37<<1)
#define AnalogTestReg		(0x38<<1)
#define TestDAC1Reg			(0x39<<1)
#define TestDAC2Reg			(0x3A<<1)
#define TestADCReg			(0x3B<<1)
#define Reserved_11			(0x3C<<1)
#define Reserved_12			(0x3D<<1)
#define Reserved_13			(0x3E<<1)
#define Reserved_14			(0x3F<<1)

/* MFRC522 Command set */
#define Idle				0x0
#define Mem					0x1
#define GenerateRandomID	0x2
#define CalcCRC				0x3
#define Transmit			0x4
#define NoCmdChange			0x7
#define Receive				0x8
#define Transceive			0xC
#define MFAuthent			0xE
#define SoftReset			0xF

/* MIFARE Command set */
#define REQA				0x26
#define WUPA				0x52
#define Anticoll_CL1		{0x93,0x20}
#define Select_CL1			{0x93,0x70}
#define Anticoll_CL2		{0x95,0x20}
#define Select_CL2			{0x95,0x70}
#define Halt				{0x50,0x00}
#define Authent_KeyA		0x60
#define Authent_KeyB		0x61
#define Personal_UID		0x40
#define SET_MOD_TYPE		0x43
#define MIFARE_Read			0x30
#define MIFARE_Write		0xA0
#define MIFARE_Decrement	0xC0
#define MIFARE_Increment	0xC1
#define MIFARE_Restore		0xC2
#define MIFARE_Transfer		0xB0

/* RFID Application */
#define MIFARE_NameAddress 		(0x01)
#define MIFARE_RegisterAddress	(0x02)
#define MIFARE_KeyAddress		(0x04)
#define MIFARE_TagMaterial		(0x05)

/* RFID task definitions */
#define gRFIDTaskPriority_c		(4)
#define gRFIDTaskStackSize_c	(300)

/* RFID task */
void RFID_Init (void);

#endif /* RFID_INTERFACE_RFID_H_ */
