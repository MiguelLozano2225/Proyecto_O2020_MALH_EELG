/*
 * SPI.h
 *
 *  Created on: Nov 19, 2017
 *      Author: Edgar
 */

#ifndef SPI_H_
#define SPI_H_

#include "derivative.h"

#ifndef F_CPU
#define F_CPU 						20970000
#endif


#define CS0_HIGH					GPIOD_PDOR |= 1
#define CS1_HIGH					GPIOC_PDOR |= (1<<4)
#define CS2_HIGH					GPIOC_PDOR |= (1<<12)
#define CS0_LOW						GPIOD_PDOR &= ~1
#define CS1_LOW						GPIOC_PDOR &= ~(1<<4)
#define CS2_LOW						GPIOC_PDOR &= ~(1<<12)

#define SPI_INSTANCE				stptr_SPI[eSPI0]
#define SPI_CPOL					eCPOL0			
#define SPI_CPHA					eCPHA0
#define SPI_BR						eBR2

#define SPI_DATA_LOAD(x) 			(SPI_PUSHR_TXDATA(x))
#define SPI_EOTF_DATA_LOAD(x)		(SPI_PUSHR_TXDATA(x))

static const SPI_MemMapPtr stptr_SPI[] = SPI_BASE_PTRS;
void SPI_Init(void);
unsigned char mSPIMstrTxRx (unsigned int Data); 

enum eSPI_Instance{
	
	eSPI0 = 0,
	eSPI1,
	eSPI2
	
};

enum eSPI_CPOL{
	
	eCPOL0 = 0,
	eCPOL1

};

enum eSPI_CPHA{
	
	eCPHA0 = 0,
	eCPHA1
	
};

enum eSPI_BR{

	eBR2 = 0,
	eBR4,
	eBR6,
	eBR8,
	eBR16,
	eBR32,
	eBR64,
	eBR128,
	eBR256,
	eBR512,
	eBR1024,
	eBR2048,
	eBR4096,
	eBR8192,
	eBR16384,
	eBR32768
	
};

// FSCK = (F_CPU/(2*BR))

#endif /* SPI_H_ */
