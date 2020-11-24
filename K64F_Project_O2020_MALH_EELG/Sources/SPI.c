/*
 * SPI.c
 *
 *  Created on: Nov 19, 2017
 *      Author: Edgar
 */

#include "SPI.h"

unsigned char SPI_RX_Flag=0, SPI_RX_Data;

void SPI_Init(void){
	
	SPI_INSTANCE->MCR = SPI_MCR_MSTR_MASK /*| SPI_MCR_PCSIS(0x01)*/ | SPI_MCR_DIS_TXF_MASK | SPI_MCR_DIS_RXF_MASK | SPI_MCR_HALT_MASK; 		//Configure as master SPI and disable FIFO function 
	SPI_INSTANCE->CTAR[0] = SPI_CTAR_BR(SPI_BR) | SPI_CTAR_FMSZ(7) | (SPI_CPHA<<SPI_CTAR_CPHA_SHIFT) | (SPI_CPOL<<SPI_CTAR_CPOL_SHIFT); //Configure frame size and baudrate
	SPI_INSTANCE->RSER = SPI_RSER_RFDF_RE_MASK; 																						//Enable receive interrupt
	SPI_INSTANCE->SR = SPI_SR_RFDF_MASK; 																								//Clear all interrupt request 
	SPI_INSTANCE->MCR &= ~SPI_MCR_HALT_MASK;																							//Start transmission

}

unsigned char mSPIMstrTxRx (unsigned int Data){
	
	SPI_INSTANCE->PUSHR = Data;
	while(!SPI_RX_Flag);
	SPI_RX_Flag = 0;
	
	return SPI_RX_Data;
	
}


void SPI0_IRQHandler(void){
	
	if((SPI_INSTANCE->SR) & SPI_SR_RFDF_MASK) {
	
		SPI_RX_Data = (unsigned char)(SPI_INSTANCE->POPR);
		(SPI_INSTANCE->SR) |= SPI_SR_RFDF_MASK;
		SPI_RX_Flag = 1;
		
	}

}
